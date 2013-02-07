#ifndef __SPEC_FILE_MANAGEMENT_DIALOG_H__
#define __SPEC_FILE_MANAGEMENT_DIALOG_H__

/*LICENSE_START*/
/*
 * Copyright 2013 Washington University,
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
#include <QSignalMapper>

#include "DataFileTypeEnum.h"
#include "StructureEnum.h"
#include "WuQDialogModal.h"

class QActionGroup;
class QCheckBox;
class QGridLayout;
class QLabel;
class QToolBar;
class QToolButton;

namespace caret {

    class Brain;
    class CaretDataFile;
    class GuiSpecFileDataFile;
    class GuiSpecFileDataFileTypeGroup;
    class SpecFile;
    class SpecFileDataFile;
    class SpecFileDataFileTypeGroup;
    class WuQEventBlockingFilter;
    class WuQWidgetObjectGroup;
    
    class SpecFileManagementDialog : public WuQDialogModal {
        
        Q_OBJECT

    public:
        static SpecFileManagementDialog* createOpenSpecFileDialog(Brain* brain,
                                                                  SpecFile* specFile,
                                                                  QWidget* parent);
        
        static SpecFileManagementDialog* createManageFilesDialog(Brain* brain,
                                                                 QWidget* parent);
        
        virtual ~SpecFileManagementDialog();
        
    private slots:
        void toolBarFileTypeActionTriggered(QAction* action);
        
        void toolBarStructuresActionTriggered(QAction* action);
        
        void toolBarSelectFilesActionTriggered(QAction* action);
        
        void chooseSpecFileNameActionTriggered();

        void fileLoadCheckBoxSelected(int indx);
        
        void fileSaveCheckBoxSelected(int indx);
        
        void fileInSpecCheckBoxSelected(int indx);
        
        void fileReloadOrOpenFileActionSelected(int indx);
        
        void fileOptionsActionSelected(int indx);
        
        void fileSelectFileNameActionSelected(int indx);
        
    private:
        friend class GuiSpecFileDataFileTypeGroup;
        friend class GuiSpecFileDataFile;
        
        enum Mode {
            MODE_MANAGE_FILES,
            MODE_OPEN_SPEC_FILE
        };
        
        SpecFileManagementDialog(const Mode dialogMode,
                                 Brain* brain,
                                 SpecFile* specFile,
                                 const AString& dialogTitle,
                                 QWidget* parent);
        
        SpecFileManagementDialog(const SpecFileManagementDialog&);

        SpecFileManagementDialog& operator=(const SpecFileManagementDialog&);
        
        QToolBar* createFilesTypesToolBar();
        
        QToolBar* createFilesSelectionToolBar();
        
        QToolBar* createStructureToolBar();
        
        QToolBar* createToolBarWithActionGroup(const QString& text,
                                               QActionGroup* actionGroup);
        
        AString getEditedGroupName(const AString& groupName) const;
        
        GuiSpecFileDataFile* getSpecFileDataFileBySignalMapperIndex(const int signalMapperIndex);
        
        void updateDisplayedFiles();
        
        virtual void okButtonClicked();
        
        void okButtonClickedOpenSpecFile();
        
        bool okButtonClickedManageFiles();
        
        AString writeSpecFile(const bool writeOnlyIfModified);
        
        // ADD_NEW_MEMBERS_HERE
        
        const Mode m_dialogMode;

        Brain* m_brain;
        
        SpecFile* m_specFile;
    
        WuQEventBlockingFilter* m_comboBoxWheelEventBlockingFilter;
        
        QActionGroup* m_fileTypesActionGroup;
        
        QActionGroup* m_fileSelectionActionGroup;
        
        QActionGroup* m_structureActionGroup;
        
        std::vector<GuiSpecFileDataFileTypeGroup*> m_guiSpecFileDataFileTypeGroups;
        
        QCheckBox* m_specFileSaveCheckBox;
        
        QLabel* m_specFileStatusLabel;
        
        QLabel* m_specFileNameLabel;
        
        QToolButton* m_chooseSpecFileToolButton;
        
        QSignalMapper* m_fileLoadCheckBoxSignalMapper;
        
        QSignalMapper* m_fileSaveCheckBoxSignalMapper;
        
        QSignalMapper* m_fileInSpecCheckBoxSignalMapper;
        
        QSignalMapper* m_fileReloadOrOpenFileActionSignalMapper;
        
        QSignalMapper* m_fileOptionsActionSignalMapper;
        
        QSignalMapper* m_fileSelectFileNameActionSignalMapper;
        
        int m_specFileDataFileCounter;
        
        static const int SHOW_FILES_ALL;
        static const int SHOW_FILES_LOADED;

    };
    
    
    class GuiSpecFileDataFileTypeGroup : public QObject {
        Q_OBJECT
        
    public:
        GuiSpecFileDataFileTypeGroup(Brain* brain,
                                     const SpecFileManagementDialog::Mode dialogMode,
                                     SpecFileDataFileTypeGroup* specFileDataFileTypeGroup,
                                     const AString& groupName,
                                     QObject* parent);
        
        ~GuiSpecFileDataFileTypeGroup();
        
        void addGuiSpecFileDataFile(GuiSpecFileDataFile* guiSpecFileDataFile);
        
        int32_t getNumberOfGuiSpecFileDataFiles() const;
        
        GuiSpecFileDataFile* getGuiSpecFileDataFile(const int32_t indx);
        
        DataFileTypeEnum::Enum getDataFileType() const;
        
        void updateContent();
        
        void setWidgetsVisible(bool visible);
        
        void setWidgetsVisibleByFileTypeAndStructure(const DataFileTypeEnum::Enum dataFileType,
                                                     const StructureEnum::Enum structure);
        
    private:
        friend class SpecFileManagementDialog;
        
        Brain* m_brain;
        
        const SpecFileManagementDialog::Mode m_dialogMode;

        SpecFileDataFileTypeGroup* m_specFileDataFileTypeGroup;
        
        std::vector<GuiSpecFileDataFile*> m_guiDataFiles;
        
        QLabel* m_groupNameLabel;
        
        QWidget* m_leftHorizontalLineWidget;
        
        QWidget* m_rightHorizontalLineWidget;
    };
    
    
    class GuiSpecFileDataFile : public QObject {
        Q_OBJECT
        
    public:
        GuiSpecFileDataFile(const int signalMapperIndex,
                            Brain* brain,
                            const SpecFileManagementDialog::Mode dialogMode,
                            SpecFileDataFileTypeGroup* specFileDataFileTypeGroup,
                            SpecFileDataFile* specFileDataFile,
                            QObject* parent);
        
        ~GuiSpecFileDataFile();
        
        void setSpecFileDataFile(SpecFileDataFile* specFileDataFile);
        
        void setWidgetsEnabled(const bool enabled);
        
        void setWidgetsVisible(const bool visible);
        
        DataFileTypeEnum::Enum getDataFileType() const;
        
        StructureEnum::Enum getStructure() const;
        
        void setStructure(const StructureEnum::Enum structure);
        
        void updateContent();
        
        int getSignalMapperIndex() const;
        
    public:
        
        const int m_signalMapperIndex;
        
        Brain* m_brain;
        
        const SpecFileManagementDialog::Mode m_dialogMode;
        
        SpecFileDataFileTypeGroup* m_specFileDataFileTypeGroup;
        
        SpecFileDataFile* m_specFileDataFile;

        QCheckBox* m_loadCheckBox;
        
        QCheckBox* m_saveCheckBox;
        
        QCheckBox* m_inSpecFileCheckBox;
        
        QAction* m_reloadOrOpenFileAction;
        
        QToolButton* m_reloadOrOpenFileToolButton;
        
        QLabel* m_statusLabel;
        
        QAction* m_optionsButtonAction;
        
        QToolButton* m_optionsToolButton;
        
        QAction* m_selectFileNameButtonAction;
        
        QToolButton* m_selectFileNameToolButton;
        
        QLabel* m_filenameLabel;
        
        WuQWidgetObjectGroup* m_widgetGroup;
    };
    
#ifdef __SPEC_FILE_MANAGEMENT_DIALOG_DECLARE__
    const int SpecFileManagementDialog::SHOW_FILES_ALL = -1;
    const int SpecFileManagementDialog::SHOW_FILES_LOADED = -2;
#endif // __SPEC_FILE_MANAGEMENT_DIALOG_DECLARE__

} // namespace
#endif  //__SPEC_FILE_MANAGEMENT_DIALOG_H__