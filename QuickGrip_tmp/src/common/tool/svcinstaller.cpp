#include "svcinstaller.h"
#include <QString>
#include <QDebug>
#include "windows.h"

SvcInstaller::SvcInstaller(QObject *parent)
	: QObject(parent)
{}

SvcInstaller::~SvcInstaller()
{}

bool SvcInstaller::isServiceInstalled(const QString& svcname)
{
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCManager) {
        qCritical() << "Failed to open service control manager.";
        return false;
    }

    SC_HANDLE hService = OpenService(hSCManager, reinterpret_cast<LPCWSTR>(svcname.utf16()), SERVICE_QUERY_CONFIG);
    if (!hService) {
        CloseServiceHandle(hSCManager);
        return false;
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
    return true;
}

bool SvcInstaller::isServiceRunning(const QString& svcname)
{
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCManager) {
        qCritical() << "Failed to open service control manager.";
        return false;
    }

    SC_HANDLE hService = OpenService(hSCManager, reinterpret_cast<LPCWSTR>(svcname.utf16()), SERVICE_QUERY_STATUS);
    if (!hService) {
        qCritical() << "Failed to open service.";
        CloseServiceHandle(hSCManager);
        return false;
    }

    SERVICE_STATUS_PROCESS serviceStatus;
    DWORD bytesNeeded;
    if (!QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO, reinterpret_cast<LPBYTE>(&serviceStatus), sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded)) {
        qCritical() << "Failed to query service status.";
        CloseServiceHandle(hService);
        CloseServiceHandle(hSCManager);
        return false;
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);

    return serviceStatus.dwCurrentState == SERVICE_RUNNING;
}

bool SvcInstaller::installService(const QString& svcname, const QString& scvpath)
{
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
    if (!hSCManager) {
        qCritical() << "Failed to open service control manager.";
        return false;
    }

    SC_HANDLE hService = CreateService(
        hSCManager,
        reinterpret_cast<LPCWSTR>(svcname.utf16()),
        reinterpret_cast<LPCWSTR>(svcname.utf16()),
        SERVICE_ALL_ACCESS,
        SERVICE_WIN32_OWN_PROCESS,
        SERVICE_AUTO_START, // Set the start type to Automatic
        SERVICE_ERROR_NORMAL,
        reinterpret_cast<LPCWSTR>(scvpath.utf16()),
        NULL,
        NULL,
        NULL,
        NULL,
        NULL);

    if (!hService) {
        DWORD error = GetLastError();
        qCritical() << "Failed to create service. Error:" << error;
        CloseServiceHandle(hSCManager);
        return false;
    }

    qDebug() << "Service installed successfully.";
    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
    return true;
}

bool SvcInstaller::startService(const QString& svcname)
{
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCManager) {
        qCritical() << "Failed to open service control manager.";
        return false;
    }

    SC_HANDLE hService = OpenService(hSCManager, reinterpret_cast<LPCWSTR>(svcname.utf16()), SERVICE_START);
    if (!hService) {
        qCritical() << "Failed to open service.";
        CloseServiceHandle(hSCManager);
        return false;
    }

    if (!StartService(hService, 0, NULL)) {
        DWORD err = GetLastError();
        qCritical() << "Failed to start service." << " : " << GetLastError();


        CloseServiceHandle(hService);
        CloseServiceHandle(hSCManager);
        return false;
    }

    qDebug() << "Service started successfully.";
    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
    return true;
}

bool SvcInstaller::deleteService(const QString& svcname)
{
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCManager) {
        qCritical() << "Failed to open service control manager.";
        return false;
    }

    SC_HANDLE hService = OpenService(hSCManager, reinterpret_cast<LPCWSTR>(svcname.utf16()), DELETE);
    if (!hService) {
        qCritical() << "Failed to open service.";
        CloseServiceHandle(hSCManager);
        return false;
    }

    if (!DeleteService(hService)) {
        qCritical() << "Failed to delete service.";
        CloseServiceHandle(hService);
        CloseServiceHandle(hSCManager);
        return false;
    }

    qDebug() << "Service deleted successfully.";
    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
    return true;
}