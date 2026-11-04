#pragma once


#include "store.h"


namespace Qux {
	namespace Redux {
		class Reducer : public QObject
		{
			Q_OBJECT
		public:
			Reducer() {
				connect(&Store::inst(), &Store::signalSendReducer, this, &Reducer::slotHandleAction);
				connect(this, &Reducer::signalChangeState, &Store::inst(), &Store::slotChangeState);

			};
			~Reducer() {};

		Q_SIGNALS:
			void signalChangeState(State _new);

		public Q_SLOTS:
			virtual void  slotHandleAction(Action _action, State _prev) { emit signalChangeState(_prev); }
		};
	}
}