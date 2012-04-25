
/*LICENSE_START*/
/*
 * Copyright 2012 Washington University,
 * All rights reserved.
 *
 * Connectome DB and Connectome Workbench are part of the integrated Connectome 
 * Informatics Platform.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the names of Washington University nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR  
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */
/*LICENSE_END*/

#include <cmath>
#include <limits>

#define __CONNECTIVITY_TIME_SERIES_VIEW_CONTROLLER_DECLARE__
#include "ConnectivityTimeSeriesViewController.h"
#undef __CONNECTIVITY_TIME_SERIES_VIEW_CONTROLLER_DECLARE__

#include <QAction>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QToolButton>
#include <QSpinBox>

#include "Brain.h"
#include "CaretLogger.h"
#include "CaretPreferences.h"
#include "ConnectivityLoaderFile.h"
#include "ConnectivityLoaderManager.h"
#include "ElapsedTimer.h"
#include "EventManager.h"
#include "EventGraphicsUpdateAllWindows.h"
#include "EventSurfaceColoringInvalidate.h"
#include "EventUserInterfaceUpdate.h"
#include "EventUpdateAnimationStartTime.h"
#include "GuiManager.h"
#include "SessionManager.h"
#include "TimeCourseDialog.h"
#include "TimeSeriesManagerForViewController.h"
#include "WuQGridLayoutGroup.h"
#include "WuQtUtilities.h"

using namespace caret;


    
/**
 * \class caret::ConnectivityTimeSeriesViewController 
 * \brief View-Controller for one connectivity loader
 *
 */
/**
 * Constructor.
 */
ConnectivityTimeSeriesViewController::ConnectivityTimeSeriesViewController(const Qt::Orientation orientation,
                                                                   QGridLayout* gridLayout,
                                                                   QObject* parent)
: QObject(parent)
{
    this->connectivityLoaderFile = NULL;
    this->previousConnectivityLoaderFile = NULL;
    
    this->enabledCheckBox = new QCheckBox(" ");
    QObject::connect(this->enabledCheckBox, SIGNAL(stateChanged(int)),
                     this, SLOT(enabledCheckBoxStateChanged(int)));
    
    this->fileNameLineEdit = new QLineEdit("                 ");
    this->fileNameLineEdit->setReadOnly(true);
    
    this->animateAction = WuQtUtilities::createAction("Animate",
                                                      "Start/stop animation",
                                                      this,
                                                      this,
                                                      SLOT(animateActionTriggered(bool)));
    this->animateAction->setCheckable(true);
    QToolButton* animateToolButton = new QToolButton();
    animateToolButton->setDefaultAction(this->animateAction);
    
    QIcon graphIcon;
    const bool graphIconValid = WuQtUtilities::loadIcon(":/time_series_graph.png",
                                                           graphIcon);
    this->graphDisplayAction = WuQtUtilities::createAction("Graph",
                                                           "Display time-series graph for selected brainordinate", 
                                                           this, 
                                                           this, 
                                                           SLOT(graphDisplayActionTriggered(bool)));
    if (graphIconValid) {
        this->graphDisplayAction->setIcon(graphIcon);
    }
    this->graphDisplayAction->setCheckable(true);
    QToolButton* graphToolButton = new QToolButton();
    graphToolButton->setDefaultAction(this->graphDisplayAction);
    
    this->timeSpinBox = new QDoubleSpinBox();
    this->timeSpinBox->setMinimum(0);
    this->timeSpinBox->setMaximum(std::numeric_limits<double>::max());
    this->timeSpinBox->setSingleStep(1.0);
    this->timeSpinBox->setMaximumWidth(80);
    WuQtUtilities::setToolTipAndStatusTip(this->timeSpinBox,
                                          "Select timepoint for display on brainordinates");
    QObject::connect(this->timeSpinBox, SIGNAL(valueChanged(double)),
                     this, SLOT(timeSpinBoxValueChanged(double)));
    
    this->gridLayoutGroup = new WuQGridLayoutGroup(gridLayout,
                                                   this);
    if (orientation == Qt::Horizontal) {
        int row = this->gridLayoutGroup->rowCount();
        this->gridLayoutGroup->addWidget(this->enabledCheckBox, row, 0);
        this->gridLayoutGroup->addWidget(graphToolButton, row, 1);
        this->gridLayoutGroup->addWidget(animateToolButton, row, 2);
        this->gridLayoutGroup->addWidget(this->timeSpinBox, row, 3);
        this->gridLayoutGroup->addWidget(this->fileNameLineEdit, row, 4);
    }
    else {
        QFrame* bottomHorizontalLineWidget = new QFrame();
        bottomHorizontalLineWidget->setLineWidth(0);
        bottomHorizontalLineWidget->setMidLineWidth(1);
        bottomHorizontalLineWidget->setFrameStyle(QFrame::HLine | QFrame::Raised);
        
        int row = this->gridLayoutGroup->rowCount();
        this->gridLayoutGroup->addWidget(this->enabledCheckBox, row, 0, 2, 1, Qt::AlignCenter);
        this->gridLayoutGroup->addWidget(this->fileNameLineEdit, row, 1, 1, 3);
        row++;
        this->gridLayoutGroup->addWidget(graphToolButton, row, 1, Qt::AlignCenter);
        this->gridLayoutGroup->addWidget(animateToolButton, row, 2);
        this->gridLayoutGroup->addWidget(this->timeSpinBox, row, 3, Qt::AlignLeft);
        row++;
        this->gridLayoutGroup->addWidget(bottomHorizontalLineWidget, row, 0, 1, -1);
    }
    
    this->animator = NULL;
    
    double time = 0.0f;
    CaretPreferences *prefs = SessionManager::get()->getCaretPreferences();
    prefs->getAnimationStartTime(time);
    this->setAnimationStartTime(time);
    EventManager::get()->addEventListener(this, EventTypeEnum::EVENT_UPDATE_ANIMATION_START_TIME);

    
    allConnectivityTimeSeriesViewControllers.insert(this);
}

