#include "../include/JieBaSpilt.h"

JieBaSpilt::JieBaSpilt(const std::string &stop_file_path)
{
    spilt = new cppjieba::Jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, stop_file_path.c_str());
    _stop_word.insert(std::string(1, '\n'));
}

std::vector<std::string> JieBaSpilt::cut(const std::string &word)
{
    std::string line = word;
    std::vector<std::string> words, res; // 分词结果
    spilt->Cut(line, words, true);
    for (auto &x : words)
    {
        if (!(x.c_str()[0] & 0x80))
        {
            std::for_each(x.begin(), x.end(), [](char &c)
                          { if(isalpha(c)) c |= (1 << 5); else c = ' '; });
            char buf[x.size() + 1];
            ::strcpy(buf, x.c_str());
            for (char *token = ::strtok(buf, " "); token; token = ::strtok(NULL, " "))
                res.push_back(token);
            continue;
        }
        if (_stop_word.count(x))
            continue;
        res.push_back(x);
    }
    return res;
}