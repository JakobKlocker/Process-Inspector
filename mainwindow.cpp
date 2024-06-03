#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initTable()
{
    ui->tableWidget->setColumnCount(2);
    QStringList headers;
    headers << "Value" << "Description";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    for (int i = 0; i < 100; ++i) {
        ui->tableWidget->insertRow(i);
        QTableWidgetItem *valueItem = new QTableWidgetItem(QString::number(i * 123.456, 'f', 3));
        QTableWidgetItem *descriptionItem = new QTableWidgetItem(QString("Description %1").arg(i));

        ui->tableWidget->setItem(i, 0, valueItem);
        ui->tableWidget->setItem(i, 1, descriptionItem);

        QString address = QString::asprintf("0x%08X", 0x10000000 + i * 16);
        QTableWidgetItem *addressHeaderItem = new QTableWidgetItem(address);
        ui->tableWidget->setVerticalHeaderItem(i, addressHeaderItem);
    }
}
