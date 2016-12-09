#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSlider>
#include <QLabel>
#include "QVTKWidget.h"

#include <vtkSTLReader.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>

#include "exportdatadlg.h"
#include "testunit.h"
#include "AneurysmUnit.h"
#include "NavigationUnit.h"
#include "Modules/Rendering/SurfaceRendering.h"
#include "Modules/Rendering/VolumeRendering.h"
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
    QLabel *m_msgLabel;
    QMenu *m_fileMenu;
    QAction *m_openStlAct;
    QAction *m_exitAct;

    QMenu *m_viewMenu;
    QAction *m_viewDockPanelAct;

    QMenu *m_segMenu;
    QAction *m_segAct;

    QMenu *m_convMenu;
    QAction *m_convAct;
    QMenu *m_testMenu;
    QAction *m_testAct;

    QVTKWidget *m_vtkWidget;
    AneurysmUnit *m_appUnit;
    QVTKWidget *m_smallvtkWidget;
    NavigationUnit*m_navgUnit;

//    VolSurRendering *m_volsurUnit;
    SurfaceRendering *m_surRenderUnit;
    VolumeRendering *m_volRenderUnit;
    ExportDataDlg *exportUntil;
private:
    void createActions();
    void createMenus();
    void createLayout();
    void createTinyLayout();
    void initModules();
    void initRenderWindow();
    void initStatus();
    void updateRenderWindow();
    bool openStl(int option);
private slots:
    void open();
    void exit();
    void viewDockPanel(bool);
    void runConvert();
    void runSegment();
    void unittest();
    void on_cb_open3dreg_clicked();
    void on_cbb_show_3dreg_currentIndexChanged(int index);
    void on_cb_openlevelset_clicked();
    void on_cbb_show_ls_currentIndexChanged(int index);
    void on_cb_openregdetect_clicked();
    void on_cbb_show_sd_currentIndexChanged(int index);
    void on_cb_collectionShow_toggled(bool checked);


    void on_pb_test1view_clicked();
    void on_pb_test2view_clicked();
    void on_pb_test3view_clicked();
    void on_pb_test4view_clicked();

    void on_cb_ctrline_method1_toggled(bool checked);
    void on_cb_ctrline_method2_toggled(bool checked);
    void on_cb_ctrline_method3_toggled(bool checked);
    void on_rb_pointpicker_toggled(bool checked);

    void on_tb_play_clicked();
    void on_tb_stop_clicked();
    void on_tb_backward_clicked();
    void on_tb_forward_clicked();
    void on_cbb_curPath_currentIndexChanged(int index);

    void on_cb_cuttingwidget_toggled(bool checked);
    void on_cb_cuttingContour_toggled(bool checked);
    void on_pb_cut_clicked();

    void on_pb_openSurface_clicked();
    void on_pb_openVolume_clicked();
    void on_pb_openMask_clicked();
    void on_pb_buildVolume_clicked();
    void on_m_hsIterNums_sliderReleased();
    void on_m_hsIterNums_valueChanged(int value);
    void on_m_hsRelaxationFactor_sliderReleased();
    void on_m_hsRelaxationFactor_valueChanged(int value);
    void on_cb_sur_freeroaming_toggled(bool checked);
    void on_m_hsspeed_sliderReleased();


};

#endif // MAINWINDOW_H
