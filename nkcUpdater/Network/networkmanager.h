#pragma once

#include <QNetworkAccessManager>

//#include <Qux/include/singleton.h>
#include "../QUX/include/singleton.h"

class NetworkManager : public QNetworkAccessManager,
    public Qux::Singleton<NetworkManager>

{
    Q_OBJECT

private:
    friend NetworkManager& Qux::Singleton<NetworkManager>::inst();
    NetworkManager();

};
