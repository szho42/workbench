
#ifndef __OVERLAY_SELECTION_CONTROL_H__
#define __OVERLAY_SELECTION_CONTROL_H__

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
#include <QVector>
#include <QWidget>
#include <stdint.h>

class QPushButton;

namespace caret {

    class BrowserTabContent;
    class OverlaySelectionControlLayer;
    class WuQWidgetObjectGroup;

    class OverlaySelectionControl : public QWidget {
        
        Q_OBJECT
        
    public:        
        OverlaySelectionControl(const int32_t browserWindowIndex,
                                const Qt::Orientation orientation,
                                QWidget* parent = 0);
        
        ~OverlaySelectionControl();
       
        void updateControl();
        
    signals:
        void layersChanged();
        
        /** Emitted when a control is removed and container should shrink */
        void controlRemoved();
        
    private slots:
        void addLayer();

    private:
        OverlaySelectionControl(const OverlaySelectionControl&);
        OverlaySelectionControl& operator=(const OverlaySelectionControl&);
        
        QWidget* createLayers();
        
        void updateLayersGridWidgetSize();

        int32_t browserWindowIndex;

        Qt::Orientation orientation;

        QWidget* layersGridWidget;
        
        QVector<OverlaySelectionControlLayer*> layers;
        
        QPushButton* addLayerPushButton;
        
    
        friend class OverlaySelectionControlLayer;
    };
    
}
#endif // __OVERLAY_SELECTION_CONTROL_H__