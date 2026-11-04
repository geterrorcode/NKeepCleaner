#ifndef QUX_ASYNC_RUNONNEXTFRAME_H
#define QUX_ASYNC_RUNONNEXTFRAME_H

#include <functional>
#include <QObject>

namespace Qux {
namespace Async {

void runOnNextFrame(QObject* contenxt, std::function<void ()> func);

}
}

#endif