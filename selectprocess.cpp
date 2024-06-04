#include "selectprocess.h"
#include "ui_selectprocess.h"
#include <QDialog>
#include <QMessageBox>

#include <windows.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <locale>
#include <codecvt>

std::vector<std::pair<DWORD64, std::string>> listProcesses();
QPixmap getProcessIcon(DWORD64 processID);

SelectProcess::SelectProcess(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectProcess),
    selectedPid(0)
{
    ui->setupUi(this);
    std::vector<std::pair<DWORD64, std::string>> processList;
    processList = listProcesses();

    for(auto &s : processList)
    {
        QString procIdHex = QString::number(s.first, 16).rightJustified(8, '0');
        QString itemText = procIdHex + " | " + s.second.c_str();
        QPixmap icon = getProcessIcon(s.first);
        QListWidgetItem *item = new QListWidgetItem(itemText);
        if (!icon.isNull())
            item->setIcon(QIcon(icon));
        else
            item->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon));
        ui->listWidget->addItem(item);
    }
}

SelectProcess::~SelectProcess()
{
    delete ui;
}

QPixmap getProcessIcon(DWORD64 processID)
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS , FALSE, processID);
    if (hProcess == nullptr) {
        qDebug() << "Error: Failed to open process" << processID;
        return QPixmap();
    }

    wchar_t devicePath[MAX_PATH];
    if (GetProcessImageFileName(hProcess, devicePath, MAX_PATH) == 0) {
        qDebug() << "Error: Failed to get process image file name";
        CloseHandle(hProcess);
        return QPixmap();
    }

    wchar_t drives[255];
    DWORD drivesLength = GetLogicalDriveStringsW(255, drives);

    std::wstring fullPath = L"";
    for (int i = 0; i < drivesLength; i += 4) {
        wchar_t drive[4];
        wcscpy_s(drive, 4, &drives[i]);
        drive[2] = 0;
        wchar_t dosDevice[MAX_PATH];
        int re = QueryDosDeviceW(drive, dosDevice, MAX_PATH);
        if (re) {
            qDebug() << devicePath << " " << dosDevice;
            if (_wcsnicmp(devicePath, dosDevice, wcslen(dosDevice - 1)) == 0) {

                fullPath += drive;
                fullPath += &devicePath[wcslen(dosDevice)];
                qDebug() << "Physical Path:" << QString::fromWCharArray(fullPath.c_str());
            }
        }
    }

    HICON hIconLarge, hIconSmall;
    int extractRet = ExtractIconEx(fullPath.c_str(), 0, &hIconLarge, &hIconSmall, 1);
    if (extractRet == 0) {
        qDebug() << "Error: Failed to extract icon";
        CloseHandle(hProcess);
        return QPixmap();
    }

    QPixmap pixmap;
    pixmap.convertFromImage(QImage::fromHICON(hIconLarge));
    DestroyIcon(hIconLarge);
    CloseHandle(hProcess);
    qDebug() << "Pixmap found " << pixmap;
    return pixmap;
}

void SelectProcess::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    std::string itemText = item->text().toStdString();
    this->selectedPid = std::stoul(itemText.substr(0,itemText.find(" ")), 0, 16);
    qDebug() << this->selectedPid;
    accept();
}

std::string wideCharToString(const wchar_t* wideCharString)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wideString(wideCharString);
    return converter.to_bytes(wideString);
}

std::vector<std::pair<DWORD64, std::string>> listProcesses()
{
    std::vector<std::pair<DWORD64, std::string>> ret;

    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        qDebug() << "Error: CreateToolhelp32Snapshot failed";
        return {};
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32)) {
        qDebug() << "Error: Process32First failed";
        CloseHandle(hProcessSnap);
        return {};
    }

    do {
        qDebug() << "Process ID:" << pe32.th32ProcessID;
        qDebug() << "Process Name:" << QString::fromWCharArray(pe32.szExeFile);
        qDebug() << "--------------------------------------";
        ret.push_back(std::make_pair(pe32.th32ProcessID, wideCharToString(pe32.szExeFile)));
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return ret;
}
