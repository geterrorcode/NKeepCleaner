#pragma once

#include <QVariant>

class ActionType
{
public:
	const static uint ACTION_TYPE_FAIL = -1;
};

namespace Qux {
	namespace Redux {
		class Action
		{
		public:
			Action() {};
			Action(uint _type) : type_(_type) {}
			Action(uint _type, QVariant _pay_load) : type_(_type), pay_load_(_pay_load) {}

		public:
			QVariant getKey()
			{
				if (pay_load_.canConvert(QVariant::List)) {
					return pay_load_.toList().at(0);
				}
				else {
					return QString("");
				}
			}
			QVariant getValue()
			{
				if (pay_load_.canConvert(QVariant::List)) {
					return pay_load_.toList().at(1);
				}
				else {
					return pay_load_;
				}
			}
			QVariant& getPayload() { return pay_load_; }
			const uint& getActionType()	{ return type_; }
		
		protected:
			uint type_;
			QVariant pay_load_;
		};
	}
}

