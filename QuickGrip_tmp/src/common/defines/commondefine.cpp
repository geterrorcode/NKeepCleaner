#include "commondefine.h"


AuthResType resCodeToRestype(const QString& str) {
	static const QMap<QString, AuthResType> map = {
		{"000", AuthResType::ok},
		{"001", AuthResType::notfindkey},
		{"003", AuthResType::serialerror},
		{"004", AuthResType::expiredkey},
		{"009", AuthResType::uuiderror}
	};

	return map.value(str, AuthResType::etc);
}