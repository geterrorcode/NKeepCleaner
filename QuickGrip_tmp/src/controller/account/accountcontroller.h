#pragma once

#include <QObject>

#include "../../../QuickGrip_tmp.h"

#include <view/account/accountview.h>

#include "../../common/ux/singleton.h"



class AccountController : public QObject, public common::ux::Singleton<AccountController>
{
	Q_OBJECT

public:
	AccountController(QObject* parent = Q_NULLPTR);
	~AccountController();

	//void expiredDateLabelUpdate();
	void getPayMentURL();
	void getExPayMentURL();
	void TerminateAutoPay();

Q_SIGNALS:
	void expiredDateUpdate();

	void AuthInfoUpdate();

	void AuthDetailComplete();
	
	void AutoPayTerminateComplete(const QString res_msg, const int resCode);
	
	void OpenPayMentURL(const QString url, const int rescode);
	void OpenExPayMentURL(const QString url, const int rescode);

	
	
};
