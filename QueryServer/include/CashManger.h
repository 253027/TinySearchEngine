#ifndef __CASHMANGER_HEAD_H__
#define __CASHMANGER_HEAD_H__

#include "./Cache.h"
#include <vector>
#include <memory>

class CashManger
{
public:
    static CashManger *GetInstance();

    void insert();

    Cache &get(int index);

    const int size() const;

private:
    class destory
    {
        ~destory() { delete _instance; };
    };

    CashManger();

    ~CashManger();

    std::vector<std::shared_ptr<Cache>> memo;

    static CashManger *_instance;

    static destory des;
};

#endif //__CASHMANGER_HEAD_H__