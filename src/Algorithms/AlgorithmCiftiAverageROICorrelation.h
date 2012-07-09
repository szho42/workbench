#ifndef __ALGORITHM_CIFTI_AVERAGE_ROI_CORRELATION_H__
#define __ALGORITHM_CIFTI_AVERAGE_ROI_CORRELATION_H__

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

#include "AbstractAlgorithm.h"
#include "StructureEnum.h"
#include <vector>

namespace caret {
    
    class CiftiInterface;
    class MetricFile;
    class VolumeFile;
    
    class AlgorithmCiftiAverageROICorrelation : public AbstractAlgorithm
    {
        AlgorithmCiftiAverageROICorrelation();
        void verifySurfaceComponent(const int& index, const CiftiInterface* myCifti, const StructureEnum::Enum& myStruct, const MetricFile* myRoi);
        void verifyVolumeComponent(const int& index, const CiftiInterface* myCifti, const VolumeFile* volROI);
        void processCifti(const CiftiInterface* myCifti, std::vector<float>& output, const MetricFile* leftROI, const MetricFile* rightROI, const MetricFile* cerebROI, const VolumeFile* volROI);
        void addSurface(const CiftiInterface* myCifti, StructureEnum::Enum myStruct, std::vector<double>& accum, int& count, const MetricFile* myRoi);
        void addVolume(const CiftiInterface* myCifti, std::vector<double>& accum, int& count, const VolumeFile* myRoi);
    protected:
        static float getSubAlgorithmWeight();
        static float getAlgorithmInternalWeight();
    public:
        AlgorithmCiftiAverageROICorrelation(ProgressObject* myProgObj, const std::vector<const CiftiInterface*>& ciftiList, CiftiFile* ciftiOut,
                                        const MetricFile* leftROI = NULL, const MetricFile* rightROI = NULL, const MetricFile* cerebROI = NULL, const VolumeFile* volROI = NULL);
        static OperationParameters* getParameters();
        static void useParameters(OperationParameters* myParams, ProgressObject* myProgObj);
        static AString getCommandSwitch();
        static AString getShortDescription();
    };

    typedef TemplateAutoOperation<AlgorithmCiftiAverageROICorrelation> AutoAlgorithmCiftiAverageROICorrelation;

}

#endif //__ALGORITHM_CIFTI_AVERAGE_ROI_CORRELATION_H__