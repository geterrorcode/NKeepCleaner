#include "autopcoffchecker.h"

AutoPcOffChecker::AutoPcOffChecker(QObject *parent)
	: QThread(parent)
{}

AutoPcOffChecker::~AutoPcOffChecker()
{
	if (isRunning()) {
		quit();
		wait(); // 스레드가 종료될 때까지 대기
	}
}



void AutoPcOffChecker::run()
{
	while (true)
	{
		mutex.lock();
		bool res = checkSchedule();
		mutex.unlock();

		/*if (res == true) {
			emit checkOptimizeReserve();
			return;
		}*/

		QThread::sleep(60);
	}
}

bool AutoPcOffChecker::checkSchedule()
{
	////QDateTime currentDateTime = QDateTime::currentDateTime();
	////QString currentMonthString = currentDateTime.toString("MMMM");
	////int index = currentDateTime.date().month();
	////qDebug() << "Thread : " << currentMonthString;
	////월욜1 일욜7

	//QMap<int, ReserveData>::iterator h = SmartOptimizeInfo::inst()->getFirstIter();
	//while (h != SmartOptimizeInfo::inst()->getLastIter())
	//{
	//	if (h.value().use) {

	//		QTime currentTime = QTime::currentTime();
	//		QString now = currentTime.toString("hh:mm");
	//		QString reservetime = QString("%1:%2").arg(h.value().hour).arg(h.value().min);

	//		if (h.value().type == ReservType::DAY) {
	//			if (reservetime == now) {
	//				return true;
	//			}

	//		}
	//		else if (h.value().type == ReservType::WEEK) {
	//			QDateTime currentDateTime = QDateTime::currentDateTime();
	//			int currentDayNumber = currentDateTime.date().dayOfWeek();
	//			if ((int)h.value().weektype == currentDayNumber && reservetime == now) {

	//				return true;
	//			}

	//		}
	//		else if (h.value().type == ReservType::MONTH) {
	//			int checkday = 0;
	//			if (h.value().monthtype == ReservMonthType::FIRST)
	//				checkday = 1;
	//			else if (h.value().monthtype == ReservMonthType::FIFTEEN)
	//				checkday = 15;
	//			else if (h.value().monthtype == ReservMonthType::THIRTY)
	//				checkday = 30;
	//			else break;

	//			QDateTime currentDateTime = QDateTime::currentDateTime();
	//			if (checkday == currentDateTime.date().month() && reservetime == now) {

	//				return true;
	//			}
	//		}

	//	}

	//	h++;
	//}

	return false;
}