
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

#include <QGroupBox>
#include <QHBoxLayout>
#include <QTabBar>
#include <QStackedWidget>
#include <QVBoxLayout>

#define __WU_Q_TAB_WIDGET_DECLARE__
#include "WuQTabWidget.h"
#undef __WU_Q_TAB_WIDGET_DECLARE__

using namespace caret;


    
/**
 * \class caret::WuQTabWidget 
 * \brief Replaces QTabWidget and allows the tab to be aligned. 
 *
 * On some platforms, the tabs are centered, and when the tab widget
 * is wide an in a scrollable area it is sometimes difficult to find the
 * tabs.  So, this tab widget allows the tab bar placed at a defined
 * alignment.
 */

/**
 * Constructor.
 * @param alignment
 *    Aligment of the tab.
 * @param parent
 *    Parent of this instance.
 */
WuQTabWidget::WuQTabWidget(const TabAlignment alignment,
                           QObject* parent)
: WuQWidget(parent)
{
    m_tabBar = new QTabBar();
    QObject::connect(m_tabBar, SIGNAL(currentChanged(int)),
                     this, SLOT(tabBarCurrentIndexChanged(int)));
    
    m_stackedWidget = new QStackedWidget();
    
    QHBoxLayout* tabBarLayout = new QHBoxLayout();
    Qt::Alignment tabBarAlignment = Qt::AlignLeft;
    switch (alignment) {
        case TAB_ALIGN_CENTER:
            tabBarAlignment = Qt::AlignHCenter;
            break;
        case TAB_ALIGN_LEFT:
            tabBarAlignment = Qt::AlignLeft;
            break;
        case TAB_ALIGN_RIGHT:
            tabBarAlignment = Qt::AlignRight;
            break;
    }
    tabBarLayout->addWidget(m_tabBar,
                            100,
                            tabBarAlignment);
    tabBarLayout->setMargin(0);
    
    QGroupBox* stackedWidgetGroupBox = new QGroupBox();
    QVBoxLayout* groupBoxLayout = new QVBoxLayout(stackedWidgetGroupBox);
    groupBoxLayout->addWidget(m_stackedWidget);
    
    m_widget = new QWidget();
    QVBoxLayout* verticalLayout = new QVBoxLayout(m_widget);
    verticalLayout->setSpacing(3);
    verticalLayout->addLayout(tabBarLayout,
                              0);
    verticalLayout->addWidget(stackedWidgetGroupBox,
                              0,
                              tabBarAlignment);
    verticalLayout->addStretch();
}

/**
 * Destructor.
 */
WuQTabWidget::~WuQTabWidget()
{
    
}

/**
 * @return The embedded widget.
 */
QWidget*
WuQTabWidget::getWidget()
{
    return m_widget;
}

/**
 * Adds a tab with the given page and label to the tab widget, and returns
 * the index of the tab in the tab bar.  If the tab's label contains an 
 * ampersand, the letter following the ampersand is used as a shortcut
 * for the tab, e.g. if the label is "Bro&wse" then Alt+W becomes a
 * shortcut which will move the focus to this tab.
 * 
 * @param page
 *    New page that is added (must not be NULL).
 * @param label
 *    Label displayed in the page's tab.
 */
void
WuQTabWidget::addTab(QWidget* page,
            const QString& label)
{
    m_tabBar->addTab(label);
    m_stackedWidget->addWidget(page);    
}

/**
 * Called when the tab bar changes the current widget.
 * @param index
 *    Index of selected widget.
 */
void
WuQTabWidget::tabBarCurrentIndexChanged(int index)
{
    setCurrentIndex(index);
    emit currentChanged(index);
}

/**
 * @return Returns the index position of the current tab page.
 * The current index is -1 if there is no current widget.
 */
int
WuQTabWidget::currentIndex() const
{
    return m_tabBar->currentIndex();
}

/**
 * @return Returns a pointer to the page currently being displayed by the 
 * tab dialog. The tab dialog does its best to make sure that this value 
 * is never 0 (but if you try hard enough, it can be).
 */
QWidget*
WuQTabWidget::currentWidget() const
{
    return m_stackedWidget->currentWidget();
}

//signals:
//void currentChanged(int index);

/**
 * Makes widget at the given index the current widget. The widget used must
 * be a page in this tab widget.
 */
void
WuQTabWidget::setCurrentIndex(int index)
{
    m_tabBar->blockSignals(true);
    m_tabBar->setCurrentIndex(index);
    m_tabBar->blockSignals(false);
    m_stackedWidget->setCurrentIndex(index);
}

/**
 * Makes widget the current widget. The widget used must be a page in 
 * this tab widget.
 */
void
WuQTabWidget::setCurrentWidget(QWidget* widget)
{
    const int indx = m_stackedWidget->indexOf(widget);
    if (indx > 0) {
        m_tabBar->blockSignals(true);
        m_tabBar->setCurrentIndex(indx);
        m_tabBar->blockSignals(false);
    }
}
