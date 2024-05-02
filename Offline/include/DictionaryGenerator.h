#ifndef __DICTIONARYGENERATOR_HEAD_H__
#define __DICTIONARYGENERATOR_HEAD_H__

#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <functional>
#include <unordered_map>

class DictionaryGenerator
{
public:
    DictionaryGenerator(const std::string &stop_words_filepath);
    // parset all english language files and generate the frequence of the words into a file
    void parseEnglish(const std::string &directory_path);
    // parset all chinese language files and generate the frequence of the words into a file
    void parseChinese(const std::string &directory_path);

private:
    std::unordered_set<std::string> _stop_words;
};

#endif //__DICTIONARYGENERATOR_HEAD_H_