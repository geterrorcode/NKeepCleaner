#pragma once

#include "action.h"

#include <QObject>

namespace Qux{
	namespace Redux
	{
		class State
		{

		public:
			State() {};
			void setter(uint _key, QVariant _value) { state_map_[_key] = _value; };
			QVariant& getter(uint _key) { return state_map_[_key]; }
			void update(uint _key, QVariant _value) { state_map_[_key] = _value; }
			void deleter(uint _key) { state_map_.remove(_key); }

		protected:
			QMap<uint, QVariant> state_map_;

		};
	}
}



