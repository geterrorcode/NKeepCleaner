#ifndef QUX_FLUX_ACTIONHELPER_H
#define QUX_FLUX_ACTIONHELPER_H

#include "action.h"

namespace Qux {
namespace Flux {

void dispatch(const Action& action);

}
}

#define ACTION0(FUNCTION, ACTIONTYPE)                                               \
	inline void FUNCTION() {                                                        \
        Qux::Flux::Action action; action["type"] = QVariant::fromValue(ACTIONTYPE); \
        Qux::Flux::dispatch(action); }

#define ACTION1(FUNCTION, ACTIONTYPE, TYPE0, NAME0)                                 \
	inline void FUNCTION(TYPE0 const & NAME0) {                                     \
        Qux::Flux::Action action; action["type"] = QVariant::fromValue(ACTIONTYPE); \
        action[#NAME0] = QVariant::fromValue(NAME0);                                \
		Qux::Flux::dispatch(action); }

#define ACTION2(FUNCTION, ACTIONTYPE, TYPE0, NAME0, TYPE1, NAME1)                                   \
	inline void FUNCTION(TYPE0 const & NAME0, TYPE1 const & NAME1) {                                \
		Qux::Flux::Action action; action["type"] = QVariant::fromValue(ACTIONTYPE);                 \
        action[#NAME0] = QVariant::fromValue(NAME0); action[#NAME1] = QVariant::fromValue(NAME1);   \
		Qux::Flux::dispatch(action); }

#define ACTION3(FUNCTION, ACTIONTYPE, TYPE0, NAME0, TYPE1, NAME1, TYPE2, NAME2)                     \
	inline void FUNCTION(TYPE0 const & NAME0, TYPE1 const & NAME1, TYPE2 const & NAME2) {           \
		Qux::Flux::Action action; action["type"] = QVariant::fromValue(ACTIONTYPE);                 \
        action[#NAME0] = QVariant::fromValue(NAME0); action[#NAME1] = QVariant::fromValue(NAME1);   \
        action[#NAME2] = QVariant::fromValue(NAME2);                                                \
		Qux::Flux::dispatch(action); }

#define ACTION4(FUNCTION, ACTIONTYPE, TYPE0, NAME0, TYPE1, NAME1, TYPE2, NAME2, TYPE3, NAME3)                   \
	inline void FUNCTION(TYPE0 const & NAME0, TYPE1 const & NAME1, TYPE2 const & NAME2, TYPE3 const & NAME3) {  \
		Qux::Flux::Action action; action["type"] = QVariant::fromValue(ACTIONTYPE);                             \
        action[#NAME0] = QVariant::fromValue(NAME0); action[#NAME1] = QVariant::fromValue(NAME1);               \
        action[#NAME2] = QVariant::fromValue(NAME2); action[#NAME3] = QVariant::fromValue(NAME3);               \
		Qux::Flux::dispatch(action); }

#endif