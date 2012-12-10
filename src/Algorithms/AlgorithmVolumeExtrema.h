#ifndef __ALGORITHM_VOLUME_EXTREMA_H__
#define __ALGORITHM_VOLUME_EXTREMA_H__

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

/*
file->save as... and enter what you will name the class, plus .h

find and replace these strings, without matching "whole word only" (plain text mode):

AlgorithmVolumeExtrema     : algorithm name, in CamelCase, with initial capital, same as what you saved the header file to
ALGORITHM_VOLUME_EXTREMA    : uppercase of algorithm name, with underscore between words, used in #ifdef guards
-volume-extrema   : switch for the command line to use, often hyphenated version of algorithm name, lowercase, minus "algorithm"
FIND EXTREMA IN A VOLUME FILE : short description of the command, uppercase, three to five words, often just command switch with more verbosity

next, make AlgorithmVolumeExtrema.cxx from AlgorithmTemplate.cxx.txt via one of the following (depending on working directory):

cat AlgorithmTemplate.cxx.txt | sed 's/[A]lgorithmName/AlgorithmVolumeExtrema/g' | sed 's/-[c]ommand-switch/-volume-extrema/g' | sed 's/[S]HORT DESCRIPTION/FIND EXTREMA IN A VOLUME FILE/g' > AlgorithmVolumeExtrema.cxx
cat Algorithms/AlgorithmTemplate.cxx.txt | sed 's/[A]lgorithmName/AlgorithmVolumeExtrema/g' | sed 's/-[c]ommand-switch/-volume-extrema/g' | sed 's/[S]HORT DESCRIPTION/FIND EXTREMA IN A VOLUME FILE/g' > Algorithms/AlgorithmVolumeExtrema.cxx
cat src/Algorithms/AlgorithmTemplate.cxx.txt | sed 's/[A]lgorithmName/AlgorithmVolumeExtrema/g' | sed 's/-[c]ommand-switch/-volume-extrema/g' | sed 's/[S]HORT DESCRIPTION/FIND EXTREMA IN A VOLUME FILE/g' > src/Algorithms/AlgorithmVolumeExtrema.cxx

or manually copy and replace

next, implement its functions - the algorithm work goes in the CONSTRUCTOR

add these into Algorithms/CMakeLists.txt:

AlgorithmVolumeExtrema.h
AlgorithmVolumeExtrema.cxx

place the following lines into Commands/CommandOperationManager.cxx:

#include "AlgorithmVolumeExtrema.h"
    //near the top

    this->commandOperations.push_back(new CommandParser(new AutoAlgorithmVolumeExtrema()));
        //in CommandOperationManager()

finally, remove this block comment
*/

#include "AbstractAlgorithm.h"
#include "Vector3D.h"
#include <vector>

namespace caret {
    
    class VolumeFile;
    
    class AlgorithmVolumeExtrema : public AbstractAlgorithm
    {
        struct VoxelIJK
        {
            int64_t m_ijk[3];//so i can put this in a vector
            VoxelIJK() { }
            VoxelIJK(int64_t i, int64_t j, int64_t k) { m_ijk[0] = i; m_ijk[1] = j; m_ijk[2] = k; }
        };
        std::vector<VoxelIJK> m_stencil;
        int64_t m_irange, m_jrange, m_krange;
        AlgorithmVolumeExtrema();
        void precomputeStencil(const VolumeFile* myVolIn, const float& distance);
        void findExtremaConsolidate(const VolumeFile* toProcess, const int& s, const int& c, const VolumeFile* myRoi, const float& distance, const bool& threshMode, const float& lowThresh, const float& highThresh, std::vector<VoxelIJK>& minima, std::vector<VoxelIJK>& maxima);
        void findExtremaStencils(const VolumeFile* toProcess, const int& s, const int& c, const VolumeFile* myRoi, const bool& threshMode, const float& lowThresh, const float& highThresh, std::vector<VoxelIJK>& minima, std::vector<VoxelIJK>& maxima);
        void consolidateStep(const VolumeFile* toProcess, const float& distance, std::vector<std::pair<Vector3D, int> > tempExtrema[2], std::vector<VoxelIJK>& minima, std::vector<VoxelIJK>& maxima);
    protected:
        static float getSubAlgorithmWeight();
        static float getAlgorithmInternalWeight();
    public:
        AlgorithmVolumeExtrema(ProgressObject* myProgObj, const VolumeFile* myVolIn, const float& distance, VolumeFile* myVolOut, const float& lowThresh,
                               const float& highThresh, const VolumeFile* myRoi = NULL, const float& presmooth = -1.0f, const bool& sumSubvols = false,
                               const bool& consolidateMode = false, const int& subvol = -1);
        AlgorithmVolumeExtrema(ProgressObject* myProgObj, const VolumeFile* myVolIn, const float& distance, VolumeFile* myVolOut,
                               const VolumeFile* myRoi = NULL, const float& presmooth = -1.0f, const bool& sumSubvols = false,
                               const bool& consolidateMode = false, const int& subvol = -1);
        static OperationParameters* getParameters();
        static void useParameters(OperationParameters* myParams, ProgressObject* myProgObj);
        static AString getCommandSwitch();
        static AString getShortDescription();
    };

    typedef TemplateAutoOperation<AlgorithmVolumeExtrema> AutoAlgorithmVolumeExtrema;

}

#endif //__ALGORITHM_VOLUME_EXTREMA_H__