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
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    ui->panelDock->setFeatures(QDockWidget::DockWidgetMovable
                                | QDockWidget::DockWidgetFloatable);
    ui->panelDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    createActions();
    createMenus();

    createLayout();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions()
{
    m_openStlAct = new QAction(tr("&Open STL"), this);
    connect(m_openStlAct, SIGNAL(triggered()), this, SLOT(openStl()));
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

void MainWindow::openStl()
{
    // to open stl file
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
