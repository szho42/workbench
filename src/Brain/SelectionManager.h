#ifndef __SELECTION_MANAGER__H_
#define __SELECTION_MANAGER__H_

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
#include "EventListenerInterface.h"

namespace caret {
    class Brain;
    class BrowserTabContent;
    class SelectionItem;
    class SelectionItemBorderSurface;
    class SelectionItemChartDataSeries;
    class SelectionItemChartMatrix;
    class SelectionItemChartTimeSeries;
    class SelectionItemFocusSurface;
    class SelectionItemFocusVolume;
    class SelectionItemSurfaceNode;
    class SelectionItemSurfaceNodeIdentificationSymbol;
    class SelectionItemSurfaceTriangle;
    class SelectionItemVoxel;
    class IdentificationTextGenerator;
    class Surface;
    
    class SelectionManager : public CaretObject, public EventListenerInterface {
        
    public:
        SelectionManager();
        
        virtual ~SelectionManager();
        
        void receiveEvent(Event* event);
        
        void reset();
        
        SelectionItemBorderSurface* getSurfaceBorderIdentification();
        
        const SelectionItemBorderSurface* getSurfaceBorderIdentification() const;
        
        SelectionItemFocusSurface* getSurfaceFocusIdentification();
        
        const SelectionItemFocusSurface* getSurfaceFocusIdentification() const;
        
        SelectionItemFocusVolume* getVolumeFocusIdentification();
        
        const SelectionItemFocusVolume* getVolumeFocusIdentification() const;
        
        SelectionItemSurfaceNode* getSurfaceNodeIdentification();
        
        const SelectionItemSurfaceNode* getSurfaceNodeIdentification() const;
        
        SelectionItemSurfaceNodeIdentificationSymbol* getSurfaceNodeIdentificationSymbol();
        
        const SelectionItemSurfaceNodeIdentificationSymbol* getSurfaceNodeIdentificationSymbol() const;
        
        SelectionItemSurfaceTriangle* getSurfaceTriangleIdentification();
        
        const SelectionItemSurfaceTriangle* getSurfaceTriangleIdentification() const;
        
        const SelectionItemVoxel* getVoxelIdentification() const;
        
        SelectionItemVoxel* getVoxelIdentification();
        
        SelectionItemChartDataSeries* getChartDataSeriesIdentification();
        
        const SelectionItemChartDataSeries* getChartDataSeriesIdentification() const;
        
        SelectionItemChartMatrix* getChartMatrixIdentification();
        
        const SelectionItemChartMatrix* getChartMatrixIdentification() const;
        
        SelectionItemChartTimeSeries* getChartTimeSeriesIdentification();
        
        const SelectionItemChartTimeSeries* getChartTimeSeriesIdentification() const;
        
        AString getIdentificationText(const Brain* brain) const;
        
        void filterSelections(const bool applySelectionBackgroundFiltering);
        
        void clearDistantSelections();
        
        void clearOtherSelectedItems(SelectionItem* selectedItem);
        
        const SelectionItem* getLastSelectedItem() const;
        
        void setLastSelectedItem(const SelectionItem* lastItem);
        
    private:
        SelectionManager(const SelectionManager&);

        SelectionManager& operator=(const SelectionManager&);
        
    public:
        virtual AString toString() const;
        
    private:
        SelectionItem* getMinimumDepthFromMultipleSelections(std::vector<SelectionItem*> items) const;

        /** ALL items */
        std::vector<SelectionItem*> m_allSelectionItems;
        
        /** Layered items (foci, borders, etc.) */
        std::vector<SelectionItem*> m_layeredSelectedItems;
        
        /** Surface items (nodes, triangles) */
        std::vector<SelectionItem*> m_surfaceSelectedItems;
        
        /** Volume items */
        std::vector<SelectionItem*> m_volumeSelectedItems;
        
        SelectionItemBorderSurface* m_surfaceBorderIdentification;
        
        SelectionItemChartDataSeries* m_chartDataSeriesIdentification;
        
        SelectionItemChartMatrix* m_chartMatrixIdentification;
        
        SelectionItemChartTimeSeries* m_chartTimeSeriesIdentification;
        
        SelectionItemFocusSurface* m_surfaceFocusIdentification;
        
        SelectionItemFocusVolume* m_volumeFocusIdentification;
        
        SelectionItemSurfaceNode* m_surfaceNodeIdentification;
        
        SelectionItemSurfaceNodeIdentificationSymbol* m_surfaceNodeIdentificationSymbol;
        
        SelectionItemSurfaceTriangle* m_surfaceTriangleIdentification;
        
        IdentificationTextGenerator* m_idTextGenerator;
        
        SelectionItemVoxel* m_voxelIdentification;
        
        /** Last selected item DOES NOT GET PUT IN m_allSelectionItems */
        SelectionItem* m_lastSelectedItem;
    };
    
#ifdef __SELECTION_MANAGER_DECLARE__
    // <PLACE DECLARATIONS OF STATIC MEMBERS HERE>
#endif // __SELECTION_MANAGER_DECLARE__

} // namespace
#endif  //__SELECTION_MANAGER__H_
