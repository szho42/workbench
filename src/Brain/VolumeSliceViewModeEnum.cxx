
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

#include <algorithm>
#define __VOLUME_SLICE_VIEW_MODE_ENUM_DECLARE__
#include "VolumeSliceViewModeEnum.h"
#undef __VOLUME_SLICE_VIEW_MODE_ENUM_DECLARE__

#include "CaretAssert.h"

using namespace caret;

    
/**
 * \class VolumeSliceViewModeEnum 
 * \brief Slice view mode enumerated type
 *
 * Slice view mode enumerated type
 */
/**
 * Constructor.
 *
 * @param enumValue
 *    An enumerated value.
 * @param name
 *    Name of enumerated value.
 *
 * @param guiName
 *    User-friendly name for use in user-interface.
 * @param guiNameAbbreviation
 *    Abbreviated User-friendly name for use in user-interface.
 */
VolumeSliceViewModeEnum::VolumeSliceViewModeEnum(const Enum enumValue,
                                                 const AString& name,
                                                 const AString& guiName,
                                                 const AString& guiNameAbbreviation)
{
    this->enumValue = enumValue;
    this->integerCode = integerCodeCounter++;
    this->name = name;
    this->guiName = guiName;
    this->guiNameAbbreviation = guiNameAbbreviation;
}

/**
 * Destructor.
 */
VolumeSliceViewModeEnum::~VolumeSliceViewModeEnum()
{
}

/**
 * Initialize the enumerated metadata.
 */
void
VolumeSliceViewModeEnum::initialize()
{
    if (initializedFlag) {
        return;
    }
    initializedFlag = true;

    enumData.push_back(VolumeSliceViewModeEnum(MONTAGE, 
                                               "MONTAGE", 
                                               "Montage",
                                               "M"));
    
    enumData.push_back(VolumeSliceViewModeEnum(OBLIQUE, 
                                               "OBLIQUE", 
                                               "Oblique",
                                               "O"));
    
    enumData.push_back(VolumeSliceViewModeEnum(ORTHOGONAL, 
                                               "ORTHOGONAL", 
                                               "Orthogonal",
                                               "S"));
}

/**
 * Find the data for and enumerated value.
 * @param enumValue
 *     The enumerated value.
 * @return Pointer to data for this enumerated type
 * or NULL if no data for type or if type is invalid.
 */
const VolumeSliceViewModeEnum*
VolumeSliceViewModeEnum::findData(const Enum enumValue)
{
    if (initializedFlag == false) initialize();

    size_t num = enumData.size();
    for (size_t i = 0; i < num; i++) {
        const VolumeSliceViewModeEnum* d = &enumData[i];
        if (d->enumValue == enumValue) {
            return d;
        }
    }

    return NULL;
}

/**
 * Get a string representation of the enumerated type.
 * @param enumValue 
 *     Enumerated value.
 * @return 
 *     String representing enumerated value.
 */
AString 
VolumeSliceViewModeEnum::toName(Enum enumValue) {
    if (initializedFlag == false) initialize();
    
    const VolumeSliceViewModeEnum* enumInstance = findData(enumValue);
    return enumInstance->name;
}

/**
 * Get an enumerated value corresponding to its name.
 * @param name 
 *     Name of enumerated value.
 * @param isValidOut 
 *     If not NULL, it is set indicating that a
 *     enum value exists for the input name.
 * @return 
 *     Enumerated value.
 */
VolumeSliceViewModeEnum::Enum 
VolumeSliceViewModeEnum::fromName(const AString& name, bool* isValidOut)
{
    if (initializedFlag == false) initialize();
    
    bool validFlag = false;
    Enum enumValue = ORTHOGONAL;
    
    for (std::vector<VolumeSliceViewModeEnum>::iterator iter = enumData.begin();
         iter != enumData.end();
         iter++) {
        const VolumeSliceViewModeEnum& d = *iter;
        if (d.name == name) {
            enumValue = d.enumValue;
            validFlag = true;
            break;
        }
    }
    
    if (isValidOut != 0) {
        *isValidOut = validFlag;
    }
    else if (validFlag == false) {
        CaretAssertMessage(0, AString("Name " + name + "failed to match enumerated value for type VolumeSliceViewModeEnum"));
    }
    return enumValue;
}

/**
 * Get a GUI string representation of the enumerated type.
 * @param enumValue 
 *     Enumerated value.
 * @return 
 *     String representing enumerated value.
 */
AString 
VolumeSliceViewModeEnum::toGuiName(Enum enumValue) {
    if (initializedFlag == false) initialize();
    
    const VolumeSliceViewModeEnum* enumInstance = findData(enumValue);
    return enumInstance->guiName;
}

/**
 * Get an enumerated value corresponding to its GUI name.
 * @param guiName
 *     Name of enumerated value.
 * @param isValidOut 
 *     If not NULL, it is set indicating that a
 *     enum value exists for the input name.
 * @return 
 *     Enumerated value.
 */
VolumeSliceViewModeEnum::Enum 
VolumeSliceViewModeEnum::fromGuiName(const AString& guiName, bool* isValidOut)
{
    if (initializedFlag == false) initialize();
    
    bool validFlag = false;
    Enum enumValue = ORTHOGONAL;
    
    for (std::vector<VolumeSliceViewModeEnum>::iterator iter = enumData.begin();
         iter != enumData.end();
         iter++) {
        const VolumeSliceViewModeEnum& d = *iter;
        if (d.guiName == guiName) {
            enumValue = d.enumValue;
            validFlag = true;
            break;
        }
    }
    
    if (isValidOut != 0) {
        *isValidOut = validFlag;
    }
    else if (validFlag == false) {
        CaretAssertMessage(0, AString("guiName " + guiName + "failed to match enumerated value for type VolumeSliceViewModeEnum"));
    }
    return enumValue;
}

