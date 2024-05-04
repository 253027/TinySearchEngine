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
#include <memory>
#include "./EnglishSpilt.h"
#include "./JieBaSpilt.h"
#include "./Configuration.h"

class DictionaryGenerator
{
public:
    DictionaryGenerator(const std::string &filepath);
    // parset all language files and generate the frequence of the words into a file
    void parse();
    // create the index directory
    void createIndexDirectory();

private:
    void recursionParse(const std::string &filepath);

    std::shared_ptr<SpiltTool> tool;

    std::shared_ptr<Configuration> config;

    std::unordered_map<std::string, int> count;
};

#endif //__DICTIONARYGENERATOR_HEAD_H_