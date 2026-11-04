#pragma once

#include <QApplication>
#include <QCoreApplication>
#include <QScreen>
#include <QPointer>
#include <QSingleApplication/SingleApplication>

#include "QuickGrip_tmp.h"

#define qApplication (static_cast<QApp*>(QCoreApplication::instance()))

class QApp : public SingleApplication
{
	Q_OBJECT

public:
	QApp(int& argc, char** argv);
	~QApp();

private:
	QPointer<QuickGrip_tmp> _mainWindow;

};

