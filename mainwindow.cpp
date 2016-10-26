#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "UnitTest/testunit.h"
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
//    connect(m_openStlAct, SIGNAL(triggered()), this, SLOT(openStl()));
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
    openStl(1);
}

void MainWindow::on_cb_openlevelset_clicked()
{
    openStl(2);
}

void MainWindow::on_cb_openregdetect_clicked()
{
    openStl(3);
}

void MainWindow::on_pb_test3view_clicked()
{
    m_appUnit -> RegisterDisplay(3);
    updateRenderWindow();
}

void MainWindow::on_pb_test4view_clicked()
{
    m_appUnit -> RegisterDisplay(4);
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
