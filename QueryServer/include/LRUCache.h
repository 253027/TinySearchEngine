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
    LRUCache(int capacity)
    {
        head = new DListNode();
        tail = new DListNode();
        head->next = tail;
        tail->pre = head;
        this->capacity = capacity;
        len = 0;
    }

    std::string get(std::string key)
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

    void put(std::string key, std::string value)
    {
        if (len < capacity)
        {
            if (mp.count(key))
            {
                DListNode *temp = new DListNode(key, value);
                addNode(temp);
                delNode(mp[key]);
                mp[key] = temp;
            }
            else
            {
                len++;
                DListNode *temp = new DListNode(key, value);
                addNode(temp);
                mp[key] = temp;
            }
        }
        else
        {
            if (mp.count(key))
            {
                DListNode *temp = new DListNode(key, value);
                addNode(temp);
                delNode(mp[key]);
                mp[key] = temp;
            }
            else
            {
                DListNode *temp = new DListNode(key, value);
                addNode(temp);
                delNode(tail->pre);
                mp[key] = temp;
            }
        }
    }

private:
    void addNode(DListNode *temp)
    {
        temp->next = head->next;
        temp->pre = head;
        head->next->pre = temp;
        head->next = temp;
    }

    void delNode(DListNode *temp)
    {
        temp->next->pre = temp->pre;
        temp->pre->next = temp->next;
        mp.erase(temp->_key);
    }

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