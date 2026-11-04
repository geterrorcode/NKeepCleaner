#pragma once

#include "state.h"


namespace Qux {
	namespace Redux
	{
		class ISubscriber
		{
		public:
			virtual void changeState(State& _new_state) = 0;
		};
	}
}

