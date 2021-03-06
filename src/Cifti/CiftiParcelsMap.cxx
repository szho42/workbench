/*LICENSE_START*/
/*
 *  Copyright (C) 2014  Washington University School of Medicine
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
/*LICENSE_END*/

#include "CiftiParcelsMap.h"

#include "CaretException.h"
#include "CaretLogger.h"

#include <QStringList>
#include <QRegExp>

using namespace std;
using namespace caret;

void CiftiParcelsMap::addParcel(const CiftiParcelsMap::Parcel& parcel)
{
    int64_t voxelListSize = (int64_t)parcel.m_voxelIndices.size();
    CaretCompact3DLookup<int64_t> tempLookup = m_volLookup;//a copy of the lookup should be faster than other methods of checking for overlap and repeat
    int64_t thisParcel = m_parcels.size();
    if (voxelListSize != 0)
    {
        const int64_t* dims = NULL;
        if (!m_ignoreVolSpace)
        {
            if (!m_haveVolumeSpace)
            {
                throw CaretException("you must set the volume space before adding parcels that use voxels");
            }
            dims = m_volSpace.getDims();
        }
        for (set<VoxelIJK>::const_iterator iter = parcel.m_voxelIndices.begin(); iter != parcel.m_voxelIndices.end(); ++iter)//do all error checking before adding to lookup - might be unnecessary
        {
            if (iter->m_ijk[0] < 0 || iter->m_ijk[1] < 0 || iter->m_ijk[2] < 0)
            {
                throw CaretException("found negative index triple in voxel list");
            }
            if (!m_ignoreVolSpace && (iter->m_ijk[0] >= dims[0] ||
                                      iter->m_ijk[1] >= dims[1] ||
                                      iter->m_ijk[2] >= dims[2]))
            {
                throw CaretException("found invalid index triple in voxel list");
            }
            if (tempLookup.find(iter->m_ijk) != NULL)
            {
                throw CaretException("parcels may not overlap in voxels");
            }
            tempLookup.at(iter->m_ijk) = thisParcel;
        }
    }
    for (map<StructureEnum::Enum, set<int64_t> >::const_iterator iter = parcel.m_surfaceNodes.begin(); iter != parcel.m_surfaceNodes.end(); ++iter)
    {
        map<StructureEnum::Enum, SurfaceInfo>::const_iterator info = m_surfInfo.find(iter->first);
        if (info == m_surfInfo.end())
        {
            throw CaretException("you must set surfaces before adding parcels that use them");
        }
        const set<int64_t>& nodeSet = iter->second;
        if (nodeSet.size() == 0)
        {
            throw CaretException("parcels may not include empty node lists");//NOTE: technically not required by Cifti, change if problematic, but probably never allow empty list in internal state
        }
        for (set<int64_t>::const_iterator iter2 = nodeSet.begin(); iter2 != nodeSet.end(); ++iter2)
        {
            if (*iter2 < 0)
            {
                throw CaretException("found negative vertex in parcel");
            }
            if (*iter2 >= info->second.m_numNodes)
            {
                throw CaretException("found invalid vertex in parcel");
            }
            if (info->second.m_lookup[*iter2] != -1)
            {
                throw CaretException("parcels may not overlap in vertices");
            }
        }
    }
    if (voxelListSize != 0)//all error checking done, modify
    {
        m_volLookup = tempLookup;
    }
    for (map<StructureEnum::Enum, set<int64_t> >::const_iterator iter = parcel.m_surfaceNodes.begin(); iter != parcel.m_surfaceNodes.end(); ++iter)
    {
        map<StructureEnum::Enum, SurfaceInfo>::iterator info = m_surfInfo.find(iter->first);
        CaretAssert(info != m_surfInfo.end());
        const set<int64_t>& nodeSet = iter->second;
        for (set<int64_t>::const_iterator iter2 = nodeSet.begin(); iter2 != nodeSet.end(); ++iter2)
        {
            CaretAssertVectorIndex(info->second.m_lookup, *iter2);
            info->second.m_lookup[*iter2] = thisParcel;
        }
    }
    m_parcels.push_back(parcel);
}

