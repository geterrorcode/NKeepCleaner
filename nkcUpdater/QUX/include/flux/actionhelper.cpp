#include "../include/flux/actionhelper.h"
#include "../include/flux/dispatcher.h"

namespace Qux {
namespace Flux {

void dispatch(const Action& action)
{
    emit Dispatcher::inst().received(action);
}

}
}