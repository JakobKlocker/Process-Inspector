#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "MemoryPage.h"

#include <QMainWindow>
#include <windows.h>
#include <vector>
#include <QListView>
#include <QStandardItemModel>
#include <QStringListModel>
#include <algorithm>

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
    void displayMemoryPages(const std::map<LPVOID, MemoryPage>& memoryMap);
private slots:

private:
    void initTable();
    Ui::MainWindow *ui;
    DWORD pid;
    int itemsPerPage;
    QListView *memoryListView;
    QStandardItemModel *listModel;
    std::map<LPVOID, MemoryPage> memoryMap;
    QStringListModel *model;
};
#endif // MAINWINDOW_H