void CiftiParcelsMap::addSurface(const int& numberOfNodes, const StructureEnum::Enum& structure)
{
    map<StructureEnum::Enum, SurfaceInfo>::const_iterator test = m_surfInfo.find(structure);
    if (test != m_surfInfo.end())
    {
        throw CaretException("parcel surface structures may not be used more than once");
    }
    SurfaceInfo tempInfo;
    tempInfo.m_numNodes = numberOfNodes;
    tempInfo.m_lookup.resize(numberOfNodes, -1);
    m_surfInfo[structure] = tempInfo;
}

void CiftiParcelsMap::clear()
{
    m_haveVolumeSpace = false;
    m_ignoreVolSpace = false;
    m_parcels.clear();
    m_surfInfo.clear();
    m_volLookup.clear();
}

void CiftiParcelsMap::setVolumeSpace(const VolumeSpace& space)
{
    const int64_t* dims = space.getDims();
    int64_t numParcels = (int64_t)m_parcels.size();
    for (int64_t i = 0; i < numParcels; ++i)
    {
        const set<VoxelIJK>& voxelList = m_parcels[i].m_voxelIndices;
        for (set<VoxelIJK>::const_iterator iter = voxelList.begin(); iter != voxelList.end(); ++iter)
        {
            if (iter->m_ijk[0] >= dims[0] ||
                iter->m_ijk[1] >= dims[1] ||
                iter->m_ijk[2] >= dims[2])
            {
                throw CaretException("parcels may not contain voxel indices outside the volume space");
            }
        }
    }
    m_haveVolumeSpace = true;
    m_ignoreVolSpace = false;
    m_volSpace = space;
}

int64_t CiftiParcelsMap::getIndexForNode(const int64_t& node, const StructureEnum::Enum& structure) const
{
    CaretAssert(node >= 0);
    map<StructureEnum::Enum, SurfaceInfo>::const_iterator test = m_surfInfo.find(structure);
    if (test == m_surfInfo.end()) return -1;
    if (node >= test->second.m_numNodes) return -1;
    CaretAssertVectorIndex(test->second.m_lookup, node);
    return test->second.m_lookup[node];
}

int64_t CiftiParcelsMap::getIndexForVoxel(const int64_t* ijk) const
{
    return getIndexForVoxel(ijk[0], ijk[1], ijk[2]);
}

int64_t CiftiParcelsMap::getIndexForVoxel(const int64_t& i, const int64_t& j, const int64_t& k) const
{
    const int64_t* test = m_volLookup.find(i, j, k);//the lookup tolerates weirdness like negatives
    if (test == NULL) return -1;
    return *test;
}

vector<StructureEnum::Enum> CiftiParcelsMap::getParcelSurfaceStructures() const
{
    vector<StructureEnum::Enum> ret;
    ret.reserve(m_surfInfo.size());
    for (map<StructureEnum::Enum, SurfaceInfo>::const_iterator iter = m_surfInfo.begin(); iter != m_surfInfo.end(); ++iter)
    {
        ret.push_back(iter->first);
    }
    return ret;
}

int64_t CiftiParcelsMap::getIndexFromNumberOrName(const QString& numberOrName) const
{
    bool ok = false;
    int64_t ret = numberOrName.toLongLong(&ok) - 1;//quirk: use string "1" as the first index
    if (ok)
    {
        if (ret < 0 || ret >= getLength()) return -1;//if it is a number, do not try to use it as a name, under any circumstances
        return ret;
    } else {
        int64_t length = getLength();
        for (int64_t i = 0; i < length; ++i)
        {
            if (numberOrName == m_parcels[i].m_name) return i;
        }
        return -1;
    }
}

const VolumeSpace& CiftiParcelsMap::getVolumeSpace() const
{
    CaretAssert(!m_ignoreVolSpace);//this should never be set except during parsing of cifti-1
    if (!m_haveVolumeSpace)
    {
        throw CaretException("getVolumeSpace called when no volume space exists");
    }
    return m_volSpace;
}

int64_t CiftiParcelsMap::getSurfaceNumberOfNodes(const StructureEnum::Enum& structure) const
{
    map<StructureEnum::Enum, SurfaceInfo>::const_iterator iter = m_surfInfo.find(structure);
    if (iter == m_surfInfo.end()) return -1;
    return iter->second.m_numNodes;
}

