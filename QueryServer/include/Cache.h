#ifndef __CACHE_HEAD_H__
#define __CACHE_HEAD_H__

#include <memory>
#include <string>
#include <mutex>

class LRUCache;

class Cache
{
public:
    Cache(uint32_t size);

    int query(const std::string &text, std::string &result);

    void insert(const std::string &key, const std::string &value);

    void notify(LRUCache *total_cacahe);

    void swap();

private:
    std::shared_ptr<LRUCache> _cache;

    std::shared_ptr<LRUCache> _cache_backup;

    std::mutex _cache_mutex;

    std::mutex _cache_backup_mutex;
};

#endif //__CACHE_HEAD_H__