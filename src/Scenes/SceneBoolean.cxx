
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

#define __SCENE_BOOLEAN_DECLARE__
#include "SceneBoolean.h"
#undef __SCENE_BOOLEAN_DECLARE__

using namespace caret;


    
/**
 * \class caret::SceneBoolean 
 * \brief For storage of a boolean value in a scene.
 * \ingroup Scene
 *
 * See the documentation in the class Scene for how to use the Scene system.
 */

/**
 * Constructor.
 *
 * @param name
 *   Name of object.
 * @param value
 *   Value of object.
 */
SceneBoolean::SceneBoolean(const AString& name,
                           const bool value)
: ScenePrimitive(name,
                 SceneObjectDataTypeEnum::SCENE_BOOLEAN)
{
    m_value = value;
}

/**
 * Destructor.
 */
SceneBoolean::~SceneBoolean()
{
    
}

/**
 * Set the value.
 * @param value
 *    The new value.
 */
void 
SceneBoolean::setValue(const bool value)
{
    m_value = value;
}

/**
 * @return The value as a boolean data type.
 */
bool 
SceneBoolean::booleanValue() const
{
    return m_value;
}

/**
 * @return The value as a float data type.
 */
float
SceneBoolean::floatValue() const
{
    const float f = (m_value ? 1.0 : 0.0);
    return f;
}

/**
 * @return The value as a integer data type.
 */
int32_t 
SceneBoolean::integerValue() const
{
    const float i = (m_value ? 1 : 0);
    return i;
}

/**
 * @return The value as a string data type.
 */
AString 
SceneBoolean::stringValue() const
{
    const AString s = (m_value ? "true" : "false");
    return s;
}