/**
 * Get a GUI abbreviated string representation  of the enumerated type.
 * @param enumValue 
 *     Enumerated value.
 * @return 
 *     Short name representing enumerated value.
 */
AString 
VolumeSliceViewModeEnum::toGuiNameAbbreviation(Enum enumValue) {
    if (initializedFlag == false) initialize();
    
    const VolumeSliceViewModeEnum* enumInstance = findData(enumValue);
    return enumInstance->guiNameAbbreviation;
}

/**
 * Get an enumerated value corresponding to its abbreviated GUI name.
 * @param guiNameAbbreviation 
 *     Name of enumerated value.
 * @param isValidOut 
 *     If not NULL, it is set indicating that a
 *     enum value exists for the input name.
 * @return 
 *     Enumerated value.
 */
VolumeSliceViewModeEnum::Enum 
VolumeSliceViewModeEnum::fromGuiNameAbbreviation(const AString& guiNameAbbreviation, bool* isValidOut)
{
    if (initializedFlag == false) initialize();
    
    bool validFlag = false;
    Enum enumValue = ORTHOGONAL;
    
    for (std::vector<VolumeSliceViewModeEnum>::iterator iter = enumData.begin();
         iter != enumData.end();
         iter++) {
        const VolumeSliceViewModeEnum& d = *iter;
        if (d.guiNameAbbreviation == guiNameAbbreviation) {
            enumValue = d.enumValue;
            validFlag = true;
            break;
        }
    }
    
    if (isValidOut != 0) {
        *isValidOut = validFlag;
    }
    else if (validFlag == false) {
        CaretAssertMessage(0, AString("guiNameAbbreviation " + guiNameAbbreviation + "failed to match enumerated value for type VolumeSliceViewPlaneEnum"));
    }
    return enumValue;
}

/**
 * Get the integer code for a data type.
 *
 * @param enumValue 
 *     Enumerated value.
 * @return
 *    Integer code for data type.
 */
int32_t
VolumeSliceViewModeEnum::toIntegerCode(Enum enumValue)
{
    if (initializedFlag == false) initialize();
    const VolumeSliceViewModeEnum* enumInstance = findData(enumValue);
    return enumInstance->integerCode;
}

/**
 * Find the data type corresponding to an integer code.
 *
 * @param integerCode
 *     Integer code for enum.
 * @param isValidOut
 *     If not NULL, on exit isValidOut will indicate if
 *     integer code is valid.
 * @return
 *     Enum for integer code.
 */
VolumeSliceViewModeEnum::Enum
VolumeSliceViewModeEnum::fromIntegerCode(const int32_t integerCode, bool* isValidOut)
{
    if (initializedFlag == false) initialize();
    
    bool validFlag = false;
    Enum enumValue = ORTHOGONAL;
    
    for (std::vector<VolumeSliceViewModeEnum>::iterator iter = enumData.begin();
         iter != enumData.end();
         iter++) {
        const VolumeSliceViewModeEnum& enumInstance = *iter;
        if (enumInstance.integerCode == integerCode) {
            enumValue = enumInstance.enumValue;
            validFlag = true;
            break;
        }
    }
    
    if (isValidOut != 0) {
        *isValidOut = validFlag;
    }
    else if (validFlag == false) {
        CaretAssertMessage(0, AString("Integer code " + AString::number(integerCode) + "failed to match enumerated value for type VolumeSliceViewModeEnum"));
    }
    return enumValue;
}

/**
 * Get all of the enumerated type values.  The values can be used
 * as parameters to toXXX() methods to get associated metadata.
 *
 * @param allEnums
 *     A vector that is OUTPUT containing all of the enumerated values.
 */
void
VolumeSliceViewModeEnum::getAllEnums(std::vector<VolumeSliceViewModeEnum::Enum>& allEnums)
{
    if (initializedFlag == false) initialize();
    
    allEnums.clear();
    
    for (std::vector<VolumeSliceViewModeEnum>::iterator iter = enumData.begin();
         iter != enumData.end();
         iter++) {
        allEnums.push_back(iter->enumValue);
    }
}

/**
 * Get all of the names of the enumerated type values.
 *
 * @param allNames
 *     A vector that is OUTPUT containing all of the names of the enumerated values.
 * @param isSorted
 *     If true, the names are sorted in alphabetical order.
 */
void
VolumeSliceViewModeEnum::getAllNames(std::vector<AString>& allNames, const bool isSorted)
{
    if (initializedFlag == false) initialize();
    
    allNames.clear();
    
    for (std::vector<VolumeSliceViewModeEnum>::iterator iter = enumData.begin();
         iter != enumData.end();
         iter++) {
        allNames.push_back(VolumeSliceViewModeEnum::toName(iter->enumValue));
    }
    
    if (isSorted) {
        std::sort(allNames.begin(), allNames.end());
    }
}

/**
 * Get all of the GUI names of the enumerated type values.
 *
 * @param allGuiNames
 *     A vector that is OUTPUT containing all of the GUI names of the enumerated values.
 * @param isSorted
 *     If true, the names are sorted in alphabetical order.
 */
void
VolumeSliceViewModeEnum::getAllGuiNames(std::vector<AString>& allGuiNames, const bool isSorted)
{
    if (initializedFlag == false) initialize();
    
    allGuiNames.clear();
    
    for (std::vector<VolumeSliceViewModeEnum>::iterator iter = enumData.begin();
         iter != enumData.end();
         iter++) {
        allGuiNames.push_back(VolumeSliceViewModeEnum::toGuiName(iter->enumValue));
    }
    
    if (isSorted) {
        std::sort(allGuiNames.begin(), allGuiNames.end());
    }
}

