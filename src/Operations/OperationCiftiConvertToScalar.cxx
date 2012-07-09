/*LICENSE_START*/
/*
 *  Copyright 1995-2002 Washington University School of Medicine
 *
 *  http://brainmap.wustl.edu
 *
 *  This file is part of CARET.
 *
 *  CARET is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  CARET is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with CARET; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "CaretLogger.h"
#include "CiftiFile.h"
#include "FileInformation.h"
#include "OperationCiftiConvertToScalar.h"
#include "OperationException.h"

#include <fstream>
#include <string>

using namespace caret;
using namespace std;

AString OperationCiftiConvertToScalar::getCommandSwitch()
{
    return "-cifti-convert-to-scalar";
}

AString OperationCiftiConvertToScalar::getShortDescription()
{
    return "CHANGE A CIFTI DIMENSION TO NAMED SCALAR MAPS";
}

OperationParameters* OperationCiftiConvertToScalar::getParameters()
{
    OperationParameters* ret = new OperationParameters();
    ret->addCiftiParameter(1, "cifti-in", "input cifti file");
    ret->addStringParameter(2, "direction", "which mapping to change to scalar maps, ROW or COLUMN");
    ret->addCiftiOutputParameter(3, "cifti-out", "output cifti file, must not be the same as input");
    OptionalParameter* nameFileOpt = ret->createOptionalParameter(4, "-name-file", "specify names for the maps");
    nameFileOpt->addStringParameter(1, "file", "text file containing map names, one per line");
    ret->setHelpText(
        AString("Creates a new cifti file with the same data as the input, but with one of the dimensions set to contain strings identifying each map.  ") +
        "DO NOT try to overwrite the existing cifti with the output with this command, it may read/write rows from/to disk on an as-needed basis.  " +
        "Specifying ROW means each row will contain one value from each scalar map.  This is the timepoints direction in dtseries."
    );
    return ret;
}

void OperationCiftiConvertToScalar::useParameters(OperationParameters* myParams, ProgressObject* myProgObj)
{
    LevelProgress myProgress(myProgObj);
    CiftiFile* ciftiIn = myParams->getCifti(1);
    AString direction = myParams->getString(2);
    int mydir = -1;
    if (direction == "ROW") mydir = 0;
    if (direction == "COLUMN") mydir = 1;
    if (mydir == -1)
    {
        throw OperationException("<direction> must be 'ROW' or 'COLUMN'");
    }
    CiftiFile* ciftiOut = myParams->getOutputCifti(3);
    OptionalParameter* nameFileOpt = myParams->getOptionalParameter(4);
    CiftiXML myXML = ciftiIn->getCiftiXML();
    int rowSize = myXML.getNumberOfColumns(), colSize = myXML.getNumberOfRows();
    if (mydir == 0)
    {
        myXML.resetRowsToScalars(rowSize);
        if (nameFileOpt->m_present)
        {
            AString listfileName = nameFileOpt->getString(1);
            FileInformation textFileInfo(listfileName);
            if (!textFileInfo.exists())
            {
                throw OperationException("name list file doesn't exist");
            }
            fstream nameListFile(listfileName.toLocal8Bit().constData(), fstream::in);
            if (!nameListFile.good())
            {
                throw OperationException("error reading name list file");
            }
            string mapName;
            for (int i = 0; i < rowSize; ++i)
            {
                getline(nameListFile, mapName);
                if (!nameListFile)//no, seriously, that is how you check if your input was good
                {
                    CaretLogWarning("warning, name file contained " + AString::number(i) + " names, expected " + AString::number(rowSize));
                    break;
                }
                myXML.setMapNameForRowIndex(i, mapName.c_str());
            }
        }
    } else {
        myXML.resetColumnsToScalars(colSize);
        if (nameFileOpt->m_present)
        {
            AString listfileName = nameFileOpt->getString(1);
            FileInformation textFileInfo(listfileName);
            if (!textFileInfo.exists())
            {
                throw OperationException("name list file doesn't exist");
            }
            fstream nameListFile(listfileName.toLocal8Bit().constData(), fstream::in);
            if (!nameListFile.good())
            {
                throw OperationException("error reading name list file");
            }
            string mapName;
            for (int i = 0; i < colSize; ++i)
            {
                getline(nameListFile, mapName);
                if (!nameListFile)//no, seriously, that is how you check if your input was good
                {
                    CaretLogWarning("warning, name file contained " + AString::number(i) + " names, expected " + AString::number(colSize));
                    break;
                }
                myXML.setMapNameForColumnIndex(i, mapName.c_str());
            }
        }
    }
    ciftiOut->setCiftiXML(myXML);
    vector<float> myrow(rowSize);
    for (int i = 0; i < colSize; ++i)
    {
        ciftiIn->getRow(myrow.data(), i);
        ciftiOut->setRow(myrow.data(), i);
    }
}