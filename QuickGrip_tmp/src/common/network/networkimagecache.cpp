#include "networkimagecache.h"

NetworkImageCache::NetworkImageCache(QObject* parent)
    : QNetworkDiskCache(parent)
{
    setCacheDirectory("NPixelImageCache");
}

QIODevice* NetworkImageCache::prepare(const QNetworkCacheMetaData& metaData)
{
    for (auto header : metaData.rawHeaders()) {
        if (header.first.toLower() == "content-type") {
            if (header.second.startsWith("image/")) {
                return QNetworkDiskCache::prepare(metaData);
            }

            break;
        }
    }
    return nullptr;
}