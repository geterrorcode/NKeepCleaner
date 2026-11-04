#pragma once

#include <Windows.h>
#include <QObject>

struct AppWizData {
	HKEY hmainkey;
	QString keyname;

	QString program_name;
	QString iconpath;
	QString publisher;
	QString size;
	QString install_date;
	QString version;
	QString uninstall_str;

	AppWizData()
	{
		hmainkey = HKEY_LOCAL_MACHINE;
		keyname = "";
		program_name = "";
		iconpath = "";
		publisher = "";
		size = "";
		install_date = "";
		version = "";
		uninstall_str = "";
	}

};