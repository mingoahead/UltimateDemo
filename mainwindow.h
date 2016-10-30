#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "QVTKWidget.h"
#include "UnitTest/testunit.h"
#include "Modules/AneurysmUnit.h"
#include "Modules/NavigationUnit.h"
#include <vtkSTLReader.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QMenu *m_fileMenu;
    QAction *m_openStlAct;
    QAction *m_exitAct;

    QMenu *m_viewMenu;
    QAction *m_viewDockPanelAct;

    QMenu *m_segMenu;
    QAction *m_segAct;

    QMenu *m_testMenu;
    QAction *m_testAct;

    QVTKWidget *m_vtkWidget;
    AneurysmUnit *m_appUnit;
    QVTKWidget *m_smallvtkWidget;
    NavigationUnit*m_navgUnit;

private:
    void createActions();
    void createMenus();
    void createLayout();
    void createTinyLayout();
    void initModules();
    void initRenderWindow();
    void updateRenderWindow();
private slots:
    void openStl(int option);
    void exit();
    void viewDockPanel(bool);
    void runSegment();
    void unittest();
    void on_cb_open3dreg_clicked();
    void on_cbb_show_3dreg_currentIndexChanged(int index);
    void on_cb_openlevelset_clicked();
    void on_cbb_show_ls_currentIndexChanged(int index);
    void on_cb_openregdetect_clicked();
    void on_cbb_show_sd_currentIndexChanged(int index);


    void on_pb_test1view_clicked();
    void on_pb_test2view_clicked();
    void on_pb_test3view_clicked();
    void on_pb_test4view_clicked();

    void on_cb_ctrline_method1_toggled(bool checked);
    void on_cb_ctrline_method2_toggled(bool checked);
    void on_cb_ctrline_method3_toggled(bool checked);

    void on_tb_play_clicked();
    void on_tb_stop_clicked();
    void on_tb_backward_clicked();
    void on_tb_forward_clicked();
    void on_cbb_curPath_currentIndexChanged(int index);


};

#endif // MAINWINDOW_H
