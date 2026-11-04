#include "pingchecker.h"

#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#include <Logger.h>

namespace
{
QString pingOutputForLog(QString output)
{
	output.replace("\r\n", " | ");
	output.replace('\n', " | ");
	return output.left(1000);
}
}

PingChecker::PingChecker(QObject *parent)
	: QThread(parent), _mincheck(false)
{}

PingChecker::~PingChecker()
{
	if (isRunning()) {
		quit();
		wait(); // 스레드가 종료될 때까지 대기
	}
}

void PingChecker::run()
{
	pingCheck();
}

void PingChecker::pingCheck()
{
	const char* checkType = _mincheck ? "after(min)" : "before(max)";
	const char* pingTarget = "8.8.8.8";
	PLOG_INFO << "[PING_CHECK_START] target=" << pingTarget << ", count=5, type=" << checkType;

	QProcess pingProcess;
	//pingProcess.start("ping", QStringList() << "-n" << "5" << "google.com");
	//test
	///pingProcess.start("ping", QStringList() << "-n" << "5" << "www.youtube.com");
	pingProcess.start("ping", QStringList() << "-4" << "-n" << "5" << pingTarget);
	if (!pingProcess.waitForStarted(5000)) {
		PLOG_WARNING << "[PING_PROCESS_START_FAILED] type=" << checkType
			<< ", error=" << pingProcess.errorString();
		emit sendPingSpeed(_mincheck, pingExeptionControl());
		return;
	}

	pingProcess.waitForFinished(-1);


	QString pingResult = QString::fromLocal8Bit(pingProcess.readAll());
	const QString logOutput = pingOutputForLog(pingResult);

	if (pingProcess.exitStatus() != QProcess::NormalExit || pingProcess.exitCode() != 0) {
		PLOG_WARNING << "[PING_PROCESS_ABNORMAL_EXIT] type=" << checkType
			<< ", exitStatus=" << static_cast<int>(pingProcess.exitStatus())
			<< ", exitCode=" << pingProcess.exitCode()
			<< ", error=" << pingProcess.errorString()
			<< ", output=" << logOutput;
		emit sendPingSpeed(_mincheck, pingExeptionControl());
		return;
	}

	PLOG_INFO << "[PING_PARSE_START] type=" << checkType
		<< ", outputLength=" << pingResult.size();
	//qDebug() << "Result: " << pingResult;
	int startIndex = 0;
	int endIndex = 0;

	//int index = pingResult.lastIndexOf(":");
	//QString data;
	//QString str = pingResult.mid(index);
	//if (str.isEmpty()) {
	//	QStringList lines = pingResult.split("\n");
	//	
	//	if (lines.size() < 2) {
	//		data = pingResult.replace(" ", "");
	//		if (_mincheck) { //최소
	//			startIndex = 0;
	//			endIndex = data.indexOf(",");
	//		}
	//		else { //최대
	//			startIndex = data.indexOf(",") + 1;
	//			endIndex = data.lastIndexOf(",");
	//		}
	//	}
	//	else {
	//		QString speedLine = lines[lines.size() - 2];

	//		data = speedLine.replace(" ", "");
	//		if (_mincheck) { //최소
	//			startIndex = 0;
	//			endIndex = data.indexOf(",");
	//		}
	//		else { //최대
	//			startIndex = data.indexOf(",") + 1;
	//			endIndex = data.lastIndexOf(",");
	//		}
	//	}
	//}
	//else {
	//	data = str.replace(" ", "");
	//	if (_mincheck) { //최소
	//		startIndex = 0;
	//		endIndex = data.indexOf(",");
	//	}
	//	else { //최대
	//		startIndex = data.indexOf(",") + 1;
	//		endIndex = data.lastIndexOf(",");
	//	}
	//}


	QStringList lines = pingResult.split("\n");
	QString data;
	
	if (lines.size() < 2) {
		PLOG_WARNING << "[PING_OUTPUT_TOO_SHORT] type=" << checkType
			<< ", lineCount=" << lines.size()
			<< ", output=" << logOutput;

		emit sendPingSpeed(_mincheck, pingExeptionControl());

		return;
	}
	else {
		QString speedLine = lines[lines.size() - 2];
		QString buf;
		if (speedLine.indexOf(",") < 0) {
			buf = "、";
			if (speedLine.indexOf(buf) < 0) {
				PLOG_WARNING << "[PING_SUMMARY_DELIMITER_NOT_FOUND] type=" << checkType
					<< ", summary=" << speedLine;
			}
		}
		else {
			buf = ",";
		}


		data = speedLine.replace(" ", "");
		if (_mincheck) { //최소
			startIndex = 0;
			endIndex = data.indexOf(buf);
		}
		else { //최대
			startIndex = data.indexOf(buf) + 1;
			endIndex = data.lastIndexOf(buf);
		}
	}


	if (endIndex == 0 || endIndex < startIndex) {
		PLOG_WARNING << "[PING_VALUE_RANGE_INVALID] type=" << checkType
			<< ", startIndex=" << startIndex
			<< ", endIndex=" << endIndex
			<< ", parsedLine=" << data
			<< ", output=" << logOutput;

		emit sendPingSpeed(_mincheck, pingExeptionControl());
		return;
	}
		

	QString parsevalue = data.mid(startIndex, endIndex - startIndex);
	QString speedStr = parsevalue.mid(parsevalue.indexOf("=") + 1);

	bool converted = false;
	int ping = QString(speedStr.replace("ms", "")).toInt(&converted);
	if (!converted || ping <= 0) {
		PLOG_WARNING << "[PING_VALUE_PARSE_FAILED] type=" << checkType
			<< ", parseValue=" << parsevalue
			<< ", speedText=" << speedStr
			<< ", converted=" << converted
			<< ", output=" << logOutput;
		ping = pingExeptionControl();
	}
	else {
		PLOG_INFO << "[PING_PARSE_SUCCESS] type=" << checkType
			<< ", pingMs=" << ping
			<< ", parseValue=" << parsevalue;
	}

	//qDebug() << speedStr;
	emit sendPingSpeed(_mincheck, ping);
	//if (_mincheck) {
	//	//BoostPingController::inst()->lastpingcheck();
	//	emit LastPingCheckEnd(); //view 에 통지
	//}
}

void PingChecker::setMinCheck(bool min)
{
	_mincheck = min;
}


int PingChecker::pingExeptionControl()
{
	QDateTime currentTime = QDateTime::currentDateTime();
	int sec = currentTime.time().second();
	QString resstring = QString("1%1").arg(QStringLiteral("%1").arg(sec, 2, 10, QLatin1Char('0')));

	const int fallbackPing = resstring.toInt();
	PLOG_WARNING << "[PING_FALLBACK_VALUE_USED] generatedPingMs=" << fallbackPing
		<< ", type=" << (_mincheck ? "after(min)" : "before(max)");

	return fallbackPing;
}
