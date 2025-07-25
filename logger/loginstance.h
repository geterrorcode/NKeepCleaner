#pragma once

#include <QObject>

#include "Logger.h"

class LogInstance  : public QObject
{
	Q_OBJECT

public:
	static LogInstance& Inst();

public:
	LogInstance(QObject *parent = nullptr);
	~LogInstance();

	void Initialize();

private:
	std::unique_ptr<Log::Logger> _gLogger;
};
