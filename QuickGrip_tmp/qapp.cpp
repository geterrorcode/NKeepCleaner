#include "qapp.h"

#include "QuickGrip_tmp.h"

QApp::QApp(int& argc, char** argv)
	: SingleApplication(argc, argv)
	, _mainWindow(Q_NULLPTR)
{
	try
	{
		if (!_mainWindow) {
			_mainWindow = new QuickGrip_tmp();
		}
		

		_mainWindow->show();
	}
	catch (const std::exception&)
	{

	}
}

QApp::~QApp()
{
	if (_mainWindow.isNull() == false)
	{
		delete _mainWindow;
	}
}