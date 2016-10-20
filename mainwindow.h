#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "QVTKWidget.h"
#include "UnitTest/testunit.h"
#include "Modules/AneurysmUnit.h"
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

private:
    void createActions();
    void createMenus();
    void createLayout();
    void initModules();
    void initRenderWindow();
    void updateRenderWindow();
private slots:
    void openStl();
    void exit();
    void viewDockPanel(bool);
    void runSegment();
    void unittest();
};

#endif // MAINWINDOW_H
