#include "revervechecker.h"

#include <QTime>
#include <QDateTime>

#include "smartoptimizeinfo.h"
#include "smartopimizedata.h"

#include <controller/smart_optimize/smartoptimizecontroller.h>


ReverveChecker::ReverveChecker(QObject *parent)
	: QThread(parent)
{

}

ReverveChecker::~ReverveChecker()
{
	if (isRunning()) {
		quit();
		wait(); // 스레드가 종료될 때까지 대기
	}
}


void ReverveChecker::run()
{
	while (true)
	{
		mutex.lock();
		bool res = checkReserve();
		mutex.unlock();

		if (res == true) {
			emit checkOptimizeReserve();
			return;
		}

		QThread::sleep(60);
	}
}

bool ReverveChecker::checkReserve()
{
	//QDateTime currentDateTime = QDateTime::currentDateTime();
	//QString currentMonthString = currentDateTime.toString("MMMM");
	//int index = currentDateTime.date().month();
	//qDebug() << "Thread : " << currentMonthString;
	//월욜1 일욜7

	QMap<int, ReserveData>::iterator h = SmartOptimizeInfo::inst()->getFirstIter();
	while (h != SmartOptimizeInfo::inst()->getLastIter())
	{
		if (h.value().use) {

			ReservType reserveType = h.value().type;
			if (reserveType == ReservType::PCON /*|| reserveType == ReservType::PCOFF*/) {
				h++;
				continue;
			}
				

			QTime currentTime = QTime::currentTime();
			QString now = currentTime.toString("hh:mm");
			//QString reservetime = QString("%1:%2").arg(h.value().hour).arg(h.value().min);
			QString reservetime =QString("%1:%2").arg(QStringLiteral("%1").arg(h.value().hour, 2, 10, QLatin1Char('0'))).arg(QStringLiteral("%1").arg(h.value().min, 2, 10, QLatin1Char('0')));

			qDebug() << "now : " << now << ",reserveTime : " << reservetime;
			if (reserveType == ReservType::DAY) {
				if (reservetime == now) {
					return true;
				}

			}
			else if (reserveType == ReservType::WEEK) {
				QDateTime currentDateTime = QDateTime::currentDateTime();
				int currentDayNumber = currentDateTime.date().dayOfWeek();
				int daytype = (int)h.value().weektype;
				if (daytype == currentDayNumber && reservetime == now) {

					return true;
				}

			}
			else if (reserveType == ReservType::MONTH) {
				int checkday = 0;
				if (h.value().monthtype == ReservMonthType::FIRST)
					checkday = 1;
				else if (h.value().monthtype == ReservMonthType::FIFTEEN)
					checkday = 15;
				else if (h.value().monthtype == ReservMonthType::THIRTY) {
					QDate currentDate = QDate::currentDate();
					QDate lastDayOfMonth = currentDate.addMonths(1).addDays(-currentDate.day());
					checkday = lastDayOfMonth.day();
					//checkday = 30;
				}
				else break;

				QDateTime currentDateTime = QDateTime::currentDateTime();
				if (checkday == currentDateTime.date().day() && reservetime == now) {

					return true;
				}
			}

		}

		h++;
	}

	return false;
}