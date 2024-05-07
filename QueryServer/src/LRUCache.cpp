#include "../include/LRUCache.h"

LRUCache::LRUCache(int capacity)
{
    head = new DListNode();
    tail = new DListNode();
    head->next = tail;
    tail->pre = head;
    this->capacity = capacity;
    len = 0;
}

std::string LRUCache::get(std::string key)
{
    if (mp.count(key))
    {
        DListNode *temp = new DListNode(key, mp[key]->_val);
        addNode(temp);
        delNode(mp[key]);
        mp[key] = temp;
        return mp[key]->_val;
    }
    return "";
}

void LRUCache::put(std::string key, std::string value)
{
    if (len < capacity)
    {
        DListNode *temp = new DListNode(key, value);
        addNode(temp);
        if (mp.count(key))
            delNode(mp[key]);
        else
            len++;
        mp[key] = temp;
    }
    else
    {
        DListNode *temp = new DListNode(key, value);
        addNode(temp);
        if (mp.count(key))
            delNode(mp[key]);
        else
            delNode(tail->pre);
        mp[key] = temp;
    }
}

void LRUCache::addNode(DListNode *temp)
{
    temp->next = head->next;
    temp->pre = head;
    head->next->pre = temp;
    head->next = temp;
}

void LRUCache::delNode(DListNode *temp)
{
    temp->next->pre = temp->pre;
    temp->pre->next = temp->next;
    mp.erase(temp->_key);
}
