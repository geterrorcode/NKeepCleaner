#include "Logger.h"

#include <fstream>
#include <iostream>
#include <experimental/filesystem>


namespace fs = std::experimental::filesystem;
using namespace Log;

Logger::Logger()
    : _defaultLogPath((fs::current_path() / L"default.log"))
{
}

Logger::~Logger()
{

}

void Logger::Init(const LogLevel& level, const wchar_t* logPath /*= "" */)
{
    std::wstring logPathString(logPath);
    if (logPathString.empty() || logPathString == std::wstring(L""))
    {
        logPathString = _defaultLogPath;
    }
    else
    {
        logPathString = logPath;
    }
    
#ifdef _DEBUG
    static plog::RollingFileAppender<plog::TxtFormatter> fileAppender(logPathString.c_str(), LOG_MEGA_BYTE, 3);
#else
    static plog::RollingFileAppender<plog::ReleaseFormatter> fileAppender(logPathString.c_str(), LOG_MEGA_BYTE, 3);
#endif
    static bool onceAppender = true;
    plog::Severity severity = _generateSeverity(level);
    if (onceAppender)
    {
        plog::init(severity, &fileAppender);
        onceAppender = false;
    }
    plog::get()->setMaxSeverity(severity);

    PLOG_INFO << "file Appender.(level: " << plog::severityToString(severity) << ")";
}

plog::Severity Log::Logger::_generateSeverity(const LogLevel& level)
{
    switch (level)
    {
    case LOG_LEVEL_NONE:
        return plog::Severity::none;
        break;

    case LOG_LEVEL_ERROR:
        return plog::Severity::error;
        break;
    case LOG_LEVEL_INFO:
        return plog::Severity::info;
        break;

    case LOG_LEVEL_DEBUG:
        return plog::Severity::debug;
        break;

    default:
        return plog::Severity::none;
        break;
    }
}

