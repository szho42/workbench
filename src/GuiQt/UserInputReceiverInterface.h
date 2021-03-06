#ifndef __USER_INPUT_RECEIVER_INTERFACE__H_
#define __USER_INPUT_RECEIVER_INTERFACE__H_

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

#include "CursorEnum.h"

class QWidget;

namespace caret {

    class BrainOpenGLViewportContent;
    class BrainOpenGLWidget;
    class MouseEvent;
    
    /**
     * \class caret::UserInputReceiverInterface
     * \brief Provides an interface for user input events
     *
     * Classes implementing this interface receive
     * user input events from the OpenGL graphics
     * region of a BrowserWindow containing brain
     * models.
     */
    class UserInputReceiverInterface {
        
    public:
        /** Enumerated type for input modes */
        enum UserInputMode {
            /** Invalid */
            INVALID,
            /** Border Operations */
            BORDERS,
            /** Foci Operations */
            FOCI,
            /** Viewing Operations */
            VIEW
        };
        
        /**
         * @return The input mode enumerated type.
         */
        virtual UserInputMode getUserInputMode() const = 0;
        
        /**
         * Called when 'this' user input receiver is set
         * to receive events.
         */
        virtual void initialize() = 0;
        
        /**
         * Called when 'this' user input receiver is no
         * longer set to receive events.
         */
        virtual void finish() = 0;
        
        /**
         * @return A widget for display at the bottom of the
         * Browser Window Toolbar when this mode is active.
         * If no user-interface controls are needed, return NULL.
         * The toolbar will take ownership of the widget and
         * delete it so derived class MUST NOT delete the widget.
         */
        virtual QWidget* getWidgetForToolBar() = 0;
        
        /**
         * @return The cursor for display in the OpenGL widget.
         */
        virtual CursorEnum::Enum getCursor() const = 0;
        
        /**
         * Process a mouse left click event.
         *
         * @param mouseEvent
         *     Mouse event information.
         */
        virtual void mouseLeftClick(const MouseEvent& /*mouseEvent*/) { }
        
        /**
         * Process a mouse left click with shift key down event.
         *
         * @param mouseEvent
         *     Mouse event information.
         */
        virtual void mouseLeftClickWithShift(const MouseEvent& /*mouseEvent*/) { }
        
        /**
         * Process a mouse left drag with no keys down event.
         *
         * @param mouseEvent
         *     Mouse event information.
         */
        virtual void mouseLeftDrag(const MouseEvent& /*mouseEvent*/) { }
        
        /**
         * Process a mouse left drag with only the alt key down event.
         *
         * @param mouseEvent
         *     Mouse event information.
         */
        virtual void mouseLeftDragWithAlt(const MouseEvent& /*mouseEvent*/) { }
        
        /**
         * Process a mouse left drag with ctrl key down event.
         *
         * @param mouseEvent
         *     Mouse event information.
         */
        virtual void mouseLeftDragWithCtrl(const MouseEvent& /*mouseEvent*/) { }
        
        /**
         * Process a mouse left drag with shift key down event.
         *
         * @param mouseEvent
         *     Mouse event information.
         */
        virtual void mouseLeftDragWithShift(const MouseEvent& /*mouseEvent*/) { }
        
        
    protected:
        UserInputReceiverInterface() { }
        
        virtual ~UserInputReceiverInterface() { }
        
    private:
        UserInputReceiverInterface(const UserInputReceiverInterface&) { }

        UserInputReceiverInterface& operator=(const UserInputReceiverInterface& uiri) {
            if (this != &uiri) {
            }
            return *this;
        }
        
    };
    
#ifdef __USER_INPUT_RECEIVER_INTERFACE_DECLARE__
    // <PLACE DECLARATIONS OF STATIC MEMBERS HERE>
#endif // __USER_INPUT_RECEIVER_INTERFACE_DECLARE__

} // namespace
#endif  //__USER_INPUT_RECEIVER_INTERFACE__H_