/**
 * Destructor.
 */
ConnectivityTimeSeriesViewController::~ConnectivityTimeSeriesViewController()
{
    this->deleteAnimator();
    
    allConnectivityTimeSeriesViewControllers.erase(this);
    EventManager::get()->removeAllEventsFromListener(this);
}

/**
 * Delete the animator.
 */
void 
ConnectivityTimeSeriesViewController::deleteAnimator()
{
    if (this->animator != NULL) {
        this->animator->stop();
        delete this->animator;
        this->animator = NULL;
    }
}

/**
 * @return The connectivity loader file in this view controller.
 * NULL if not valid.
 */
ConnectivityLoaderFile* 
ConnectivityTimeSeriesViewController::getConnectivityLoaderFile()
{
    return this->connectivityLoaderFile;
}

/**
 * Create the grid layout for this view controller using the given orientation.
 * @param orientation
 *    Orientation in toolbox.
 * @return
 *    GridLayout setup for this view controller.
 */
QGridLayout* 
ConnectivityTimeSeriesViewController::createGridLayout(const Qt::Orientation orientation)
{
    QGridLayout* gridLayout = new QGridLayout();
    WuQtUtilities::setLayoutMargins(gridLayout, 2, 2);
    if (orientation == Qt::Horizontal) {
        gridLayout->setColumnStretch(0, 0);
        gridLayout->setColumnStretch(1, 0);
        gridLayout->setColumnStretch(2, 0);
        gridLayout->setColumnStretch(3, 0);
        gridLayout->setColumnStretch(4, 100);
        
        QLabel* onLabel = new QLabel("On");
        QLabel* graphLabel = new QLabel("Graph");
        QLabel* animateLabel = new QLabel("Animate");
        QLabel* timeLabel = new QLabel("Time");
        QLabel* fileLabel = new QLabel("File");
        
        const int row = gridLayout->rowCount();
        gridLayout->addWidget(onLabel, row, 0, Qt::AlignHCenter);
        gridLayout->addWidget(graphLabel, row, 1, Qt::AlignHCenter);
        gridLayout->addWidget(animateLabel, row, 2, Qt::AlignHCenter);
        gridLayout->addWidget(timeLabel, row, 3, Qt::AlignHCenter);
        gridLayout->addWidget(fileLabel, row, 4, Qt::AlignHCenter);
    }
    else {
        gridLayout->setColumnStretch(0, 0);
        gridLayout->setColumnStretch(1, 0);
        gridLayout->setColumnStretch(2, 0);
        gridLayout->setColumnStretch(3, 100);
    }
    return gridLayout;
}

