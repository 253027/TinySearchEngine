#include "../include/EnglishSpilt.h"
#include "../include/utility.h"

EnglishSpilt::EnglishSpilt(const std::string &stop_word_filepath)
{
    std::ifstream in;
    in.open(stop_word_filepath, std::ios::in);
    ERROR_CHECK(in.is_open() == false, "open %s failed", stop_word_filepath.c_str());
    for (std::string line; std::getline(in, line);)
        _stop_word.insert(line);
    _stop_word.insert(std::string(1, '\n'));
    in.close();
}

std::vector<std::string> EnglishSpilt::cut(const std::string &word)
{
    std::vector<std::string> res;
    char buf[word.size() + 1];
    ::strcpy(buf, word.c_str());
    for (int i = 0; i < word.size(); i++)
    {
        if (::isalpha(buf[i]))
            buf[i] |= (1 << 5);
        else
            buf[i] = ' ';
    }
    for (char *token = ::strtok(buf, " "); token; token = ::strtok(NULL, " "))
    {
        if (_stop_word.count(token))
            continue;
        res.push_back(token);
    }
    return res;
}