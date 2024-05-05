#ifndef __ENGLISHSPILT_HEAD_H__
#define __ENGLISHSPILT_HEAD_H__

#include <fstream>
#include <unordered_map>
#include "./SpiltTool.h"
#include "./cppjieba/Jieba.hpp"

class EnglishSpilt : public SpiltTool
{
public:
    EnglishSpilt(const std::string &stop_word_filepath);

    std::vector<std::string> cut(const std::string &word) override;

private:
    // 停用词集合
    std::unordered_set<std::string> _stop_word;
};

#endif //__ENGLISHSPILT_HEAD_H__