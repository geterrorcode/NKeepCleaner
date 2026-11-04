#pragma once

#include <Windows.h>
#include <QObject>


enum class ReservType
{
	NONE = 0,
	PCON,
	PCOFF,
	DAY,
	WEEK,
	MONTH
};

enum class ReservWeekType
{
	NONE = 0,
	MON,
	TUES,
	WEDNES,
	THURS,
	FRI,
	SAT,
	SUN
};

enum class ReservMonthType
{
	NONE = 0,
	FIRST,
	FIFTEEN,
	THIRTY
};

enum class LastClieckType
{
	NONE = 0,
	MON,
	TUES,
	WEDNES,
	THURS,
	FRI,
	SAT,
	SUN,
	FIRST,
	FIFTEEN,
	THIRTY
};


static QString typeToText(ReservType type) {
	if (type == ReservType::PCON)
		return "N-Keepcleaner起動時最適化";
	//else if (type == ReservType::PCOFF)
	//	return "PC終了時最適化";
	else if (type == ReservType::DAY)
		return "毎日";
	else if (type == ReservType::WEEK)
		return "毎週";
	else if (type == ReservType::MONTH)
		return "毎月";
	else return "";
}

static QString weekTypeToText(ReservWeekType type) {
	if (type == ReservWeekType::MON)
		return "月";
	else if (type == ReservWeekType::TUES)
		return "火";
	else if (type == ReservWeekType::WEDNES)
		return "水";
	else if (type == ReservWeekType::THURS)
		return "木";
	else if (type == ReservWeekType::FRI)
		return "金";
	else if (type == ReservWeekType::SAT)
		return "土";
	else if (type == ReservWeekType::SUN)	
		return "日";
	else return "";
}

static QString monthTypeToText(ReservMonthType type) {
	if (type == ReservMonthType::FIRST)
		return "1日";
	else if (type == ReservMonthType::FIFTEEN)
		return "15日";
	else if (type == ReservMonthType::THIRTY)
		return "最終日";//毎月最終日
	else return "";
}

struct ReserveData {
	ReservType type;
	ReservWeekType weektype;
	ReservMonthType monthtype;

	int hour;
	int min;

	bool use;
	
	ReserveData()
	{
		type = ReservType::NONE;
		weektype = ReservWeekType::NONE;
		monthtype = ReservMonthType::NONE;
		hour = 0;
		min = 0;
		use = false;
	}

};