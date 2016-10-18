#include "mainwindow.h"
#include <QApplication>

#include "UnitTest/testunit.h"
#include "Utils/initials.h"

int main(int argc, char *argv[])
{

    PreInitials::Initilize();
    QApplication a(argc, argv);
    a.setApplicationDisplayName("Aneursym-Demo");

    MainWindow w;
    w.show();

    return a.exec();
}
