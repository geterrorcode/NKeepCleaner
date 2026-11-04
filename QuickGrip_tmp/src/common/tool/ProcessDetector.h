#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include <thread>

#include <QObject>


class ProcessDetector : public QObject
{
    Q_OBJECT

public:
    //ProcessDetector(const char* processName);
    ProcessDetector(const wchar_t* processName, QObject* parent = Q_NULLPTR);
    ~ProcessDetector();

    void detectProcess();

private:
    const wchar_t* _processName;
    std::thread _thread;
    volatile bool _stop;


Q_SIGNALS:
    void sendProcessChecked();
};

