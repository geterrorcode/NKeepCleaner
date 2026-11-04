#pragma once

#include <list>

#include "plog/Log.h"
#include "plog/Severity.h"



enum LogLevel
{
    
    LOG_LEVEL_NONE = 1,

    LOG_LEVEL_ERROR = 2,

    LOG_LEVEL_INFO = 3,

    LOG_LEVEL_DEBUG = 4
};


#define LOG_TAB() "    "
#define LOG_EXCEPTION_FORMAT(msg) Util::format("exception [%s@%d] message - %s", PLOG_GET_FUNC(), __LINE__, msg).c_str()
#define LOG_MEGA_BYTE 1024*1024

namespace plog{

template<class Formatter> // Typically a formatter is passed as a template parameter.
class SaveAppender : public IAppender // All appenders MUST inherit IAppender interface.
{
public:
    virtual void write(const Record& record) // This is a method from IAppender that MUST be implemented.
    {
        util::nstring str = Formatter::format(record); // Use the formatter to get a string from a record.

        m_messageList.push_back(str); // Store a log message in a list.
    }

    std::list<util::nstring>& getMessageList()
    {
        return m_messageList;
    }

private:
    std::list<util::nstring> m_messageList;
};

class ReleaseFormatter
{
public:
    static util::nstring header() // This method returns a header for a new file. In our case it is empty.
    {
        return util::nstring();
    }

    static util::nstring format(const Record& record) // This method returns a string from a record.
    {
        tm t;
        bool useUtcTime = false;
        (useUtcTime ? util::gmtime_s : util::localtime_s)(&t, &record.getTime().time);

        util::nostringstream ss;
        ss << t.tm_year + 1900 << "-" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mon + 1 << PLOG_NSTR("-") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mday << PLOG_NSTR(" ");
        ss << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_hour << PLOG_NSTR(":") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_min << PLOG_NSTR(":") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_sec << PLOG_NSTR(".") << std::setfill(PLOG_NSTR('0')) << std::setw(3) << record.getTime().millitm << PLOG_NSTR(" ");
        ss << std::setfill(PLOG_NSTR(' ')) << std::setw(5) << std::left << severityToString(record.getSeverity()) << PLOG_NSTR(" ");
        ss << PLOG_NSTR("[") << record.getTid() << PLOG_NSTR("] ");
        ss << record.getMessage() << PLOG_NSTR("\n");

        return ss.str();
    }
};

}

namespace Log {

class Logger
{
public:
    Logger();
    ~Logger();

public:
    void Init(const LogLevel& level, const wchar_t* logPath = L"");

private:
    plog::Severity _generateSeverity(const LogLevel& level);
    std::wstring _defaultLogPath;
};

}
