#include "smartoptimizecontroller.h"

#include "../optimize/optimizecontroller.h"

#include "../../../QuickGrip_tmp.h"


SmartOptimizeController::SmartOptimizeController(QObject *parent)
	: QObject(parent), _usedata(false), _checker(Q_NULLPTR)
{
	_checker = new ReverveChecker(this);
	_checker->start();

	QObject::connect(SmartOptimizeInfo::inst(), &SmartOptimizeInfo::reserveUpdateComplete, [=]() {

		slotgetOptiReserveData();
		emit optimizeReserveSetAlert(false);
		});

	QObject::connect(SmartOptimizeInfo::inst(), &SmartOptimizeInfo::reserveDelComplete, [=]() {
		slotgetOptiReserveData();
		emit optimizeReserveSetAlert(true);
		});


	QObject::connect(this, SIGNAL(signalupdateOptiReserveData(int, ReserveData)), SmartOptimizeInfo::inst(), SLOT(setReserve(int, ReserveData)));
	QObject::connect(this, SIGNAL(signaldelOptiReserveData(QList<int>)), SmartOptimizeInfo::inst(), SLOT(delReserve(QList<int>)));

	QObject::connect(this, SIGNAL(signalAutoOptimize(bool)), OptimizeController::inst(), SLOT(hideOptimize(bool)));
	QObject::connect(this, SIGNAL(signalPcOFFOptimize()), OptimizeController::inst(), SLOT(pcoffOptimize()));
	QObject::connect(_checker, &ReverveChecker::checkOptimizeReserve, [=]() {
		//_checker->wait();
		emit signalAutoOptimize(false);
		});



	
}

SmartOptimizeController::~SmartOptimizeController()
{
	if (_checker) {
		//_checker->wait();
		_checker->terminate();
		_checker->deleteLater();
	}
}


bool SmartOptimizeController::isReserveData()
{
	_usedata = SmartOptimizeInfo::inst()->getUseData();
	return _usedata;
}

bool SmartOptimizeController::getReservePcOn()
{
	return SmartOptimizeInfo::inst()->isBootingOptimize();
}

bool SmartOptimizeController::getReservePcOff()
{
	return SmartOptimizeInfo::inst()->isPcOffOptimize();
}

void SmartOptimizeController::slotgetOptiReserveData()
{
	QMap<int, QString> viewData;
	viewData.clear();

	QMap<int, ReserveData>::iterator h = SmartOptimizeInfo::inst()->getFirstIter();
	while (h != SmartOptimizeInfo::inst()->getLastIter())
	{
		QString text;
		if (!h.value().use)
			text = "";
		else {

			const ReservType type = h.value().type;
			QString hourtext = QStringLiteral("%1").arg(h.value().hour, 2, 10, QLatin1Char('0'));
			QString mintext = QStringLiteral("%1").arg(h.value().min, 2, 10, QLatin1Char('0'));

			QString typetext = typeToText(type);
			if (!typetext.isEmpty()) {

				if (type == ReservType::PCON/* || type == ReservType::PCOFF*/)
					text = typetext;
				else if (type == ReservType::DAY)  //12 : 00
					text = QString("%1 %2 : %3").arg(typetext).arg(hourtext).arg(mintext);
				else if (type == ReservType::WEEK) 
					text = QString("%1(%2) %3 : %4").arg(typetext).arg(weekTypeToText((ReservWeekType)h.value().weektype))
					.arg(hourtext).arg(mintext);
				else if (type == ReservType::MONTH)
					text = QString("%1%2 %3 : %4").arg(typetext).arg(monthTypeToText((ReservMonthType)h.value().monthtype))
					.arg(hourtext).arg(mintext);

			}
		}
		viewData[h.key()] = text;
		h++;
	}

	signalsetOptiReserveData(viewData);
	
	bool usedata = SmartOptimizeInfo::inst()->getUseData();
	if (_usedata != usedata) {
		emit isUseData(usedata);
		_usedata = usedata;
	}
}

void SmartOptimizeController::slotsetOptiReservePcOnOff(bool pcon, bool use)
{
	ReserveData data;
	data.use = use;
	if (pcon)
		data.type = ReservType::PCON;
	/*else
		data.type = ReservType::PCOFF;*/

	emit signalupdateOptiReserveData(1, data);
}

void SmartOptimizeController::slotsetOptiReserve(int key, ReserveData data)
{
	emit signalupdateOptiReserveData(key, data);
}

void SmartOptimizeController::slotdelOptiReserve(QList<int> list)
{
	if (list.count() == 0)
		return;

	emit signaldelOptiReserveData(list);

}

void SmartOptimizeController::slotCheckPcStartOptimize()
{
	if (SmartOptimizeInfo::inst()->isBootingOptimize()) { //부팅시 최적화인 경우

		emit signalAutoOptimize(false);

	}
}

void SmartOptimizeController::slotCheckPcOffOptimize()
{
	if (SmartOptimizeInfo::inst()->isPcOffOptimize()) {

		//emit signalPcOFFOptimize();

		emit signalAutoOptimize(false);
	}
}