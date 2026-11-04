#pragma once


#include "action.h"
#include "store.h"
#include "state.h"
#include <QVariant>


#define RDX_ACTION( ACTION_TYPE)\
	Qux::Redux::Action action(ACTION_TYPE);\
	Store::inst().dispatch(action);

#define RDX_ACTION_VALUE(ACTION_TYPE, TYPE, VALUE)\
	const TYPE _value = VALUE;\
	Qux::Redux::Action action(ACTION_TYPE, QVariant::fromValue(_value));\
	Store::inst().dispatch(action);


#define RDX_ACTION_KEY_VALUE(ACTION_TYPE, TYPE1, KEY, TYPE2, VALUE)\
	const TYPE1 _key = KEY;\
	const TYPE2 _value = VALUE;\
	QList<QVariant> key_value;\
	key_value.append(QVariant::fromValue(_key));key_value.append(QVariant::fromValue(_value));\
	Qux::Redux::Action action(ACTION_TYPE, QVariant::fromValue(key_value));\
	Store::inst().dispatch(action);