bool CiftiParcelsMap::hasSurfaceData(const StructureEnum::Enum& structure) const
{
    if (m_surfInfo.find(structure) == m_surfInfo.end()) return false;
    int64_t numParcels = (int64_t)m_parcels.size();
    for (int64_t i = 0; i < numParcels; ++i)
    {
        map<StructureEnum::Enum, set<int64_t> >::const_iterator iter = m_parcels[i].m_surfaceNodes.find(structure);
        if (iter != m_parcels[i].m_surfaceNodes.end() && iter->second.size() != 0) return true;
    }
    return false;
}

bool CiftiParcelsMap::hasVolumeData() const
{
    CaretAssert(!m_ignoreVolSpace);
    int64_t numParcels = (int64_t)m_parcels.size();//NOTE: this function is used when reading cifti-1 to determine whether it is an error to not have a volume space, so we can't just check m_haveVolumeSpace
    for (int64_t i = 0; i < numParcels; ++i)
    {
        if (m_parcels[i].m_voxelIndices.size() != 0) return true;
    }
    return false;
}

bool CiftiParcelsMap::approximateMatch(const CiftiMappingType& rhs) const
{
    if (rhs.getType() != getType()) return false;
    const CiftiParcelsMap& myrhs = dynamic_cast<const CiftiParcelsMap&>(rhs);
    CaretAssert(!m_ignoreVolSpace && !myrhs.m_ignoreVolSpace);
    if (m_haveVolumeSpace != myrhs.m_haveVolumeSpace) return false;
    if (m_haveVolumeSpace && m_volSpace != myrhs.m_volSpace) return false;
    if (m_surfInfo.size() != myrhs.m_surfInfo.size()) return false;//as below, return false if they won't write the mapping part to xml the same - 1 to 1 compare only requires 1 simple loop
    for (map<StructureEnum::Enum, SurfaceInfo>::const_iterator iter = m_surfInfo.begin(); iter != m_surfInfo.end(); ++iter)
    {
        map<StructureEnum::Enum, SurfaceInfo>::const_iterator rhsiter = myrhs.m_surfInfo.find(iter->first);
        if (rhsiter == myrhs.m_surfInfo.end()) return false;//technically, they might still have the same meaning, if the surface isn't used, but they will still write differently, so false
        if (iter->second.m_numNodes != rhsiter->second.m_numNodes) return false;
    }
    if (m_parcels.size() != myrhs.m_parcels.size()) return false;
    for (int64_t i = 0; i < (int64_t)m_parcels.size(); ++i)
    {
        if (!m_parcels[i].approximateMatch(myrhs.m_parcels[i])) return false;
    }
    return true;
}

bool CiftiParcelsMap::operator==(const CiftiMappingType& rhs) const
{
    if (rhs.getType() != getType()) return false;
    const CiftiParcelsMap& myrhs = dynamic_cast<const CiftiParcelsMap&>(rhs);
    CaretAssert(!m_ignoreVolSpace && !myrhs.m_ignoreVolSpace);
    if (m_haveVolumeSpace != myrhs.m_haveVolumeSpace) return false;
    if (m_haveVolumeSpace && m_volSpace != myrhs.m_volSpace) return false;
    if (m_surfInfo.size() != myrhs.m_surfInfo.size()) return false;//as below, return false if they won't write to xml the same - 1 to 1 compare only requires 1 simple loop
    for (map<StructureEnum::Enum, SurfaceInfo>::const_iterator iter = m_surfInfo.begin(); iter != m_surfInfo.end(); ++iter)
    {
        map<StructureEnum::Enum, SurfaceInfo>::const_iterator rhsiter = myrhs.m_surfInfo.find(iter->first);
        if (rhsiter == myrhs.m_surfInfo.end()) return false;//technically, they might still have the same meaning, if the surface isn't used, but they will still write differently, so false
        if (iter->second.m_numNodes != rhsiter->second.m_numNodes) return false;
    }
    return (m_parcels == myrhs.m_parcels);
}

