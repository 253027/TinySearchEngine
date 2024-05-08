#include "../include/LRUCache.h"

LRUCache::LRUCache(int capacity)
{
    this->capacity = capacity;
    prehead = new listnode("", "");
    backend = new listnode("", "");
    prehead->next = backend;
    backend->prev = prehead;
}

LRUCache::~LRUCache()
{
    for (auto &x : hash)
        delete x.second;
    delete prehead;
    delete backend;
}

LRUCache &LRUCache::operator=(const LRUCache &val)
{
    if (this == &val)
        return *this;
    for (auto &x : hash)
        delete x.second;
    prehead->next = backend;
    backend->prev = prehead;
    hash.clear();
    for (listnode *p = val.backend; p != val.prehead; p = p->prev)
    {
        listnode *temp = new listnode(p->key, p->value);
        temp->next = this->prehead->next;
        this->prehead->next->prev = temp;
        this->prehead->next = temp;
        temp->prev = this->prehead;
        hash[p->key] = temp;
    }
    return *this;
}

std::string LRUCache::get(std::string key)
{
    if (!hash.count(key))
        return "-1";
    listnode *temp = hash[key];
    MoveToHead(temp);
    return temp->value;
}

void LRUCache::put(std::string key, std::string value)
{
    if (hash.count(key))
    {
        listnode *temp = hash[key];
        temp->value = value;
        MoveToHead(temp);
    }
    else
    {
        listnode *temp = new listnode(key, value);
        AddToHead(temp), capacity--, hash[key] = temp;
        if (capacity < 0)
        {
            capacity++;
            temp = RemoveTail();
            hash.erase(temp->key);
            delete temp;
        }
    }
}

void LRUCache::RemoveNode(listnode *target)
{
    target->prev->next = target->next;
    target->next->prev = target->prev;
}

void LRUCache::AddToHead(listnode *target)
{
    target->next = prehead->next;
    target->next->prev = target;
    target->prev = prehead;
    prehead->next = target;
}

void LRUCache::MoveToHead(listnode *target)
{
    RemoveNode(target);
    AddToHead(target);
}

LRUCache::listnode *LRUCache::RemoveTail()
{
    listnode *temp = backend->prev;
    RemoveNode(temp);
    return temp;
}
