#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent, DWORD newPid)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , pid(newPid)
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

// Still in progress, change datatype to map so having all the memory addresses paired to the bytes. Also fix VirtualProtect
bool MainWindow::loadProcessMemory()
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS , FALSE, this->pid);
    if (hProcess == nullptr) {
        qDebug() << "Error: Failed to open process" << this->pid;
        return false;
    }

    MEMORY_BASIC_INFORMATION memoryInfo;

    std::vector<BYTE>memory(0);
    LPVOID address = nullptr;
    std::size_t totalBytesRead = 0;
    std::size_t bytesRead = 0;
    DWORD oldProtect;

    while(VirtualQueryEx(hProcess, address, &memoryInfo, sizeof(memoryInfo)) == sizeof(memoryInfo))
    {
        if (memoryInfo.State == MEM_COMMIT)
        {
            if (!VirtualProtectEx(hProcess, memoryInfo.BaseAddress, memoryInfo.RegionSize, PAGE_EXECUTE_READWRITE, &oldProtect)) {
                qDebug() << "Error changing protection of memory at" << memoryInfo.BaseAddress;
            }

        if(memoryInfo.RegionSize == -1)
            std::string s = "";
        memory.resize(totalBytesRead + memoryInfo.RegionSize);

        if(!ReadProcessMemory(hProcess, address, &memory[totalBytesRead], memoryInfo.RegionSize, &bytesRead))
        {
            qDebug() << "Error readingProcessMem";
            DWORD64 test = memory.size();
            memory.resize(totalBytesRead);
        }

        qDebug() << address << "   " << bytesRead;

        if(0x25ce8d000 == (DWORD64)address)
            std::string test = "";

        totalBytesRead += bytesRead;
        }
        address = LPVOID((BYTE*)memoryInfo.BaseAddress + memoryInfo.RegionSize);
    }



    return true;
}
