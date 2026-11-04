#pragma once

#include "../ux/singleton.h"
#include <QNetworkAccessManager>

class NetworkManager : public QNetworkAccessManager,
    public common::ux::Singleton<NetworkManager>

{
    Q_OBJECT

private:
    friend NetworkManager& common::ux::Singleton<NetworkManager>::inst();
    NetworkManager();


public:
    void clearCacheData(const QUrl& url);
};
