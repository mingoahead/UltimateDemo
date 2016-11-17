#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "testunit.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QProcess>
#include <QProcessEnvironment>
#include <QException>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QSizePolicy>
#include <QMap>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    ui->panelDock->setFeatures(QDockWidget::DockWidgetMovable
                                | QDockWidget::DockWidgetFloatable);
    ui->panelDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    ui->NavgunitWidget->setSizePolicy(sizePolicy);
    createActions();
    createMenus();
    createLayout();
    createTinyLayout();
    ui->cbb_show_3dreg->setEnabled(false);
    ui->cbb_show_ls->setEnabled(false);
    ui->cbb_show_sd->setEnabled(false);
    initModules();
    initRenderWindow();

}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::createActions()
{
    m_openStlAct = new QAction(tr("&Open"), this);
    connect(m_openStlAct, SIGNAL(triggered()), this, SLOT(open()));
    m_exitAct = new QAction(tr("&Exit"), this);
    connect(m_exitAct, SIGNAL(triggered()), this, SLOT(exit()));
    m_viewDockPanelAct = new QAction(tr("View Panel"), this);
    m_viewDockPanelAct -> setCheckable(true);
    m_viewDockPanelAct -> setChecked(true);
    connect(m_viewDockPanelAct, SIGNAL(toggled(bool)), this, SLOT(viewDockPanel(bool)));

    m_segAct = new QAction(tr("&Segment"), this);
    connect(m_segAct, SIGNAL(triggered()), this, SLOT(runSegment()));
    m_testAct = new QAction(tr("Test Module"), this);
    connect(m_testAct, SIGNAL(triggered()), this, SLOT(unittest()));
}

void MainWindow::createMenus()
{
    m_fileMenu = menuBar() -> addMenu(tr("&File"));
    m_fileMenu -> addAction(m_openStlAct);
    m_fileMenu -> addAction(m_exitAct);
    m_viewMenu = menuBar() -> addMenu(tr("&View"));
    m_viewMenu -> addAction(m_viewDockPanelAct);
    m_segMenu = menuBar() -> addMenu(tr("&System"));
    m_segMenu -> addAction(m_segAct);
    m_testMenu = menuBar() -> addMenu(tr("&Test"));
    m_testMenu -> addAction(m_testAct);
}

void MainWindow::createLayout()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;

    auto leftLayout = new QVBoxLayout;
    auto rightLayout = new QVBoxLayout;
    mainLayout -> addLayout(leftLayout);
    mainLayout -> addLayout(rightLayout);

    m_vtkWidget = new QVTKWidget;
    m_vtkWidget->autoFillBackground();
    leftLayout->addWidget(m_vtkWidget);
    mainLayout->update();
    centralWidget()->setLayout(mainLayout);
}

void MainWindow::createTinyLayout()
{
    QHBoxLayout * mainLayout = new QHBoxLayout;
    QVBoxLayout * leftLayout = new QVBoxLayout;
    QVBoxLayout * rightLayout = new QVBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    m_smallvtkWidget = new QVTKWidget;
    m_smallvtkWidget->autoFillBackground();
    leftLayout->addWidget(m_smallvtkWidget);
    mainLayout->update();
    ui->NavgunitWidget->setLayout(mainLayout);

}

void MainWindow::initModules()
{
     m_appUnit = new AneurysmUnit(m_vtkWidget -> GetRenderWindow());
     m_navgUnit = new NavigationUnit(m_smallvtkWidget -> GetRenderWindow());

}

void MainWindow::initRenderWindow()
{
    m_vtkWidget -> GetRenderWindow() -> AddRenderer(m_appUnit -> GetRenderer());
    updateRenderWindow();
}

void MainWindow::updateRenderWindow()
{
    m_vtkWidget->GetRenderWindow()->Render();
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Segment Model for ")
                                                    , tr("/home"), tr("mhd or mha file (*.mhd)"));
    if(!fileName.isNull()) {
        m_appUnit->LoadRawData(fileName.toStdString());
        updateRenderWindow();
    }
}

void MainWindow::openStl(int option)
{
    // to open stl file
    QMap<int, QString> segMap;
    segMap.insert(1, "3D Region Growing");
    segMap.insert(2, "Level Set");
    segMap.insert(3, "Shape Detection");
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Segment Model for ") + segMap[option]
                                                    , tr("/home"), tr("stl file (*.stl)"));
    if(!fileName.isNull()) {
        m_appUnit->ReadInputSegmentationModel(fileName.toStdString(), option);
        m_appUnit->ShowSegmentationModel(option);
        updateRenderWindow();
    }
}

void MainWindow::exit()
{
    QApplication::quit();
}

void MainWindow::viewDockPanel(bool checked)
{
    checked ? ui->panelDock->show() : ui->panelDock->hide();
}

