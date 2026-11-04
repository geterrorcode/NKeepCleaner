#include "networkmanager.h"
#include "networkimagecache.h"

NetworkManager::NetworkManager()
{
    setCache(new NetworkImageCache);
}

void NetworkManager::clearCacheData(const QUrl& url)
{
    auto cacheData = cache();

    cacheData->remove(url);
    


}