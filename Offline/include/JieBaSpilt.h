#ifndef __JIEBASPILT_HEAD_H__
#define __JIEBASPILT_HEAD_H__

#include <unordered_map>
#include "./SpiltTool.h"
#include "./cppjieba/Jieba.hpp"

class JieBaSpilt : public SpiltTool
{
public:
    JieBaSpilt(const std::string &stop_file_path);

    std::vector<std::string> cut(const std::string &word) override;

private:
    // 停用词集合用于二次过滤
    std::unordered_set<std::string> _stop_word;
    // 分词器
    cppjieba::Jieba *spilt;
    // 中文分词词典1
    const char *const DICT_PATH = "./include/cppjieba/dict/jieba.dict.utf8";
    // 生僻字词典
    const char *const HMM_PATH = "./include/cppjieba/dict/hmm_model.utf8";
    // 用户自定义词典
    const char *const USER_DICT_PATH = "./include/cppjieba/dict/user.dict.utf8";
    // 中文分词词典2
    const char *const IDF_PATH = "./include/cppjieba/dict/idf.utf8";
    // 停用词词典
    const char *const STOP_WORD_PATH = "./include/cppjieba/dict/stop_words.utf8";
};

#endif //__JIEBASPILT_HEAD_H__