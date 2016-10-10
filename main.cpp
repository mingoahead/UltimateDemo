#include "mainwindow.h"
#include <QApplication>

#include "UnitTest/testunit.h"
#include "Utils/initials.h"

int main(int argc, char *argv[])
{
//    PreInitials::Initialize();

    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
