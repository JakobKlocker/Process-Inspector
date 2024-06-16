#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

std::map<LPVOID, MemoryPage> memoryMap;

MainWindow::MainWindow(QWidget *parent, DWORD newPid)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , pid(newPid)
{
    ui->setupUi(this);
    loadProcessMemory();
    model = new QStringListModel(this);
    ui->listView->setModel(model);
    displayMemoryPages(memoryMap);

}

MainWindow::~MainWindow()
{
    delete ui;

}


void MainWindow::displayMemoryPages(const std::map<LPVOID, MemoryPage>& memoryMap)
{
    QStringList dataList;

    size_t bytesPerLine = 36;
    int i = 0;
    for (auto it = memoryMap.begin(); i < 10 && it != memoryMap.end(); ++it, ++i)
    {
        QString baseAddressStr = QString::asprintf("0x%llx", reinterpret_cast<unsigned long long>(it->first));
        const MemoryPage& page = it->second;

        for (size_t i = 0; i < page.bytes.size(); i += bytesPerLine)
        {

            LPVOID currentAddress = reinterpret_cast<LPVOID>(reinterpret_cast<uintptr_t>(it->first) + i);
            QString addressStr = QString::asprintf("0x%llx", reinterpret_cast<unsigned long long>(currentAddress));

            QString bytesStr;
            size_t chunkSize = std::min(bytesPerLine, page.bytes.size() - i);
            for (size_t j = 0; j < chunkSize; ++j) {
                bytesStr += QString::asprintf("%02X ", page.bytes[i + j]);
            }

            QString itemText = QString("%1: %2").arg(addressStr).arg(bytesStr.trimmed());
            dataList << itemText;

        }
    }

    model->setStringList(dataList);
}



bool MainWindow::loadProcessMemory()
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS , FALSE, this->pid);
    if (hProcess == nullptr) {
        qDebug() << "Error: Failed to open process" << this->pid;
        return false;
    }

    MEMORY_BASIC_INFORMATION memoryInfo;

    LPVOID address = nullptr;
    std::size_t totalBytesRead = 0;
    std::size_t bytesRead = 0;
    DWORD oldProtect;

    while(VirtualQueryEx(hProcess, address, &memoryInfo, sizeof(memoryInfo)) == sizeof(memoryInfo))
    {
        if (memoryInfo.State == MEM_COMMIT)
        {
            if (!(memoryInfo.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE)))
            {
                if (!VirtualProtectEx(hProcess, memoryInfo.BaseAddress, memoryInfo.RegionSize, PAGE_EXECUTE_READWRITE, &oldProtect)) {
                    qDebug() << "Error changing protection of memory at" << memoryInfo.BaseAddress;
                }
            }

        std::vector<BYTE> buffer(memoryInfo.RegionSize);

        if(ReadProcessMemory(hProcess, address, buffer.data(), memoryInfo.RegionSize, &bytesRead) && bytesRead == memoryInfo.RegionSize)
        {
            MemoryPage page = {std::move(buffer), memoryInfo.RegionSize};
            memoryMap[memoryInfo.BaseAddress] = std::move(page);
        }
        else
            qDebug() << "Error readingProcessMem";

        qDebug() << address << "   " << bytesRead;

        totalBytesRead += bytesRead;
        }
        address = LPVOID((BYTE*)memoryInfo.BaseAddress + memoryInfo.RegionSize);
    }
    return true;
}