bool CiftiParcelsMap::Parcel::operator==(const CiftiParcelsMap::Parcel& rhs) const
{
    if (m_name != rhs.m_name) return false;
    if (m_voxelIndices != rhs.m_voxelIndices) return false;
    return (m_surfaceNodes == rhs.m_surfaceNodes);
}

//same, but don't test name
bool CiftiParcelsMap::Parcel::approximateMatch(const CiftiParcelsMap::Parcel& rhs) const
{
    if (m_voxelIndices != rhs.m_voxelIndices) return false;
    return (m_surfaceNodes == rhs.m_surfaceNodes);
}

void CiftiParcelsMap::readXML1(QXmlStreamReader& xml)
{
    CaretLogFiner("parsing nonstandard parcels mapping type in cifti-1");
    clear();
    m_ignoreVolSpace = true;//cifti-1 has volume space outside the index map
    vector<Parcel> myParcels;//because we need to add the surfaces first
    while (xml.readNextStartElement())
    {
        QStringRef name = xml.name();
        if (name == "Surface")
        {
            QXmlStreamAttributes attrs = xml.attributes();
            if (!attrs.hasAttribute("BrainStructure"))
            {
                throw CaretException("Surface element missing required attribute BrainStructure");
            }
            bool ok = false;
            StructureEnum::Enum tempStructure = StructureEnum::fromCiftiName(attrs.value("BrainStructure").toString(), &ok);
            if (!ok)
            {
                throw CaretException("invalid value for BrainStructure: " + attrs.value("BrainStructure").toString());
            }
            if (!attrs.hasAttribute("SurfaceNumberOfNodes"))
            {
                throw CaretException("Surface element missing required attribute SurfaceNumberOfNodes");
            }
            int64_t numNodes = attrs.value("SurfaceNumberOfNodes").toString().toLongLong(&ok);
            if (!ok || numNodes < 1)
            {
                throw CaretException("invalid value for SurfaceNumberOfNodes: " + attrs.value("SurfaceNumberOfNodes").toString());
            }
            addSurface(numNodes, tempStructure);//let the standard modification functions do error checking
            if (xml.readNextStartElement())
            {
                throw CaretException("unexpected element inside Surface: " + xml.name().toString());
            }
        } else if (name == "Parcel") {
            myParcels.push_back(readParcel1(xml));
        } else {
            throw CaretException("unexpected element in parcels map: " + name.toString());
        }
    }
    int64_t numParcels = (int64_t)myParcels.size();
    for (int64_t i = 0; i < numParcels; ++i)
    {
        addParcel(myParcels[i]);
    }
    m_ignoreVolSpace = false;//in case there are no voxels, but some will be added later
    CaretAssert(xml.isEndElement() && xml.name() == "MatrixIndicesMap");
}

void CiftiParcelsMap::readXML2(QXmlStreamReader& xml)
{
    clear();
    vector<Parcel> myParcels;//because we need to add the surfaces and volume space first
    while (xml.readNextStartElement())
    {
        QStringRef name = xml.name();
        if (name == "Surface")
        {
            QXmlStreamAttributes attrs = xml.attributes();
            if (!attrs.hasAttribute("BrainStructure"))
            {
                throw CaretException("Surface element missing required attribute BrainStructure");
            }
            bool ok = false;
            StructureEnum::Enum tempStructure = StructureEnum::fromCiftiName(attrs.value("BrainStructure").toString(), &ok);
            if (!ok)
            {
                throw CaretException("invalid value for BrainStructure: " + attrs.value("BrainStructure").toString());
            }
            if (!attrs.hasAttribute("SurfaceNumberOfVertices"))
            {
                throw CaretException("Surface element missing required attribute SurfaceNumberOfVertices");
            }
            int64_t numNodes = attrs.value("SurfaceNumberOfVertices").toString().toLongLong(&ok);
            if (!ok || numNodes < 1)
            {
                throw CaretException("invalid value for SurfaceNumberOfVertices: " + attrs.value("SurfaceNumberOfVertices").toString());
            }
            addSurface(numNodes, tempStructure);//let the standard modification functions do error checking
            while (!xml.atEnd() && !xml.isEndElement())
            {
                xml.readNext();
                switch (xml.tokenType())
                {
                    case QXmlStreamReader::StartElement:
                        throw CaretException("unexpected element inside Surface: " + xml.name().toString());
                    default:
                        break;
                }
            }
        } else if (name == "Parcel") {
            myParcels.push_back(readParcel2(xml));
            if (xml.hasError()) return;
        } else if (name == "Volume") {
            if (m_haveVolumeSpace)
            {
                throw CaretException("Volume specified more than once in Parcels mapping type");
            } else {
                m_volSpace.readCiftiXML2(xml);
                if (xml.hasError()) return;
                m_haveVolumeSpace = true;
            }
        } else {
            throw CaretException("unexpected element in parcels map: " + name.toString());
        }
    }
    int64_t numParcels = (int64_t)myParcels.size();
    for (int64_t i = 0; i < numParcels; ++i)
    {
        addParcel(myParcels[i]);
    }
    CaretAssert(xml.isEndElement() && xml.name() == "MatrixIndicesMap");
}

