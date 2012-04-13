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

#include <algorithm>
#include <cmath>

#include "BrainStructure.h"
#include "BrowserTabContent.h"
#include "BoundingBox.h"
#include "CaretAssert.h"
#include "EventManager.h"
#include "EventModelSurfaceGet.h"
#include "ModelSurface.h"

#include "Brain.h"
#include "BrainOpenGL.h"
#include "OverlaySet.h"
#include "Surface.h"

using namespace caret;

/**
 * Constructor.
 * @param surface - surface for this controller.
 *
 */
ModelSurface::ModelSurface(Brain* brain,
                                                             Surface* surface)
: Model(ModelTypeEnum::MODEL_TYPE_SURFACE,
                         YOKING_ALLOWED_YES,
                         ROTATION_ALLOWED_YES,
                         brain)
{
    CaretAssert(surface);
    this->initializeMembersModelSurface();
    this->surface = surface;
    for (int32_t i = 0; i < BrainConstants::MAXIMUM_NUMBER_OF_BROWSER_TABS; i++) {
        this->lateralView(i);
    }
    
    EventManager::get()->addEventListener(this, 
                                          EventTypeEnum::EVENT_MODEL_DISPLAY_CONTROLLER_SURFACE_GET);
}

/**
 * Destructor
 */
ModelSurface::~ModelSurface()
{
    EventManager::get()->removeAllEventsFromListener(this);
}

/**
 * Receive an event.
 * 
 * @param event
 *     The event that the receive can respond to.
 */
void 
ModelSurface::receiveEvent(Event* event)
{
    if (event->getEventType() == EventTypeEnum::EVENT_MODEL_DISPLAY_CONTROLLER_SURFACE_GET) {
        EventModelSurfaceGet* getSurfaceControllerEvent =
        dynamic_cast<EventModelSurfaceGet*>(event);
        CaretAssert(getSurfaceControllerEvent);
        
        
        /*
         * Looking this controller?
         */
        if (getSurfaceControllerEvent->getSurface() == this->getSurface()) {
            getSurfaceControllerEvent->setModelSurface(this);
            getSurfaceControllerEvent->setEventProcessed();
        }
    }
    else {     
        CaretAssertMessage(0, "Unexpected event: " + EventTypeEnum::toName(event->getEventType()));
    }
}

void
ModelSurface::initializeMembersModelSurface()
{
    this->surface = NULL;
}

/**
 * Get the surface in this controller.
 * @return  Surface in this controller.
 */
Surface*
ModelSurface::getSurface()
{
    return this->surface;
}

/**
 * Get the surface in this controller.
 * @return  Surface in this controller.
 */
const Surface*
ModelSurface::getSurface() const
{
    return this->surface;
}

/**
 * Get the name for use in a GUI.
 *
 * @param includeStructureFlag - Prefix label with structure to which
 *      this structure model belongs.
 * @return   Name for use in a GUI.
 *
 */
AString
ModelSurface::getNameForGUI(const bool includeStructureFlag) const
{
    AString name;
    if (includeStructureFlag) {
        const StructureEnum::Enum structure = this->surface->getStructure();
        name += StructureEnum::toGuiName(structure);
        name += " ";
    }
    name += this->surface->getFileNameNoPath();
    return name;
}

/**
 * @return The name that should be displayed in the tab
 * displaying this model controller.
 */
AString 
ModelSurface::getNameForBrowserTab() const
{
    const StructureEnum::Enum structure = this->surface->getStructure();
    const AString name = StructureEnum::toGuiName(structure);
    return name;
}

/**
 * Set the scaling so that the model fills the window.
 *
 */
void
ModelSurface::setDefaultScalingToFitWindow()
{
    BoundingBox bounds;
    this->surface->getBounds(bounds);
    
    float bigY = std::max(std::abs(bounds.getMinY()), bounds.getMaxY());
    float percentScreenY = BrainOpenGL::getModelViewingHalfWindowHeight() * 0.90f;
    float scale = percentScreenY / bigY;
    this->defaultModelScaling = scale;
    
    for (int32_t i = 0; i < BrainConstants::MAXIMUM_NUMBER_OF_BROWSER_TABS; i++) {
        this->setScaling(i, this->defaultModelScaling);
    }
}

/**
 * Reset view.  For left and right hemispheres, the default
 * view is a lateral view.
 * @param  windowTabNumber  Window for which view is requested
 * reset the view.
 */
void
ModelSurface::resetView(const int32_t windowTabNumber)
{
    Model::resetView(windowTabNumber);
    this->lateralView(windowTabNumber);    
}

/**
 * Switch to a lateral view.  This method only affects
 * surfaces that are the left or right cerebral cortex and
 * are not flat.
 *
 * @param  windowTabNumber  Window for which view is requested
 * reset the view.
 */
void
ModelSurface::lateralView(const int32_t windowTabNumber)
{
    if (this->surface->getSurfaceType() != SurfaceTypeEnum::FLAT) {
        switch (this->surface->getStructure()) {
            case StructureEnum::CORTEX_LEFT:
                this->leftView(windowTabNumber);
                break;
            case StructureEnum::CORTEX_RIGHT:
                this->rightView(windowTabNumber);
                break;
            default:
                break;
        }
    }
}

/**
 * Switch to a medial view.  This method only affects
 * surfaces that are the left or right cerebral cortex and
 * are not flat.
 *
 * @param  windowTabNumber  Window for which view is requested
 * reset the view.
 */
void
ModelSurface::medialView(const int32_t windowTabNumber)
{
    if (this->surface->getSurfaceType() != SurfaceTypeEnum::FLAT) {
        switch (this->surface->getStructure()) {
            case StructureEnum::CORTEX_LEFT:
                this->rightView(windowTabNumber);
                break;
            case StructureEnum::CORTEX_RIGHT:
                this->leftView(windowTabNumber);
                break;
            default:
                break;
        }
    }
}

/**
 * Get the overlay set for the given tab.
 * @param tabIndex
 *   Index of tab.
 * @return
 *   Overlay set at the given tab index.
 */
OverlaySet* 
ModelSurface::getOverlaySet(const int tabIndex)
{
    if (this->surface != NULL) {
        BrainStructure* brainStructure = this->surface->getBrainStructure();
        if (brainStructure != NULL) {
            return brainStructure->getOverlaySet(tabIndex);
        }
    }
    
    return NULL;
}

/**
 * Get the overlay set for the given tab.
 * @param tabIndex
 *   Index of tab.
 * @return
 *   Overlay set at the given tab index.
 */
const OverlaySet* 
ModelSurface::getOverlaySet(const int tabIndex) const
{
    if (this->surface != NULL) {
        const BrainStructure* brainStructure = this->surface->getBrainStructure();
        if (brainStructure != NULL) {
            return brainStructure->getOverlaySet(tabIndex);
        }
    }
    
    return NULL;
}

/**
 * Initilize the overlays for this controller.
 */
void 
ModelSurface::initializeOverlays()
{
}



