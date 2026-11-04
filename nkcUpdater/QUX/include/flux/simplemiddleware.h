#ifndef QUX_FLUX_SIMPLEMIDDLEWARE_H
#define QUX_FLUX_SIMPLEMIDDLEWARE_H

#include "action.h"

#include <QObject>
#include <functional>

namespace Qux {
namespace Flux {

class SimpleMiddleware : public QObject
{
    Q_OBJECT

public:
    typedef std::function<void (const Action& action)> Reducer;

    SimpleMiddleware(Reducer reducer, QObject* parent = nullptr);

};

}
}

#endif