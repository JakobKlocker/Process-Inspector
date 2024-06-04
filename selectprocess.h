#ifndef SELECTPROCESS_H
#define SELECTPROCESS_H

#include <QWidget>
#include <QDialog>
#include <QListWidgetItem>
#include <windows.h>

namespace Ui {
class SelectProcess;
}

class SelectProcess : public QDialog
{
    Q_OBJECT

public:
    explicit SelectProcess(QWidget *parent = nullptr);
    ~SelectProcess();
    DWORD getSelectedPid() {return this->selectedPid;}

private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::SelectProcess *ui;
    DWORD selectedPid;
};

#endif // SELECTPROCESS_H
