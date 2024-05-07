#ifndef __LRUCACHE_H_HEAD__
#define __LRUCACHE_H_HEAD__

#include <unordered_map>
#include <string>

class LRUCache
{
    struct DListNode
    {
        std::string _key, _val;
        DListNode *pre;
        DListNode *next;
        DListNode() : _val(""), pre(nullptr), next(nullptr) {}
        DListNode(std::string key, std::string val) : _key(key), _val(val), pre(nullptr), next(nullptr) {}
    };

public:
    LRUCache(int capacity);

    std::string get(std::string key);

    void put(std::string key, std::string value);

private:
    void addNode(DListNode *temp);

    void delNode(DListNode *temp);

    std::unordered_map<std::string, DListNode *> mp;

    DListNode *head, *tail;

    int capacity;

    int len;
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */

#endif //__LRUCASHE_H_HEAD__