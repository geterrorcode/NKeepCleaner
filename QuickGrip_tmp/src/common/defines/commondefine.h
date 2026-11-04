#ifndef COMMONDEFINE_H
#define COMMONDEFINE_H


#include <QString>
#include <QMap>

enum class viewIndex
{
	none = 0,
	optimize = 1,
	smartoptimize,
	boostping,
	autopcoff,
	startup,
	appwiz,
	pcspec,
	optins,
	as,
	account
};

constexpr auto MAIN_WIDTH = 1008;
constexpr auto MAIN_HEIGTH = 672;


const QString AUTHSERVER_URL = "https://prod.tniauth.com";
//const QString AUTHSERVER_URL = "https://dev.tniauth.com";
const QString AUTHSERVER_DEV_URL = "https://dev.tniauth.com";


enum class AuthResType
{
	ok = 0,				/*Success*/
	notfindkey = 1,		/*Param Error*/
	serialerror,		/*Serial Key Error*/
	expiredkey,			/*Expired Serial Key*/
	uuiderror,			/*UUID error*/
	etc
};

AuthResType resCodeToRestype(const QString& str);

struct OptimizeListInfo
{
	uint optimizeType;
	uint optimizeDetailType;
	QString hintText;

	/*OptimizeListInfo() {
		optimizeType = optimizeDetailType = 0;
		hintText = "";
	}*/
};

#endif // COMMONDEFINE_H