#pragma once

#include <QObject>


enum class StartUpType
{
	regType = 0,
	fileType = 1
};

struct StartUpData {

	QString namne;
	QString file_path;
	StartUpType type;
	QString regPath;
	int filemake_date;

	StartUpData()
	{
		namne = "";
		file_path = "";
		type = StartUpType::regType;
		regPath = "";
		filemake_date = 0;
	}

};

//bool operator<(const StartUpData& a, const StartUpData& b) { return a.filemake_date < b.filemake_date; }