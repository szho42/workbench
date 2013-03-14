#ifndef __USER_INPUT_MODE_VIEW__H_
#define __USER_INPUT_MODE_VIEW__H_

/*LICENSE_START*/
/* 
 *  Copyright 1995-2011 Washington University School of Medicine 
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


#include "CaretObject.h"
#include "UserInputReceiverInterface.h"

namespace caret {

    class UserInputModeView : public CaretObject, public UserInputReceiverInterface {
        
    public:
        UserInputModeView();
        
        virtual ~UserInputModeView();
        
        virtual void processMouseEvent(MouseEvent* mouseEvent,
                               BrainOpenGLViewportContent* viewportContent,
                               BrainOpenGLWidget* openGLWidget);
        
        virtual UserInputMode getUserInputMode() const;
        
        virtual void initialize();
        
        virtual void finish();

        virtual CursorEnum::Enum getCursor() const;
        
        virtual QWidget* getWidgetForToolBar();
        
        static void processModelViewTransformation(MouseEvent* mouseEvent,
                                                   BrainOpenGLViewportContent* viewportContent,
                                                   BrainOpenGLWidget* openGLWidget,
                                                   const int32_t mousePressedX,
                                                   const int32_t mousePressedY);
        
        static void processModelViewIdentification(BrainOpenGLViewportContent* viewportContent,
                                                   BrainOpenGLWidget* openGLWidget,
                                                   const int32_t mouseClickX,
                                                   const int32_t mouseClickY);
        
        virtual void mouseLeftClick(const MouseEvent& mouseEvent);
        
        virtual void mouseLeftClickWithShift(const MouseEvent& mouseEvent);
        
        virtual void mouseLeftDrag(const MouseEvent& mouseEvent);
        
        virtual void mouseLeftDragWithAlt(const MouseEvent& mouseEvent);
        
        virtual void mouseLeftDragWithCtrl(const MouseEvent& mouseEvent);
        
        virtual void mouseLeftDragWithShift(const MouseEvent& mouseEvent);

    private:
        UserInputModeView(const UserInputModeView&);

        UserInputModeView& operator=(const UserInputModeView&);
        
        void processIdentification(MouseEvent* mouseEvent,
                                   BrainOpenGLViewportContent* viewportContent,
                                   BrainOpenGLWidget* openGLWidget);
        
    public:
        virtual AString toString() const;
        
    private:
        int32_t mousePressX;
        int32_t mousePressY;
    };
    
#ifdef __USER_INPUT_MODE_VIEW_DECLARE__
    // <PLACE DECLARATIONS OF STATIC MEMBERS HERE>
#endif // __USER_INPUT_MODE_VIEW_DECLARE__

} // namespace
#endif  //__USER_INPUT_MODE_VIEW__H_