/**
 * Update this view controller.
 * @param connectivityLoaderFile
 *    Connectivity loader file in this view controller.
 */
void 
ConnectivityTimeSeriesViewController::updateViewController(ConnectivityLoaderFile* connectivityLoaderFile)
{
    this->connectivityLoaderFile = connectivityLoaderFile;
    if (this->connectivityLoaderFile != NULL) {
        if (this->connectivityLoaderFile != this->previousConnectivityLoaderFile) {
            this->deleteAnimator();
        }
        if (this->animator == NULL) {
            this->animator = new TimeSeriesManagerForViewController(this);            
        }
        
        Qt::CheckState enabledState = Qt::Unchecked;
        if (this->connectivityLoaderFile->isDataLoadingEnabled()) {
            enabledState = Qt::Checked;
        }
        this->enabledCheckBox->setCheckState(enabledState);
        
        this->fileNameLineEdit->setText(this->connectivityLoaderFile->getFileNameNoPath());
    }
    else {
        this->deleteAnimator();
    }
    
    this->previousConnectivityLoaderFile = this->connectivityLoaderFile;
}

/**
 * Update the view controller.
 */
void 
ConnectivityTimeSeriesViewController::updateViewController()
{
    this->updateViewController(this->connectivityLoaderFile);    
}

/**
 * Called when enabled check box changes state.
 */
void 
ConnectivityTimeSeriesViewController::enabledCheckBoxStateChanged(int state)
{
    const bool selected = (state == Qt::Checked);
    if (this->connectivityLoaderFile != NULL) {
        this->connectivityLoaderFile->setDataLoadingEnabled(selected);
        this->fileNameLineEdit->setText(this->connectivityLoaderFile->getFileNameNoPath());
    }
    
    this->updateUserInterfaceAndGraphicsWindow();
}

/**
 * Set the visiblity of this overlay view controller.
 */
void 
ConnectivityTimeSeriesViewController::setVisible(bool visible)
{
    this->gridLayoutGroup->setVisible(visible);
    
//    this->updateUserInterfaceAndGraphicsWindow();
}

/**
 * Update graphics and GUI after 
 */
void 
ConnectivityTimeSeriesViewController::updateUserInterfaceAndGraphicsWindow()
{
    this->updateOtherConnectivityTimeSeriesViewControllers();
    
    EventManager::get()->sendEvent(EventSurfaceColoringInvalidate().getPointer());
    //EventManager::get()->sendEvent(EventUserInterfaceUpdate().getPointer());
    EventManager::get()->sendEvent(EventGraphicsUpdateAllWindows().getPointer());
}

/**
 * Update other connectivity view controllers other than 'this' instance
 * that contain the same connectivity file.
 */
void 
ConnectivityTimeSeriesViewController::updateOtherConnectivityTimeSeriesViewControllers()
{
    if (this->connectivityLoaderFile != NULL) {
        for (std::set<ConnectivityTimeSeriesViewController*>::iterator iter = allConnectivityTimeSeriesViewControllers.begin();
             iter != allConnectivityTimeSeriesViewControllers.end();
             iter++) {
            ConnectivityTimeSeriesViewController* clvc = *iter;
            if (clvc != this) {
                clvc->updateViewController();
            }
        }
    }
}

