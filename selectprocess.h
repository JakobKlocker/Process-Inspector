#ifndef SELECTPROCESS_H
#define SELECTPROCESS_H

#include <QWidget>
#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class SelectProcess;
}

class SelectProcess : public QDialog
{
    Q_OBJECT

public:
    explicit SelectProcess(QWidget *parent = nullptr);
    ~SelectProcess();

private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::SelectProcess *ui;
};

#endif // SELECTPROCESS_H
