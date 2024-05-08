#include "../include/CashManger.h"
#include <unistd.h>
#include <signal.h>

CashManger *CashManger::GetInstance()
{
    if (!_instance)
        _instance = new CashManger();
    return _instance;
}

void CashManger::insert()
{
    std::shared_ptr<Cache> cach(new Cache(3));
    memo.push_back(cach);
}

Cache &CashManger::get(int index)
{
    static Cache error(0);
    if (index < 0 || index >= memo.size())
        return error;
    return *memo[index];
}

const int CashManger::size() const
{
    return memo.size();
}

CashManger::CashManger() {}

CashManger::~CashManger() {}
