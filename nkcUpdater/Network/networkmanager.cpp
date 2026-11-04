#include "networkmanager.h"
#include "networkimagecache.h"

NetworkManager::NetworkManager()
{
    setCache(new NetworkImageCache);
}