#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QjsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QProcess>

#include <Windows.h>
#include <Psapi.h>

// Service name
//#define SERVICE_NAME TEXT("MyService")

constexpr auto SERVICE_NAME = L"N-KeepCleaner Svc";

constexpr auto UPDATER_NAME = L"N-KeepCleanerup.exe";
constexpr auto OPTIMIZER_NAME = L"N-KeepCleaner.exe";

// Prototypes
VOID WINAPI serviceMain(DWORD, LPTSTR*);
VOID WINAPI serviceControlHandler(DWORD);

// Globals
SERVICE_STATUS_HANDLE g_statusHandle;
SERVICE_STATUS g_serviceStatus;

bool isStartCfg() {

    QString configpath = QString("%1/cfg/config.json").arg(qApp->applicationDirPath());
    if (!QFile::exists(configpath))
        return false;

    QFile config_file(configpath);
    if (!config_file.open(QIODevice::ReadOnly))
        return false;

    QByteArray bytes = config_file.readAll();

    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(bytes, &jsonError);

    if (jsonError.error != QJsonParseError::NoError)
        return false;

    bool opt = false;
    if (document.isObject()) {

        QJsonObject jsonObj = document.object();
        QJsonObject obj = jsonObj.value("config").toObject();

        auto startup = obj.take("startup");
        if (startup.isBool())
            opt = startup.toBool();

    }

    return opt;

}



bool isProcessRunning(const QStringList& processNames) {
    DWORD processIds[1024], bytesReturned;

    if (!EnumProcesses(processIds, sizeof(processIds), &bytesReturned)) {
        qCritical() << "Failed to enumerate processes.";
        return false;
    }

    DWORD processCount = bytesReturned / sizeof(DWORD);

    for (DWORD i = 0; i < processCount; i++) {
        if (processIds[i] == 0) {
            continue;
        }

        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processIds[i]);

        if (hProcess) {
            TCHAR baseName[MAX_PATH];
            if (GetModuleBaseName(hProcess, NULL, baseName, sizeof(baseName) / sizeof(TCHAR))) {

                for (auto data : processNames)
                {
                    qDebug() << "search : " << QString::fromWCharArray(baseName);

                    if (data.compare(QString::fromWCharArray(baseName), Qt::CaseInsensitive) == 0) {
                        qDebug() << "find! : " << data;
                        CloseHandle(hProcess);
                        return true;
                    }
                }


            }
            CloseHandle(hProcess);
        }
    }

    return false;
}




VOID reportServiceStatus(DWORD currentState, DWORD win32ExitCode, DWORD waitHint) {
    static DWORD checkPoint = 1;

    g_serviceStatus.dwCurrentState = currentState;
    g_serviceStatus.dwWin32ExitCode = win32ExitCode;
    g_serviceStatus.dwWaitHint = waitHint;

    if (currentState == SERVICE_RUNNING || currentState == SERVICE_STOPPED) {
        g_serviceStatus.dwCheckPoint = 0;
    }
    else {
        g_serviceStatus.dwCheckPoint = checkPoint++;
    }

    SetServiceStatus(g_statusHandle, &g_serviceStatus);
}

VOID WINAPI serviceControlHandler(DWORD control) {
    if (control == SERVICE_CONTROL_STOP) {
        reportServiceStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);
        QCoreApplication::quit();
    }
    else {
        reportServiceStatus(g_serviceStatus.dwCurrentState, NO_ERROR, 0);
    }
}

VOID WINAPI serviceMain(DWORD, LPTSTR*) {
    g_statusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, serviceControlHandler);

    if (!g_statusHandle) {
        return;
    }

    g_serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_serviceStatus.dwServiceSpecificExitCode = 0;

    reportServiceStatus(SERVICE_START_PENDING, NO_ERROR, 3000);

    QCoreApplication* app = new QCoreApplication(__argc, __argv);

    reportServiceStatus(SERVICE_RUNNING, NO_ERROR, 0);

    //=program run
    // 
    // 
    // Launch the target program with administrator privileges
    
    QStringList ar;
    ar.append("N-KeepCleaneup.exe");
    ar.append("N-KeepCleaner.exe");
    if (!isProcessRunning(ar)) { //not run
        QProcess process;
        QString processpath = QString("%1/N-KeepCleane.exe").arg(qApp->applicationDirPath());
        process.setProgram(processpath);
        process.setArguments(QStringList() << " /hide");
        process.setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments* args) {
            args->flags |= CREATE_NEW_CONSOLE;
            args->startupInfo->dwFlags |= STARTF_USESHOWWINDOW;
            args->startupInfo->wShowWindow = SW_SHOW;
            });

        if (process.startDetached()) {
            qDebug() << "Target program started successfully.";
        }
        else {
            qCritical() << "Failed to start the target program.";
        }
    }
    //isProcessRunning


    //===

    app->exec();

    reportServiceStatus(SERVICE_STOPPED, NO_ERROR, 0);

    delete app;
}

int main(int argc, char* argv[]) {
    SERVICE_TABLE_ENTRY serviceTable[] = {
        {const_cast<LPWSTR>(SERVICE_NAME), static_cast<LPSERVICE_MAIN_FUNCTION>(serviceMain)},
        {NULL, NULL}
    };

    if (!StartServiceCtrlDispatcher(serviceTable)) {
        qCritical() << "Failed to run service";
    }

    return 0;
}
