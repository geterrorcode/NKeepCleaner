#pragma once

#include <QNetworkDiskCache>

class NetworkImageCache : public QNetworkDiskCache

{
    Q_OBJECT

public:
    NetworkImageCache(QObject* parent = nullptr);

    QIODevice* prepare(const QNetworkCacheMetaData& metaData) override;

};
