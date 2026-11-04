#include "boostpingmanager.h"

#include <windows.h>
#include <QSettings>
#include <QTimer>
#include <QEventLoop>
#include <QDnsLookup>

#include "../../controller/boostping/boostpingcontroller.h"
#include "../../common/tool/utils.h"

BoostPingManager::BoostPingManager(QObject *parent)
	: QObject(parent)
{
	//QObject::connect(this, SIGNAL(signalInternetConnect()), this, SLOT(slotInternetConnect()));
	//QObject::connect(this, SIGNAL(signalInternetDisConnect()), this, SLOT(slotInternetDisConnect()));
	//_pingchecker = new PingChecker(this);


}

BoostPingManager::~BoostPingManager()
{}

void BoostPingManager::setBoostPing()
{
	setNodelay();
	setTCPNoAckFrequency();
	setMSMQsetup();
	setBandWidth();

	utils::networkControl(false);
	checkNetworkConnect();
}

void BoostPingManager::setNodelay(bool unuse)
{
	QSettings nodelay_reg(REGKEY_NODELAY, QSettings::NativeFormat);

	int value = 20;
	if (unuse == true)
		value = 0;

	nodelay_reg.setValue("TCPNoDelay", value);
	nodelay_reg.sync();
}

void BoostPingManager::setTCPNoAckFrequency(bool unuse)
{
	HKEY hkey;
	DWORD enumindex = 0;
	TCHAR subkey[MAX_PATH];
	::ZeroMemory(subkey, MAX_PATH);
	DWORD buffersize = MAX_PATH;

	LONG result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, REGKEY_TCPACK, 0, KEY_READ | KEY_ENUMERATE_SUB_KEYS, &hkey);
	if (result != ERROR_SUCCESS)
		return;

	int value = 1;
	if (unuse == true)
		value = 2;

	while (::RegEnumKey(hkey, enumindex++, subkey, buffersize) != ERROR_NO_MORE_ITEMS) {

		QString reg_path = QString("HKEY_LOCAL_MACHINE\\%2\\").arg(REGKEY_TCPACK);
		QString key_name = QString("%1").arg((LPWSTR)subkey);
		QSettings tcpnoack_reg(reg_path + key_name, QSettings::NativeFormat);

		if (tcpnoack_reg.value("IPAddress").toString().isEmpty())
			continue;

		//TcpNoAckFrequency
		tcpnoack_reg.setValue("TcpAckFrequency", value);

		//TcpDelAckTicks
		if (unuse)  //delete
			tcpnoack_reg.remove("TcpDelAckTicks");
		else
			tcpnoack_reg.setValue("TcpDelAckTicks", 0);
	
		tcpnoack_reg.sync();
	}

	RegCloseKey(hkey);
}

void BoostPingManager::setMSMQsetup()
{
	QSettings msmq_reg(REGKEY_MSMQ, QSettings::NativeFormat);
	QStringList childkeys = msmq_reg.childKeys();

	//OCMsetup
	QString check_subkey = "Parameters\\OCMsetup";
	if (!childkeys.contains(check_subkey, Qt::CaseInsensitive))
	{
		QString subreg = QString("%1\\%2").arg(REGKEY_MSMQ, check_subkey);
		QSettings sub(subreg, QSettings::NativeFormat);
		QStringList keys = sub.childKeys();
	}


	//param setup
	check_subkey = "Parameters\\setup";
	if (!childkeys.contains(check_subkey, Qt::CaseInsensitive))
	{
		QString subreg = QString("%1\\%2").arg(REGKEY_MSMQ, check_subkey);
		QSettings sub(subreg, QSettings::NativeFormat);
		QStringList keys = sub.childKeys();
	}

	//Parameters Security
	check_subkey = "Parameters\\Security";
	if (!childkeys.contains(check_subkey, Qt::CaseInsensitive))
	{
		QString subreg = QString("%1\\%2").arg(REGKEY_MSMQ, check_subkey);
		QSettings sub(subreg, QSettings::NativeFormat);
		//QStringList keys = sub.childKeys();
		sub.setValue("SecureDSCommunication", 0);
	}

	//MSMQ Setup
	check_subkey = "Setup";
	if (!childkeys.contains(check_subkey, Qt::CaseInsensitive))
	{
		QString subreg = QString("%1\\%2").arg(REGKEY_MSMQ, check_subkey);
		QSettings sub(subreg, QSettings::NativeFormat);
		QStringList keys = sub.childKeys();
	}

}

//대역폭 해제
void BoostPingManager::setBandWidth(bool unuse)
{
	int value = 70;
	if (unuse)
		value = 10;

	QSettings nodelay_reg(REGKEY_SYSTEMPROFILE, QSettings::NativeFormat);
	nodelay_reg.setValue("NetworkThrottlingInde", value);
	nodelay_reg.sync();
}

void BoostPingManager::setInternetOn()
{
	QTimer::singleShot(100, [=]() {
		utils::networkControl(true);
		});

	
	auto timer = new QTimer(this);
	timer->setInterval(300);
	timer->setTimerType(Qt::VeryCoarseTimer);
	connect(timer, &QTimer::timeout, this, [=]()
		{
			if (checkNetworkConnect() == true) {
				timer->stop();
				emit signalInternetConnect();
			}
		});
	timer->start();

	//checkNetworkConnect();
}

void BoostPingManager::setInternetOff()
{
	QTimer::singleShot(100, [=]() {
		utils::networkControl(false);
		});

	auto timer = new QTimer(this);
	timer->setInterval(300);
	timer->setTimerType(Qt::VeryCoarseTimer);
	connect(timer, &QTimer::timeout, this, [=]()
		{
			if (checkNetworkConnect() == false) {
				timer->stop();
				emit signalInternetDisConnect();
			}
		});
	timer->start();

	
}

bool BoostPingManager::checkNetworkConnect()
{
	QTimer timer;
	timer.setInterval(3000);
	timer.setSingleShot(true);
	QEventLoop eventLoop;
	QObject::connect(&timer, &QTimer::timeout, &eventLoop, &QEventLoop::quit);
	QDnsLookup lookup;
	QObject::connect(&lookup, &QDnsLookup::finished, &eventLoop, &QEventLoop::quit);
	lookup.setType(QDnsLookup::A);
	lookup.setName("dns.google.com");
	lookup.lookup();
	timer.start();
	eventLoop.exec();

	if (timer.remainingTime() == 0)
	{
		lookup.abort();
		//emit signalInternetDisConnect();
		return false;
	}
	if (lookup.error() != QDnsLookup::NoError) {
		//emit signalInternetDisConnect();
		return false;
	}

	//emit signalInternetConnect();
	return true;
}


//void BoostPingManager::slotInternetConnect()
//{
//	qDebug() << "Internet On Connect!";
//}
//
//void BoostPingManager::slotInternetDisConnect()
//{
//	qDebug() << "Internet Dis Connect!";
//	utils::networkControl(true);
//	checkNetworkConnect();
//}
