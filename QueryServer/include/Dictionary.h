#ifndef __DICTIONARY_HEAD_H__
#define __DICTIONARY_HEAD_H__

#include <vector>
#include <string>
#include <queue>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "./Configuration.h"

class SpiltTool;

class Dictionary
{

    struct value
    {
        int mid_dis, mid_freq;
        std::string word;
        value(const int &a, const int &b, const std::string &c);
    };

    struct cmp
    {
        bool operator()(const struct value &a, const struct value &b) const;
    };

    using value_heap = std::priority_queue<value, std::vector<value>, cmp>;

public:
    static Dictionary *GetInstance(const std::string zh_filepath = "", const std::string en_filepath = "");

    std::string query(const std::string &word);

private:
    Dictionary(const std::string &zh_filepath = "", const std::string &en_filepath = "");

    ~Dictionary();

    void query(const std::string &word, value_heap &zh_heap, value_heap &en_heap);

    Dictionary(const Dictionary &) = delete;

    Dictionary &operator=(const Dictionary &) = delete;

    Dictionary(const Dictionary &&) = delete;

    std::vector<std::string> spilt(const std::string &s);

    int minDistance(std::vector<std::string> source, std::vector<std::string> query);

    void createIndexDictionary(std::shared_ptr<Configuration> &config);

    void createEnglishDictionary();

    void createChineseDictionary();

    std::string getMinIndex(std::unordered_set<std::string> &query, std::unordered_map<std::string, std::unordered_set<int>> &dict);

    static Dictionary *dic;

    std::shared_ptr<Configuration> zh_config;

    std::shared_ptr<Configuration> en_config;

    std::shared_ptr<SpiltTool> _spilt;

    std::vector<std::tuple<std::string, int, int>> _en_dict;

    std::vector<std::pair<std::string, int>> _zh_dict;

    std::unordered_map<std::string, std::unordered_set<int>> _en_index_dict;

    std::unordered_map<std::string, std::unordered_set<int>> _zh_index_dict;
};

#endif //__DICTIONARY_HEAD_H__