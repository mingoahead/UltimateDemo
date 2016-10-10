#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "QVTKWidget.h"
#include "UnitTest/testunit.h"

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

    QMenu *m_testMenu;
    QAction *m_testAct;

    QVTKWidget *m_vtkWidget;

private:
    void createActions();
    void createMenus();
    void createLayout();
private slots:
    void openStl();
    void exit();
    void viewDockPanel(bool);
    void unittest();
};

#endif // MAINWINDOW_H
