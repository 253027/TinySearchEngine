#ifndef __WEBPAGEGENERATOR_HEAD_H__
#define __WEBPAGEGENERATOR_HEAD_H__

#include "./simhash/Simhasher.hpp"
#include <unistd.h>
#include <dirent.h>
#include <string>
#include <functional>
#include <algorithm>
#include <regex>
#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>
#include <utility>
#include "./tinyxml2.h"
#include "./cppjieba/Jieba.hpp"

class WebPageGenerator
{
public:
    WebPageGenerator(const std::string &dirpath, const std::string &dicPath, const std::string &indexDicPath);

    void parse();

    void removeDuplicates();

    void createInvertIndex();

private:
    double idf(int page_nums, int df_frequence);

    std::vector<std::tuple<int, int, int>> memo;
    // 处理语料库
    std::string _dirpath;
    // 字典存储路径
    std::string _dicPath;
    // 索引字典存储路径
    std::string _indexDicPath;

    const char *const DICT_PATH = "./include/cppjieba/dict/jieba.dict.utf8";

    const char *const HMM_PATH = "./include/cppjieba/dict/hmm_model.utf8";

    const char *const USER_DICT_PATH = "./include/cppjieba/dict/user.dict.utf8";

    const char *const IDF_PATH = "./include/cppjieba/dict/idf.utf8";

    const char *const STOP_WORD_PATH = "./include/cppjieba/dict/stop_words.utf8";
};

#endif //__WEBPAGEGENERATOR_HEAD_H__