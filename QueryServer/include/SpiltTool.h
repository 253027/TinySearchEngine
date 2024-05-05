#ifndef __SPILTTOOL_HEAD_H__
#define __SPILTTOOL_HEAD_H__

#include <vector>
#include <string>

class SpiltTool
{
public:
    virtual std::vector<std::string> cut(const std::string &word) = 0;
};

#endif //__SPILTTOOL_HEAD_H__