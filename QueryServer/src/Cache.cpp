#include "../include/Cache.h"
#include "../include/LRUCache.h"

Cache::Cache(uint32_t size) : _cache(new LRUCache(size)),
                              _cache_backup(new LRUCache(size))
{
    ;
}

int Cache::query(const std::string &text, std::string &result)
{
    std::lock_guard<std::mutex> guard(_cache_mutex);
    return _cache->get(text) == "-1";
}

void Cache::insert(const std::string &key, const std::string &value)
{
    std::lock_guard<std::mutex> guard(_cache_mutex);
    _cache->put(key, value);
}

void Cache::notify(LRUCache *total_cacahe)
{
    std::lock_guard<std::mutex> guard(_cache_backup_mutex);
    *(_cache_backup.get()) = *total_cacahe;
}

void Cache::swap()
{
    std::lock_guard<std::mutex> guard_1(_cache_backup_mutex);
    std::lock_guard<std::mutex> guard_2(_cache_mutex);
    _cache.swap(_cache_backup);
}
