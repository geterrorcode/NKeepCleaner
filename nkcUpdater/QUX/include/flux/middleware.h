#ifndef QUX_FLUX_MIDDLEWARE_H
#define QUX_FLUX_MIDDLEWARE_H

#include "action.h"

#include <QObject>

namespace Qux {
namespace Flux {

class Middleware : public QObject
{
    Q_OBJECT

protected:
    Middleware(QObject* parent = nullptr);

protected slots:
    virtual void reduce(const Action& action) = 0;

};

}
}

#endif