/**
 * Called when graph display tool button is triggered.
 * @param status
 *    New status.
 */
void 
ConnectivityTimeSeriesViewController::graphDisplayActionTriggered(bool status)
{
    if (this->connectivityLoaderFile != NULL) {
        if (this->connectivityLoaderFile->isDenseTimeSeries()) {
            this->connectivityLoaderFile->setTimeSeriesGraphEnabled(status);
            GuiManager::get()->getTimeCourseDialog((void *)this->connectivityLoaderFile)->setTimeSeriesGraphEnabled(status);
        }
    }
}

/**
 * Called when animate tool button is triggered.
 * @param status
 *    New status.
 */
void 
ConnectivityTimeSeriesViewController::animateActionTriggered(bool /*status*/)
{
    if (this->connectivityLoaderFile != NULL) {
        this->animator->toggleAnimation();
    }
}

/**
 * Called when animate time spin box value is changed.
 * @param value
 *    New value.
 */
void 
ConnectivityTimeSeriesViewController::timeSpinBoxValueChanged(double value)
{
    if (this->connectivityLoaderFile != NULL) {
        const double currentValue = this->connectivityLoaderFile->getSelectedTimePoint();
        if (std::fabs(currentValue - value) < 0.001) {
            std::cout << "IGNORED UNCHANGED SPIN BOX VALUE" << std::endl;
            return;
        }
        
        ConnectivityLoaderManager* manager = GuiManager::get()->getBrain()->getConnectivityLoaderManager();
        ElapsedTimer et;
        et.start();
        CaretPreferences *prefs = SessionManager::get()->getCaretPreferences();
        double timeStepOffset;
        prefs->getAnimationStartTime(timeStepOffset);
        
        bool dataLoadedFlag = false;
        if (manager->loadTimePointAtTime(this->connectivityLoaderFile, 
                                         this->timeSpinBox->value()-timeStepOffset)) {
            dataLoadedFlag = true;
        }
        const float loadTime = et.getElapsedTimeSeconds();
        
        if (dataLoadedFlag) {
            this->updateOtherConnectivityTimeSeriesViewControllers();
            const float guiUpdateTime = et.getElapsedTimeSeconds() - loadTime;
            
            EventManager::get()->sendEvent(EventSurfaceColoringInvalidate().getPointer());
            const float invalidateTime = et.getElapsedTimeSeconds() - guiUpdateTime;
            
            EventManager::get()->sendEvent(EventGraphicsUpdateAllWindows(true).getPointer());   
            const float graphicsTime= et.getElapsedTimeSeconds() - invalidateTime;
            
            const float totalTime = et.getElapsedTimeSeconds();
            
            CaretLogFiner("Time load/conngui/invalidate/graphics/total " 
                          + AString::number(loadTime) + " "
                          + AString::number(guiUpdateTime) + " "
                          + AString::number(invalidateTime) + " "
                          + AString::number(graphicsTime) + " "
                          + AString::number(totalTime));
        }
    }
}

void 
ConnectivityTimeSeriesViewController::receiveEvent(Event* event)
{    
    if(event->getEventType() == EventTypeEnum::EVENT_UPDATE_ANIMATION_START_TIME) {
        EventUpdateAnimationStartTime *e = (EventUpdateAnimationStartTime *)event->getPointer();
        double time = e->getStartTime();
        this->setAnimationStartTime(time);
        e->setEventProcessed();
    }
}

QDoubleSpinBox *
ConnectivityTimeSeriesViewController::getTimeSpinBox()
{
    return this->timeSpinBox;
}

void 
ConnectivityTimeSeriesViewController::setAnimationStartTime(const double &value)
{
    if (this->connectivityLoaderFile != NULL) {
        if (this->connectivityLoaderFile->isDenseTimeSeries()) {
            if(animator) animator->setAnimationStartTime(value);
            TimeCourseDialog * dialog = GuiManager::get()->getTimeCourseDialog(this->connectivityLoaderFile);
            dialog->setAnimationStartTime(value);
        }
    }
}




