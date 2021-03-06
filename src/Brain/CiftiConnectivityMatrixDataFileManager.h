#ifndef __CIFTI_CONNECTIVITY_MATRIX_DATA_FILE_MANAGER_H__
#define __CIFTI_CONNECTIVITY_MATRIX_DATA_FILE_MANAGER_H__

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


#include "CaretObject.h"
#include "DataFileException.h"
#include "VoxelIJK.h"

namespace caret {

    class Brain;
    class CiftiConnectivityMatrixParcelFile;
    class SurfaceFile;
    
    class CiftiConnectivityMatrixDataFileManager
    : public CaretObject
    {
        
    public:
        CiftiConnectivityMatrixDataFileManager();
        
        virtual ~CiftiConnectivityMatrixDataFileManager();
        
        bool loadDataForSurfaceNode(Brain* brain,
                                    const SurfaceFile* surfaceFile,
                                    const int32_t nodeIndex,
                                    std::vector<AString>& rowColumnInformationOut) throw (DataFileException);
        
        bool loadAverageDataForSurfaceNodes(Brain* brain,
                                            const SurfaceFile* surfaceFile,
                                            const std::vector<int32_t>& nodeIndices) throw (DataFileException);
        
        bool loadDataForVoxelAtCoordinate(Brain* brain,
                                          const float xyz[3],
                                          std::vector<AString>& rowColumnInformationOut) throw (DataFileException);
        
        bool loadAverageDataForVoxelIndices(Brain* brain,
                                            const int64_t volumeDimensionIJK[3],
                                            const std::vector<VoxelIJK>& voxelIndices) throw (DataFileException);

        bool loadRowFromParcelFile(Brain* brain,
                                   CiftiConnectivityMatrixParcelFile* parcelFile,
                                   const int32_t rowIndex,
                                   std::vector<AString>& rowColumnInformationOut) throw (DataFileException);
        
        bool hasNetworkFiles(Brain* brain) const;
        
    private:
        CiftiConnectivityMatrixDataFileManager(const CiftiConnectivityMatrixDataFileManager&);

        CiftiConnectivityMatrixDataFileManager& operator=(const CiftiConnectivityMatrixDataFileManager&);
        
    public:

        // ADD_NEW_METHODS_HERE

    private:

        // ADD_NEW_MEMBERS_HERE
    };
    
#ifdef __CIFTI_CONNECTIVITY_MATRIX_DATA_FILE_MANAGER_DECLARE__
    // <PLACE DECLARATIONS OF STATIC MEMBERS HERE>
#endif // __CIFTI_CONNECTIVITY_MATRIX_DATA_FILE_MANAGER_DECLARE__

} // namespace
#endif  //__CIFTI_CONNECTIVITY_MATRIX_DATA_FILE_MANAGER_H__
