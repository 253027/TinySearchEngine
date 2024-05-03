#ifndef __WORDQUERY_HEAD_H__
#define __WORDQUERY_HEAD_H__

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <fstream>
#include <sstream>
#include <utility>
#include "./utility.h"
#include "./cppjieba/Jieba.hpp"
using std::string;

class WordQuery
{
public:
    WordQuery();

    void Initial(const string &en_index_file, const string &en_dic_file, const string &zh_index_file, const string &zh_dic_file);

    std::vector<string> query(const string &queryword);

private:
    struct value
    {
        int mid_dis, mid_freq;
        string word;
        value(const int &a, const int &b, const string &c) : mid_dis(a), mid_freq(b), word(c) {}
    };

    struct cmp
    {
        bool operator()(const struct value &a, const struct value &b) const
        {
            if (a.mid_dis == b.mid_dis)
            {
                if (a.mid_freq == b.mid_freq)
                    return a.word > b.word;
                return a.mid_freq > b.mid_freq;
            }
            return a.mid_dis > b.mid_dis;
        };
    };

    int minDistance(std::vector<string> s, std::vector<string> t);

    void createIndexDictionary(const string &filepath, const string &filetype);

    void createEnglishDictionary(const string &filepath);

    void createChineseDictionary(const string &filepath);

    std::vector<string> spilt(const string &s);

    std::vector<std::tuple<string, int, int>> _english_dictionary;

    std::unordered_map<string, std::unordered_set<int>> _english_index_dictionary;

    std::vector<std::pair<string, int>> _chinese_dictionary;

    std::unordered_map<string, std::unordered_set<int>> _chinese_index_dictionary;
};

#endif //__WORDQUERY_HEAD_H__