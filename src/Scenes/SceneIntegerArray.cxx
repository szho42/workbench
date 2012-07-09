
/*LICENSE_START*/
/*
 * Copyright 2012 Washington University,
 * All rights reserved.
 *
 * Connectome DB and Connectome Workbench are part of the integrated Connectome 
 * Informatics Platform.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the names of Washington University nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR  
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */
/*LICENSE_END*/

#include <algorithm>

#define __SCENE_INTEGER_ARRAY_DECLARE__
#include "SceneIntegerArray.h"
#undef __SCENE_INTEGER_ARRAY_DECLARE__

#include "CaretAssert.h"

using namespace caret;


    
/**
 * \class caret::SceneIntegerArray 
 * \brief For storage of a integer value in a scene.
 */

/**
 * Constructor.
 *
 * @param name
 *   Name of object.
 * @param values
 *   Value in the array.
 * @param numberOfArrayElements
 *   Number of values in the array.
 */
SceneIntegerArray::SceneIntegerArray(const AString& name,
                                     const int32_t values[],
                                     const int32_t numberOfArrayElements)
: ScenePrimitiveArray(name,
                 SceneObjectDataTypeEnum::SCENE_INTEGER,
                 numberOfArrayElements)
{
    m_values.resize(numberOfArrayElements);
    for (int32_t i = 0; i < numberOfArrayElements; i++) {
        m_values[i] = values[i];
    }
}

/**
 * Constructor.
 *
 * @param name
 *   Name of object.
 * @param values
 *   Value in the array.
 */
SceneIntegerArray::SceneIntegerArray(const AString& name,
                                     const std::vector<int32_t>& values)
: ScenePrimitiveArray(name,
                      SceneObjectDataTypeEnum::SCENE_INTEGER,
                      values.size())
{
    m_values = values;
}

/**
 * Constructor that initializes all values to false.
 *
 * @param name
 *   Name of object.
 * @param numberOfArrayElements
 *   Number of values in the array.
 */
SceneIntegerArray::SceneIntegerArray(const AString& name,
                                     const int numberOfArrayElements)
: ScenePrimitiveArray(name,
                      SceneObjectDataTypeEnum::SCENE_INTEGER,
                      numberOfArrayElements)
{
    m_values.resize(numberOfArrayElements);
    std::fill(m_values.begin(),
              m_values.end(),
              false);
}

/**
 * Destructor.
 */
SceneIntegerArray::~SceneIntegerArray()
{
    
}


/**
 * Set a value.
 * @param arrayIndex
 *    Index of the element.
 * @param value
 *    Value of element.
 */
void 
SceneIntegerArray::setValue(const int32_t arrayIndex,
                            const int32_t value)
{
    CaretAssertVectorIndex(m_values, arrayIndex);
    m_values[arrayIndex] = value;
}

/** 
 * Get the values as a boolean. 
 * @param arrayIndex
 *    Index of element.
 * @return The value.
 */
bool 
SceneIntegerArray::booleanValue(const int32_t arrayIndex) const
{    
    CaretAssertVectorIndex(m_values, arrayIndex);
    const bool b = ((m_values[arrayIndex] != 0.0) ? true : false);
    return b;
}

/** 
 * Get the values as a float. 
 * @param arrayIndex
 *    Index of element.
 * @return The value.
 */
float 
SceneIntegerArray::floatValue(const int32_t arrayIndex) const
{    
    CaretAssertVectorIndex(m_values, arrayIndex);
    const float f = static_cast<float>(m_values[arrayIndex]);
    return f;
}

/** 
 * Get the values as a integer. 
 * @param arrayIndex
 *    Index of element.
 * @return The value.
 */
int32_t 
SceneIntegerArray::integerValue(const int32_t arrayIndex) const
{    
    CaretAssertVectorIndex(m_values, arrayIndex);
    return m_values[arrayIndex];
}

/** 
 * Get the values as a string. 
 * @param arrayIndex
 *    Index of element.
 * @return The value.
 */
AString 
SceneIntegerArray::stringValue(const int32_t arrayIndex) const
{    
    CaretAssertVectorIndex(m_values, arrayIndex);
    const AString s = AString::number(m_values[arrayIndex]);
    return s;
}