CiftiParcelsMap::Parcel CiftiParcelsMap::readParcel1(QXmlStreamReader& xml)
{
    Parcel ret;
    bool haveVoxels = false;
    QXmlStreamAttributes attrs = xml.attributes();
    if (!attrs.hasAttribute("Name"))
    {
        throw CaretException("Parcel element missing required attribute Name");
    }
    ret.m_name = attrs.value("Name").toString();
    while (xml.readNextStartElement())
    {
        QStringRef name = xml.name();
        if (name == "Nodes")
        {
            QXmlStreamAttributes attrs1 = xml.attributes();
            if (!attrs1.hasAttribute("BrainStructure"))
            {
                throw CaretException("Nodes element missing required attribute BrainStructure");
            }
            bool ok = false;
            StructureEnum::Enum myStructure = StructureEnum::fromCiftiName(attrs1.value("BrainStructure").toString(), &ok);
            if (!ok)
            {
                throw CaretException("unrecognized value for BrainStructure: " + attrs1.value("BrainStructure").toString());
            }
            if (ret.m_surfaceNodes.find(myStructure) != ret.m_surfaceNodes.end())
            {
                throw CaretException("Nodes elements may not reuse a BrainStructure within a Parcel");
            }
            set<int64_t>& mySet = ret.m_surfaceNodes[myStructure];
            vector<int64_t> array = readIndexArray(xml);
            if (xml.hasError()) return ret;
            int64_t arraySize = (int64_t)array.size();
            for (int64_t i = 0; i < arraySize; ++i)
            {
                if (mySet.find(array[i]) != mySet.end())
                {
                    throw CaretException("Nodes elements may not reuse indices");
                }
                mySet.insert(array[i]);
            }
        } else if (name == "VoxelIndicesIJK") {
            if (haveVoxels)
            {
                throw CaretException("VoxelIndicesIJK may only appear once in a Parcel");
            }
            vector<int64_t> array = readIndexArray(xml);
            if (xml.hasError()) return ret;
            int64_t arraySize = (int64_t)array.size();
            if (arraySize % 3 != 0)
            {
                throw CaretException("number of indices in VoxelIndicesIJK must be a multiple of 3");
            }
            for (int64_t index3 = 0; index3 < arraySize; index3 += 3)
            {
                VoxelIJK temp(array.data() + index3);
                if (ret.m_voxelIndices.find(temp) != ret.m_voxelIndices.end())
                {
                    throw CaretException("VoxelIndicesIJK elements may not reuse voxels");
                }
                ret.m_voxelIndices.insert(temp);
            }
            haveVoxels = true;
        } else {
            throw CaretException("unexpected element in Parcel: " + name.toString());
        }
    }
    CaretAssert(xml.isEndElement() && xml.name() == "Parcel");
    return ret;
}

