//#include "../../include/async/runonnextframe.h"
#include "runonnextframe.h"

#include <QTimer>

namespace Qux {
namespace Async {

void runOnNextFrame(QObject* context, std::function<void()> func)
{
    auto timer = new QTimer(context);
    timer->setSingleShot(true);
    QObject::connect(timer, &QTimer::timeout, func);
    QObject::connect(timer, &QTimer::timeout, timer, &QObject::deleteLater);
    timer->start();
}

}
}