#include "loadsync.h"

//#include <Qux/include/async/waitforsignal.h>

#include "../QUX/include/async/waitforsignal.h"
#include "loader.h"

QByteArray loadSync(QUrl url)
{
    Loader loader(url, nullptr);
    QByteArray result;

    QObject::connect(&loader, &Loader::completed, [&result] (QByteArray bytes) {
        result = bytes;
    });

    Qux::Async::waitForSignal(&loader, &Loader::completed, &Loader::error);

    return result;
}