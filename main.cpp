#include "mainwindow.h"
#include <QApplication>

#include "UnitTest/testunit.h"
#include "Utils/initials.h"

int main(int argc, char *argv[])
{
//    PreInitials::Initialize();

    QApplication a(argc, argv);
    a.setApplicationName("Aneursym-Demo");


    MainWindow w;
    w.show();

    return a.exec();
}
