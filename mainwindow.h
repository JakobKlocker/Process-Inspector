#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <windows.h>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, DWORD _pid = 0);
    ~MainWindow();
    void setPid(const DWORD &newPid) {this->pid = newPid;}
    DWORD getPid() {return this->pid;}
    bool loadProcessMemory();
private slots:

private:
    void initTable();
    Ui::MainWindow *ui;
    DWORD pid;
};
#endif // MAINWINDOW_H
