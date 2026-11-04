#include "loginstance.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <Windows.h>


LogInstance::LogInstance(QObject *parent)
	: QObject(parent)
	, _gLogger(std::make_unique<Log::Logger>())
{}

LogInstance::~LogInstance()
{}

LogInstance& LogInstance::Inst()
{
	static LogInstance* loginstance = new LogInstance;
	return *loginstance;
}


void LogInstance::Initialize()
{
    // 현재 디렉토리 설정 
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    // 로그 설정
    QDir dir;
    QDateTime curDate = QDateTime::currentDateTime();
    QString date = curDate.toString("yyyyMMdd");
    QString FolderName = QString("Log") + QDir::separator() + date;

    dir.mkpath(FolderName);

    QString FolderPath = dir.absoluteFilePath(FolderName);
    QString logPath = FolderPath + QDir::separator() + "N-KeepCleaner.log";
    logPath = QDir::toNativeSeparators(logPath);

    try
    {//reinterpret_cast<const wchar_t*>(logPath.utf16())
        _gLogger->Init(LOG_LEVEL_INFO, reinterpret_cast<const wchar_t*>(logPath.utf16()));
        //_gLogger->Init(LOG_LEVEL_INFO, logPath.toStdWString().c_str());
    }
    catch (const std::exception& e)
    {
        int errorCode = GetLastError();
        qDebug() << "Exception caught: " << e.what() << ", Error code: " << errorCode;
    }

    



}
