#ifndef __DICTIONARYGENERATOR_HEAD_H__
#define __DICTIONARYGENERATOR_HEAD_H__

#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <string>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include "./EnglishSpilt.h"
#include "./JieBaSpilt.h"
#include "./Configuration.h"

class DictionaryGenerator
{
public:
    DictionaryGenerator(const std::string &filepath);

    void parse();

    void createIndexDictionary();

private:
    void recursionParse(const std::string &filepath);

    std::shared_ptr<SpiltTool> tool;

    std::shared_ptr<Configuration> config;

    std::unordered_map<std::string, int> count;
};

#endif //__DICTIONARYGENERATOR_HEAD_H_