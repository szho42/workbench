#ifndef __BRAIN_OPEN_G_L_WIDGET_CONTEXT_MENU__H_
#define __BRAIN_OPEN_G_L_WIDGET_CONTEXT_MENU__H_

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

#include <stdint.h>
#include <vector>

#include <QMenu>

#include "VolumeSliceViewPlaneEnum.h"
#include "VoxelIJK.h"

class QAction;

namespace caret {

    class Brain;
    class BrainOpenGLWidget;
    class BrowserTabContent;
    class CaretMappableDataFile;
    class CiftiConnectivityMatrixDataFileManager;
    class CiftiFiberTrajectoryManager;
    class ChartingDataManager;
    class SelectionManager;
    class LabelFile;
    class Surface;
    
    class BrainOpenGLWidgetContextMenu : public QMenu {
        
        Q_OBJECT

    public:
        BrainOpenGLWidgetContextMenu(SelectionManager* selectionManager,
                                     BrowserTabContent* browserTabContent,
                                     BrainOpenGLWidget* parentOpenGLWidget);
        
        virtual ~BrainOpenGLWidgetContextMenu();
        
    private slots:
        void createSurfaceFocusSelected();
        
        void createSurfaceIDSymbolFocusSelected();
        
        void createVolumeFocusSelected();
        
        void editSurfaceFocusSelected();
        
        void editVolumeFocusSelected();
        
        void removeAllNodeIdentificationSymbolsSelected();
        
        void removeNodeIdentificationSymbolSelected();
        
        void identifySurfaceBorderSelected();
        
        void identifySurfaceFocusSelected();
        
        void identifyVolumeFocusSelected();
        
        void identifySurfaceNodeSelected();

        void identifyVoxelSelected();

        void parcelCiftiFiberTrajectoryActionSelected(QAction* action);
        
        void parcelCiftiConnectivityActionSelected(QAction* action);

        void parcelChartableDataActionSelected(QAction* action);
        
        void borderCiftiConnectivitySelected();

//        void borderConnectivitySelected();
        
        void borderDataSeriesSelected();
        
    private:
        class ParcelConnectivity {
        public:
            enum ParcelType {
                PARCEL_TYPE_INVALID,
                PARCEL_TYPE_SURFACE_NODES,
                PARCEL_TYPE_VOLUME_VOXELS
            };
            
            ParcelConnectivity(Brain* brain,
                               const ParcelType parcelType,
                               CaretMappableDataFile* mappableLabelFile,
                               const int32_t labelFileMapIndex,
                               const int32_t labelKey,
                               const QString& labelName,
                               Surface* surface,
                               const int32_t nodeNumber,
                               const int64_t volumeDimensions[3],
                               ChartingDataManager* chartingDataManager,
                               CiftiConnectivityMatrixDataFileManager* ciftiConnectivityManager,
                               CiftiFiberTrajectoryManager* ciftiFiberTrajectoryManager);
            
            virtual ~ParcelConnectivity();
            
            void getNodeIndices(std::vector<int32_t>& nodeIndicesOut) const;
            
            void getVoxelIndices(std::vector<VoxelIJK>& voxelIndicesOut) const;
            
            Brain* brain;
            ParcelType parcelType;
            CaretMappableDataFile* mappableLabelFile;
            int32_t labelFileMapIndex;
            int32_t labelKey;
            QString labelName;
            Surface* surface;
            int32_t nodeNumber;
            int64_t volumeDimensions[3];
            CiftiConnectivityMatrixDataFileManager* ciftiConnectivityManager;
            ChartingDataManager* chartingDataManager;
            CiftiFiberTrajectoryManager* ciftiFiberTrajectoryManager;
        };
        
        BrainOpenGLWidgetContextMenu(const BrainOpenGLWidgetContextMenu&);

        BrainOpenGLWidgetContextMenu& operator=(const BrainOpenGLWidgetContextMenu&);
        
        bool warnIfNetworkBrainordinateCountIsLarge(const int64_t numberOfBrainordinatesInROI);
        
        bool enableDataSeriesGraphsIfNoneEnabled();
        
        void addIdentificationActions();
        
        void addBorderRegionOfInterestActions();
        
        void addFociActions();
        
        void addLabelRegionOfInterestActions();
        
        void addActionsToMenu(QList<QAction*>& actionsToAdd,
                              const bool addSeparatorBeforeActions);
        
        BrainOpenGLWidget* parentOpenGLWidget;

        std::vector<ParcelConnectivity*> parcelConnectivities;
        
        SelectionManager* selectionManager;
        
        BrowserTabContent* browserTabContent;
    };
    
#ifdef __BRAIN_OPEN_G_L_WIDGET_CONTEXT_MENU_DECLARE__
    // <PLACE DECLARATIONS OF STATIC MEMBERS HERE>
#endif // __BRAIN_OPEN_G_L_WIDGET_CONTEXT_MENU_DECLARE__

} // namespace
#endif  //__BRAIN_OPEN_G_L_WIDGET_CONTEXT_MENU__H_
