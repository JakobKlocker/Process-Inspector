#include "mainwindow.h"
#include "selectprocess.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DWORD pid = 0;

    SelectProcess selectProc;
    if(selectProc.exec() ==  QDialog::Accepted)
        pid = selectProc.getSelectedPid();

    qDebug() << pid;

    if(!pid)
        return 0;

    MainWindow w(nullptr, pid);
    w.show();
    return a.exec();
}
