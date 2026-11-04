#ifndef QUX_FLUX_STORE_H
#define QUX_FLUX_STORE_H

#include "action.h"

#include <QObject>

namespace Qux {
namespace Flux {

class Store : public QObject
{
    Q_OBJECT

public:
    Store(QObject* parent = nullptr);

protected slots:
    virtual void reduce(const Action& action) = 0;

};

}
}

#endif