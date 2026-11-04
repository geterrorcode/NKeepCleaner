#ifndef QUX_FLUX_DISPATCHER_H
#define QUX_FLUX_DISPATCHER_H

#include "action.h"
#include "../singleton.h"

#include <QObject>

namespace Qux {
namespace Flux {

class Dispatcher : public QObject,
    public Singleton<Dispatcher>
{
    Q_OBJECT

private:
    friend Dispatcher& Singleton<Dispatcher>::inst();
    Dispatcher() {}

signals:
    void received(const Action& action) const;

};

}
}

#endif
