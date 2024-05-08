#ifndef __LRUCACHE_H_HEAD__
#define __LRUCACHE_H_HEAD__

#include <unordered_map>
#include <string>

class LRUCache
{
public:
    LRUCache(int capacity);

    ~LRUCache();

    LRUCache &operator=(const LRUCache &val);

    std::string get(std::string key);

    void put(std::string key, std::string value);

private:
    struct listnode
    {
        std::string key, value;
        listnode *prev, *next;
        listnode(std::string _key, std::string _value) : key(_key), value(_value), prev(nullptr), next(nullptr) {}
    };

    int capacity;

    std::unordered_map<std::string, listnode *> hash;

    listnode *prehead, *backend;

private:
    void RemoveNode(listnode *target);

    void AddToHead(listnode *target);

    void MoveToHead(listnode *target);

    listnode *RemoveTail();
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */

#endif //__LRUCASHE_H_HEAD__