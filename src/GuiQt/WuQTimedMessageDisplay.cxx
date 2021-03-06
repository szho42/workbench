
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

#include <QLabel>
#include <QLayout>
#include <QTimer>

#define __WU_Q_TIMED_MESSAGE_DISPLAY_DECLARE__
#include "WuQTimedMessageDisplay.h"
#undef __WU_Q_TIMED_MESSAGE_DISPLAY_DECLARE__

#include "AString.h"
#include "CaretAssert.h"

using namespace caret;


    
/**
 * \class caret::WuQTimedMessageDisplay 
 * \brief Timed message display.
 *
 * Displays a buttonless dialog for a period of time.
 *
 * Use the static show() method to display a timed message display.
 */

/**
 * Constructor.
 *
 * Display a message containing the given message for the given amount
 * of time.
 *
 * @param parent
 *    Parent on which message is displayed.
 * @param displayForSeconds
 *    Message is displayed for this amount of time, in milliseconds.
 * @param message
 *    Message that is displayed.
 */
WuQTimedMessageDisplay::WuQTimedMessageDisplay(QWidget* parent,
                                               const float displayForSeconds,
                                               const QString& message)
: QDialog(parent,
          Qt::FramelessWindowHint)
{
    CaretAssertMessage(displayForSeconds > 0.0,
                       "Display time must be greater than zero.");
    
    /*
     * Modal so it blocks until done.
     */
    setModal(true);
    
    /*
     * Delete self when done.
     */
    this->setAttribute(Qt::WA_DeleteOnClose,
                       true);

    /*
     * Put message in window.
     */
    QLabel* label = new QLabel(message);
    label->setFrameStyle(QFrame::Panel
                         | QFrame::Plain);
    label->setLineWidth(2);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(3, 3, 3, 3);
    layout->addWidget(label);
    
    /*
     * Setup a timer to call the accept(() slot when done.
     * accept() will close the dialog.
     */
    QTimer* timer = new QTimer(this);
    timer->setSingleShot(true);
    QObject::connect(timer, SIGNAL(timeout()),
                     this, SLOT(accept()));
    timer->start(displayForSeconds * 1000.0);
    
    /*
     * Display directly over the parent
     */
    QPoint pos = parent->mapToGlobal(parent->pos());
    move(pos);
}

/**
 * Display a message containing the given message for the given amount
 * of time.  This method will not return until the message window closes.
 *
 * @param parent
 *    Parent on which message is displayed.
 * @param displayForSeconds
 *    Message is displayed for this amount of time, in milliseconds.
 * @param message
 *    Message that is displayed.
 */
void
WuQTimedMessageDisplay::show(QWidget* parent,
                             const float displayForSeconds,
                             const QString& message)
{
    WuQTimedMessageDisplay* md = new WuQTimedMessageDisplay(parent,
                                                            displayForSeconds,
                                                            message);
    md->exec();
}

/**
 * Destructor.
 */
WuQTimedMessageDisplay::~WuQTimedMessageDisplay()
{
    
}