CiftiParcelsMap::Parcel CiftiParcelsMap::readParcel2(QXmlStreamReader& xml)
{
    Parcel ret;
    bool haveVoxels = false;
    QXmlStreamAttributes attrs = xml.attributes();
    if (!attrs.hasAttribute("Name"))
    {
        throw CaretException("Parcel element missing required attribute Name");
    }
    ret.m_name = attrs.value("Name").toString();
    while (xml.readNextStartElement())
    {
        QStringRef name = xml.name();
        if (name == "Vertices")
        {
            QXmlStreamAttributes attrs1 = xml.attributes();
            if (!attrs1.hasAttribute("BrainStructure"))
            {
                throw CaretException("Vertices element missing required attribute BrainStructure");
            }
            bool ok = false;
            StructureEnum::Enum myStructure = StructureEnum::fromCiftiName(attrs1.value("BrainStructure").toString(), &ok);
            if (!ok)
            {
                throw CaretException("unrecognized value for BrainStructure: " + attrs1.value("BrainStructure").toString());
            }
            if (ret.m_surfaceNodes.find(myStructure) != ret.m_surfaceNodes.end())
            {
                throw CaretException("Vertices elements may not reuse a BrainStructure within a Parcel");
            }
            set<int64_t>& mySet = ret.m_surfaceNodes[myStructure];
            vector<int64_t> array = readIndexArray(xml);
            if (xml.hasError()) return ret;
            int64_t arraySize = (int64_t)array.size();
            for (int64_t i = 0; i < arraySize; ++i)
            {
                if (mySet.find(array[i]) != mySet.end())
                {
                    throw CaretException("Vertices elements may not reuse indices");
                }
                mySet.insert(array[i]);
            }
        } else if (name == "VoxelIndicesIJK") {
            if (haveVoxels)
            {
                throw CaretException("VoxelIndicesIJK may only appear once in a Parcel");
            }
            vector<int64_t> array = readIndexArray(xml);
            if (xml.hasError()) return ret;
            int64_t arraySize = (int64_t)array.size();
            if (arraySize % 3 != 0)
            {
                throw CaretException("number of indices in VoxelIndicesIJK must be a multiple of 3");
            }
            for (int64_t index3 = 0; index3 < arraySize; index3 += 3)
            {
                VoxelIJK temp(array.data() + index3);
                if (ret.m_voxelIndices.find(temp) != ret.m_voxelIndices.end())
                {
                    throw CaretException("VoxelIndicesIJK elements may not reuse voxels");
                }
                ret.m_voxelIndices.insert(temp);
            }
            haveVoxels = true;
        } else {
            throw CaretException("unexpected element in Parcel: " + name.toString());
        }
    }
    CaretAssert(xml.isEndElement() && xml.name() == "Parcel");
    return ret;
}

vector<int64_t> CiftiParcelsMap::readIndexArray(QXmlStreamReader& xml)
{
    vector<int64_t> ret;
    QString text = xml.readElementText();//raises error if it encounters a start element
    if (xml.hasError()) return ret;
    QStringList separated = text.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    int64_t numElems = separated.size();
    ret.reserve(numElems);
    for (int64_t i = 0; i < numElems; ++i)
    {
        bool ok = false;
        ret.push_back(separated[i].toLongLong(&ok));
        if (!ok)
        {
            throw CaretException("found noninteger in index array: " + separated[i]);
        }
    }
    return ret;
}