void MainWindow::runSegment()
{
//   QProcessEnvironment env;
//   QProcess process;
//   env.insert("MYPATH", QString("D:/3dresearch/QtItkVtk/test/UltimateDemo/SubSys"));

   //QString prog = "./WTK.exe";
//   process.setProcessEnvironment(env);
//   process.start("./WTK.exe");
//   QString path;
//   QDir dir;
//   path = dir.currentPath();
//   QMessageBox::warning(0, "PATH", path, QMessageBox::Yes);

   QString program = "../UltimateDemo/SubSys/WTK.exe";
   QProcess::startDetached(program, QStringList());

}

void MainWindow::unittest()
{
    TestUnit *test = new TestUnit();
    test -> RunTest();
    delete test;
}

void MainWindow::on_cb_open3dreg_clicked()
{
    m_appUnit->RegisterDisplay(1);
    updateRenderWindow();
    openStl(1);
    ui->cbb_show_3dreg->setEnabled(true);
}

void MainWindow::on_cbb_show_3dreg_currentIndexChanged(int index)
{
    m_appUnit->RegisterDisplay(1);
    updateRenderWindow();
    if(index == 0) {
        m_appUnit->ShowFrameMode(1);
    }else if(index == 1) {
        m_appUnit->ShowLineMode(1);
    }else if(index == 2) {
        m_appUnit->ShowPointMode(1);
    }else if(index == 3) {
        m_appUnit->HideSegmentationModel(1);
    }
    updateRenderWindow();
}

void MainWindow::on_cb_openlevelset_clicked()
{
    m_appUnit->RegisterDisplay(1);
    updateRenderWindow();
    openStl(2);
    ui->cbb_show_ls->setEnabled(true);

}

void MainWindow::on_cbb_show_ls_currentIndexChanged(int index)
{
    m_appUnit->RegisterDisplay(1);
    updateRenderWindow();
    if(index == 0) {
        m_appUnit->ShowFrameMode(2);
    }else if(index == 1) {
        m_appUnit->ShowLineMode(2);
    }else if(index == 2) {
        m_appUnit->ShowPointMode(2);
    }else if(index == 3) {
        m_appUnit->HideSegmentationModel(2);
    }
    updateRenderWindow();
}

void MainWindow::on_cb_openregdetect_clicked()
{
    m_appUnit->RegisterDisplay(1);
    updateRenderWindow();
    openStl(3);
    ui->cbb_show_sd->setEnabled(true);
}

void MainWindow::on_cbb_show_sd_currentIndexChanged(int index)
{
    m_appUnit->RegisterDisplay(1);
    updateRenderWindow();
    if(index == 0) {
        m_appUnit->ShowFrameMode(3);
    }else if(index == 1) {
        m_appUnit->ShowLineMode(3);
    }else if(index == 2) {
        m_appUnit->ShowPointMode(3);
    }else if(index == 3) {
        m_appUnit->HideSegmentationModel(3);
    }
    updateRenderWindow();
}
void MainWindow::on_pb_test1view_clicked()
{
    m_appUnit -> RegisterDisplay(1);
    updateRenderWindow();
}

void MainWindow::on_pb_test2view_clicked()
{
    m_appUnit -> RegisterDisplay(2);
    updateRenderWindow();
}
void MainWindow::on_pb_test3view_clicked()
{

//    updateRenderWindow();
    if(!(m_appUnit->RawDataExist())) {
        QMessageBox::warning(NULL, "warning", "First, need to load raw data! ", QMessageBox::Yes);
        return ;
    }
    m_appUnit -> RegisterDisplay(3);
    double curpos[3] = {4.99979, -134.5, 1157.95};
    m_appUnit->Draw3DSlice(curpos);
    updateRenderWindow();


}

void MainWindow::on_pb_test4view_clicked()
{
    m_appUnit -> RegisterDisplay(4);
    updateRenderWindow();
}

void MainWindow::on_cb_ctrline_method1_toggled(bool checked)
{
    if(checked){
        m_appUnit->GetCenterLine(0);
        m_appUnit->DrawCenterLine(0, true);
    }else{
        m_appUnit->HideCenterLine(0, true);
    }
}

void MainWindow::on_cb_ctrline_method2_toggled(bool checked)
{

}

void MainWindow::on_cb_ctrline_method3_toggled(bool checked)
{

}

void MainWindow::on_rb_pointpicker_toggled(bool checked)
{
     std::cout << ui->rb_pointpicker->isChecked() << std::endl;
     m_appUnit->SetPointPickerEnabled(checked);
}


void MainWindow::on_tb_play_clicked()
{

}

void MainWindow::on_tb_stop_clicked()
{

}

void MainWindow::on_tb_backward_clicked()
{

}

void MainWindow::on_tb_forward_clicked()
{

}

void MainWindow::on_cbb_curPath_currentIndexChanged(int index)
{

}


