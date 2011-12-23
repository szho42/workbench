#ifndef __OCT_TREE_H__
#define __OCT_TREE_H__

#include "MathFunctions.h"

namespace caret
{
    ///low level Oct structure with a bunch of helper members, use it to build your own tree of Octs, possibly by extension
    template<typename T>
    struct Oct
    {
        //data
        T m_data;
        //children
        Oct<T>* m_children[2][2][2];
        Oct<T>* m_parent;
        bool m_leaf;
        float m_bounds[3][3];
        
        Oct();
        Oct(const float minCoords[3], const float maxCoords[3]);
        ~Oct();
        void makeChildren();
        void deleteChildren();
        ///makes an Oct with this node as the child specified by octant
        Oct* makeParent(const int octant[3]);
        float distToPoint(const float point[3]);
        bool lineIntersects(const float p1[3], const float p2[3]);
        bool rayIntersects(const float start[3], const float p2[3]);
        bool lineSegmentIntersects(const float start[3], const float end[3]);
        bool pointInside(const float point[3]);
        bool boundsOverlaps(const float minCoords[3], const float maxCoords[3]);
        ///returns which child Oct the point would be contained in if the point were inside this Oct
        Oct* containingChild(const float point[3], int* whichOct = NULL);
    };
    
    template<typename T>
    Oct<T>::Oct()
    {
        for (int i = 0; i < 2; ++i)
        {
            m_children[i][0][0] = NULL; m_children[i][0][1] = NULL;
            m_children[i][1][0] = NULL; m_children[i][1][1] = NULL;
        }
        m_parent = NULL;
        m_leaf = true;
    }

    template<typename T>
    Oct<T>::Oct(const float minCoords[3], const float maxCoords[3])
    {
        for (int i = 0; i < 2; ++i)
        {
            m_children[i][0][0] = NULL; m_children[i][0][1] = NULL;
            m_children[i][1][0] = NULL; m_children[i][1][1] = NULL;
        }
        m_parent = NULL;
        m_leaf = true;
        for (int i = 0; i < 3; ++i)
        {
            m_bounds[i][0] = minCoords[i];
            m_bounds[i][2] = maxCoords[i];
            m_bounds[i][1] = (m_bounds[i][0] + m_bounds[i][2]) * 0.5f;
        }
    }

    template<typename T>
    Oct<T>::~Oct()
    {
        deleteChildren();
    }
    
    template<typename T>
    void Oct<T>::makeChildren()
    {
        m_leaf = false;
        int ijk[3];
        for (ijk[0] = 0; ijk[0] < 2; ++ijk[0])
        {
            for (ijk[1] = 0; ijk[1] < 2; ++ijk[1])
            {
                for (ijk[2] = 0; ijk[2] < 2; ++ijk[2])
                {
                    Oct<T>* temp = new Oct<T>();
                    m_children[ijk[0]][ijk[1]][ijk[2]] = temp;
                    temp->m_parent = this;
                    for (int m = 0; m < 3; ++m)
                    {
                        temp->m_bounds[m][0] = m_bounds[m][ijk[m]];
                        temp->m_bounds[m][2] = m_bounds[m][ijk[m] + 1];
                        temp->m_bounds[m][1] = (temp->m_bounds[m][0] + temp->m_bounds[m][2]) * 0.5f;
                    }
                }
            }
        }
    }
    
