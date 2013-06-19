#ifndef CHARTINGDIALOG_H
#define CHARTINGDIALOG_H

#include <QDialog>

namespace Ui {
class ChartingDialog;
}

#include "CaretMappableDataFile.h"
#include "EventListenerInterface.h"
#include "CiftiMappableConnectivityMatrixDataFile.h"
#include <QTableView>
class TimeCourseControls;
class Table;
class Plot2D;

namespace caret {
class ChartingDialog : public QDialog, public EventListenerInterface
{
    Q_OBJECT
    
public:
    explicit ChartingDialog(QWidget *parent = 0);
    ~ChartingDialog();
    void setChartMode(QString type);
    void openPconnMatrix(CaretMappableDataFile *pconnFile);    
    QTableView * getMatrixTableView();
    void showDialog();
	void updateSelectedItem(int32_t &row,int32_t &col);
	void receiveEvent(Event* event);
public slots:
	
private slots:
    void on_closeButton_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);  
    
    void showToolBar(bool show=true);
    void showToolBox(bool show=true);

    void customContextMenuRequestedSlot(const QPoint &pos);
    void currentChanged(const QModelIndex &row,const QModelIndex &col);
	void updateMatrix();	
	void currentRowChanged(const QModelIndex & current, const QModelIndex & previous );
private:
    Table *table;
    Ui::ChartingDialog *ui;
    TimeCourseControls *form;    
    Plot2D *plot2d;
    bool showDialogFirstTime;
    CiftiMappableConnectivityMatrixDataFile *cmf;
};
}
#endif // CHARTINGDIALOG_H