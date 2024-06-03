#include "mainwindow.h"
#include "selectprocess.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SelectProcess selectProc;
    selectProc.exec();
    MainWindow w;
    w.show();
    return a.exec();
}