    template<typename T>
    void Oct<T>::deleteChildren()
    {
        m_leaf = true;
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                for (int k = 0; k < 2; ++k)
                {
                    if (m_children[i][j][k] != NULL)
                    {
                        delete m_children[i][j][k];
                        m_children[i][j][k] = NULL;
                    }
                }
            }
        }
    }
    
    template<typename T>
    Oct<T>* Oct<T>::makeParent(const int octant[3])
    {
        Oct<T>* ret = new Oct<T>();
        for (int i = 0; i < 3; ++i)
        {
            ret->m_bounds[i][octant[i]] = m_bounds[i][0];
            ret->m_bounds[i][octant[i] + 1] = m_bounds[i][1];
            ret->m_bounds[i][(octant[i] + 2) % 3] = (octant[i] ? (2.0f * m_bounds[i][0] - m_bounds[i][1]) : (2.0f * m_bounds[i][1] - m_bounds[i][0]));
        }
        ret->makeChildren();//do this the lazy way (code wise), with an extra new and delete - we shouldn't be doing this very often, so not important
        delete ret->m_children[octant[0]][octant[1]][octant[2]];
        ret->m_children[octant[0]][octant[1]][octant[2]] = this;
        m_parent = ret;
        return ret;
    }
    
    template<typename T>
    float Oct<T>::distToPoint(const float point[3])
    {
        float temp[3];
        for (int i = 0; i < 3; ++i)
        {
            if (point[i] < m_bounds[i][0])
            {
                temp[i] = m_bounds[i][0] - point[i];
            } else {
                if (point[i] > m_bounds[i][2])
                {
                    temp[i] = m_bounds[i][2] - point[i];
                } else {
                    temp[i] = 0.0f;
                }
            }
        }
        return MathFunctions::vectorLength(temp);
    }
    
    template<typename T>
    bool Oct<T>::lineIntersects(const float p1[3], const float p2[3])
    {
        float direction[3];
        float curlow = 1.0f, curhigh = -1.0f;//quiet compiler, make default say "false", but we use pointInside on zero length queries
        MathFunctions::subtractVectors(p2, p1, direction);
        bool first = true;
        for (int i = 0; i < 3; ++i)
        {
            if (direction[i] != 0.0f)
            {
                float templow = (m_bounds[i][0] - p1[i]) / direction[i];//compute the range of t over which this line lies between the planes for this axis
                float temphigh = (m_bounds[i][2] - p1[i]) / direction[i];
                if (first)
                {
                    first = false;
                    curlow = templow;
                    curhigh = temphigh;
                } else {
                    if (templow > curlow) curlow = templow;//intersect the ranged
                    if (temphigh < curhigh) curhigh = temphigh;
                    if (curhigh < curlow) return false;//if intersection is null, false
                }
            }
        }
        if (first)
        {
            return pointInside(p1);
        }
        return true;
    }
    
    template<typename T>
    bool Oct<T>::rayIntersects(const float start[3], const float p2[3])
    {
        float direction[3];
        float curlow = 1.0f, curhigh = -1.0f;//quiet compiler, make default say "false", but we use pointInside on zero length queries
        MathFunctions::subtractVectors(p2, start, direction);
        bool first = true;
        for (int i = 0; i < 3; ++i)
        {
            if (direction[i] != 0.0f)
            {
                float templow = (m_bounds[i][0] - start[i]) / direction[i];//compute the range of t over which this line lies between the planes for this axis
                float temphigh = (m_bounds[i][2] - start[i]) / direction[i];
                if (first)
                {
                    first = false;
                    curlow = templow;
                    curhigh = temphigh;
                } else {
                    if (templow > curlow) curlow = templow;//intersect the ranged
                    if (temphigh < curhigh) curhigh = temphigh;
                    if (curhigh < curlow || curhigh < 0.0f) return false;//if intersection is null or has no positive range, false
                }
            }
        }
        if (first)
        {
            return pointInside(start);
        }
        return true;
    }
    
    template<typename T>
    bool Oct<T>::lineSegmentIntersects(const float start[3], const float end[3])
    {
        float direction[3];
        float curlow = 1.0f, curhigh = -1.0f;//quiet compiler, make default say "false", but we use pointInside on zero length queries
        MathFunctions::subtractVectors(end, start, direction);//parameterize the line segment to the range [0, 1] of t
        bool first = true;
        for (int i = 0; i < 3; ++i)
        {
            if (direction[i] != 0.0f)
            {
                float templow = (m_bounds[i][0] - start[i]) / direction[i];//compute the range of t over which this line lies between the planes for this axis
                float temphigh = (m_bounds[i][2] - start[i]) / direction[i];
                if (first)
                {
                    first = false;
                    curlow = templow;
                    curhigh = temphigh;
                } else {
                    if (templow > curlow) curlow = templow;//intersect the ranged
                    if (temphigh < curhigh) curhigh = temphigh;
                    if (curhigh < curlow || curhigh < 0.0f || curlow > 1.0f) return false;//if intersection is null or has no positive range, or has no range less than 1, false
                }
            }
        }
        if (first)
        {
            return pointInside(start);
        }
        return true;
    }
    
    template<typename T>
    bool Oct<T>::pointInside(const float point[3])
    {
        for (int i = 0; i < 3; ++i)
        {
            if (point[i] < m_bounds[i][0] || point[i] > m_bounds[i][2]) return false;//be permissive, equal to boundary falls into both, though for traversal, strictly less than the boundary is the test condition
        }
        return true;
    }
    
    template<typename T>
    bool Oct<T>::boundsOverlaps(const float minCoords[3], const float maxCoords[3])
    {
        for (int i = 0; i < 3; ++i)
        {
            if (maxCoords[i] < m_bounds[i][0] || minCoords[i] > m_bounds[i][2]) return false;//be permissive, equal to boundary falls into both
        }
        return true;
    }
    
    template<typename T>
    Oct<T>* Oct<T>::containingChild(const float point[3], int* whichOct)
    {
        int myOct[3];
        for (int i = 0; i < 3; ++i)
        {
            myOct[i] = (point[i] < m_bounds[i][1] ? 0 : 1);//strictly less than, using only the midpoint is how traversal works, even if the point isn't inside the Oct
            if (whichOct != NULL) whichOct[i] = myOct[i];
        }
        return m_children[myOct[0]][myOct[1]][myOct[2]];
    }
}

#endif //__OCT_TREE_H__