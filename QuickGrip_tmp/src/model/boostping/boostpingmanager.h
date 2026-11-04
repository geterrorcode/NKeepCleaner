#pragma once

#include <QObject>

constexpr auto REGKEY_NODELAY = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\MSMQ\\Parameters";
constexpr auto REGKEY_TCPACK = L"SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces\\";
constexpr auto REGKEY_MSMQ = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\MSMQ";
constexpr auto REGKEY_SYSTEMPROFILE = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Multimedia\\SystemProfile";



#include "../../common/ux/singleton.h"


class BoostPingManager  : public QObject,
	public common::ux::Singleton<BoostPingManager>
{
	Q_OBJECT

public:
	BoostPingManager(QObject *parent = Q_NULLPTR);
	~BoostPingManager();

	void setBoostPing();
	
	void setNodelay(bool unuse = false);
	void setTCPNoAckFrequency(bool unuse = false);
	void setMSMQsetup();
	void setBandWidth(bool unuse = false);
	void setInternetOn();
	void setInternetOff();

private:
	bool checkNetworkConnect();

signals:
	void signalInternetConnect();
	void signalInternetDisConnect();



private Q_SLOTS:
	//void slotInternetConnect();
	//void slotInternetDisConnect();
};