void CiftiParcelsMap::writeXML1(QXmlStreamWriter& xml) const
{
    CaretAssert(!m_ignoreVolSpace);
    CaretLogFiner("writing nonstandard parcels mapping type in cifti-1");
    xml.writeAttribute("IndicesMapToDataType", "CIFTI_INDEX_TYPE_PARCELS");
    for (map<StructureEnum::Enum, SurfaceInfo>::const_iterator iter = m_surfInfo.begin(); iter != m_surfInfo.end(); ++iter)
    {
        xml.writeStartElement("Surface");
        xml.writeAttribute("BrainStructure", StructureEnum::toCiftiName(iter->first));
        xml.writeAttribute("SurfaceNumberOfNodes", QString::number(iter->second.m_numNodes));
        xml.writeEndElement();
    }
    int64_t numParcels = m_parcels.size();
    for (int64_t i = 0; i < numParcels; ++i)
    {
        xml.writeStartElement("Parcel");
        xml.writeAttribute("Name", m_parcels[i].m_name);
        int64_t numVoxels = (int64_t)m_parcels[i].m_voxelIndices.size();
        if (numVoxels != 0)
        {
            xml.writeStartElement("VoxelIndicesIJK");
            for (set<VoxelIJK>::const_iterator iter = m_parcels[i].m_voxelIndices.begin(); iter != m_parcels[i].m_voxelIndices.end(); ++iter)
            {
                xml.writeCharacters(QString::number(iter->m_ijk[0]) + " " + QString::number(iter->m_ijk[1]) + " " + QString::number(iter->m_ijk[2]) + "\n");
            }
            xml.writeEndElement();
        }
        for (map<StructureEnum::Enum, set<int64_t> >::const_iterator iter = m_parcels[i].m_surfaceNodes.begin(); iter != m_parcels[i].m_surfaceNodes.end(); ++iter)
        {
            if (iter->second.size() != 0)//prevent writing empty elements, regardless
            {
                xml.writeStartElement("Nodes");
                xml.writeAttribute("BrainStructure", StructureEnum::toCiftiName(iter->first));
                set<int64_t>::const_iterator iter2 = iter->second.begin();//which also allows us to write the first one outside the loop, to not add whitespace on the front or back
                xml.writeCharacters(QString::number(*iter2));
                ++iter2;
                for (; iter2 != iter->second.end(); ++iter2)
                {
                    xml.writeCharacters(" " + QString::number(*iter2));
                }
                xml.writeEndElement();
            }
        }
        xml.writeEndElement();
    }
}

void CiftiParcelsMap::writeXML2(QXmlStreamWriter& xml) const
{
    CaretAssert(!m_ignoreVolSpace);
    xml.writeAttribute("IndicesMapToDataType", "CIFTI_INDEX_TYPE_PARCELS");
    if (hasVolumeData())//could be m_haveVolumeSpace if we want to be able to write a volspace without having voxels, but that seems silly
    {
        m_volSpace.writeCiftiXML2(xml);
    }
    for (map<StructureEnum::Enum, SurfaceInfo>::const_iterator iter = m_surfInfo.begin(); iter != m_surfInfo.end(); ++iter)
    {
        xml.writeStartElement("Surface");
        xml.writeAttribute("BrainStructure", StructureEnum::toCiftiName(iter->first));
        xml.writeAttribute("SurfaceNumberOfVertices", QString::number(iter->second.m_numNodes));
        xml.writeEndElement();
    }
    int64_t numParcels = m_parcels.size();
    for (int64_t i = 0; i < numParcels; ++i)
    {
        xml.writeStartElement("Parcel");
        xml.writeAttribute("Name", m_parcels[i].m_name);
        int64_t numVoxels = (int64_t)m_parcels[i].m_voxelIndices.size();
        if (numVoxels != 0)
        {
            xml.writeStartElement("VoxelIndicesIJK");
            for (set<VoxelIJK>::const_iterator iter = m_parcels[i].m_voxelIndices.begin(); iter != m_parcels[i].m_voxelIndices.end(); ++iter)
            {
                xml.writeCharacters(QString::number(iter->m_ijk[0]) + " " + QString::number(iter->m_ijk[1]) + " " + QString::number(iter->m_ijk[2]) + "\n");
            }
            xml.writeEndElement();
        }
        for (map<StructureEnum::Enum, set<int64_t> >::const_iterator iter = m_parcels[i].m_surfaceNodes.begin(); iter != m_parcels[i].m_surfaceNodes.end(); ++iter)
        {
            if (iter->second.size() != 0)//prevent writing empty elements, regardless
            {
                xml.writeStartElement("Vertices");
                xml.writeAttribute("BrainStructure", StructureEnum::toCiftiName(iter->first));
                set<int64_t>::const_iterator iter2 = iter->second.begin();//which also allows us to write the first one outside the loop, to not add whitespace on the front or back
                xml.writeCharacters(QString::number(*iter2));
                ++iter2;
                for (; iter2 != iter->second.end(); ++iter2)
                {
                    xml.writeCharacters(" " + QString::number(*iter2));
                }
                xml.writeEndElement();
            }
        }
        xml.writeEndElement();
    }
}
