#include "ProcessDetector.h"

#include <QDebug>


ProcessDetector::ProcessDetector(const wchar_t* processName, QObject* parent)
    : QObject(parent), _processName(processName), _stop(false)
{
    _thread = std::thread(&ProcessDetector::detectProcess, this);
}

ProcessDetector::~ProcessDetector()
{
    _stop = true;
    if (_thread.joinable()) {
        _thread.join();
    }
}



void ProcessDetector::detectProcess() 
{
    while (!_stop) 
    {
        Sleep(3000); // Check every second

        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(PROCESSENTRY32);

        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

        if (Process32First(snapshot, &entry) == TRUE) {
            while (Process32Next(snapshot, &entry) == TRUE) {
                if (_wcsicmp(entry.szExeFile, _processName) == 0) {
                    //MessageBox(0, "Detected the process", "Notification", 0);
                    qDebug() << "check! : " << _processName;
                    emit sendProcessChecked();
                    CloseHandle(snapshot);
                    return;
                }
            }
        }
        CloseHandle(snapshot);
    }
}