#ifndef QUX_ASYNC_RUNONMAINTHREAD_H
#define QUX_ASYNC_RUNONMAINTHREAD_H

#include <functional>

namespace Qux {
namespace Async {

/* thread-safe */
void runOnMainThread(std::function<void ()> func);

}
}

#endif