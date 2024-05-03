#ifndef __WEBPAGEGENERATOR_HEAD_H__
#define __WEBPAGEGENERATOR_HEAD_H__

#include <unistd.h>
#include <dirent.h>
#include <string>
#include <functional>
#include <regex>
#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include "./tinyxml2.h"

class WebPageGenerator
{
public:
    WebPageGenerator(const std::string &dirpath, const std::string &dicPath, const std::string &indexDicPath);

    void parse();

private:
    std::vector<std::tuple<int, int, int>> memo;

    std::string _dirpath;

    std::string _dicPath;

    std::string _indexDicPath;
};

#endif //__WEBPAGEGENERATOR_HEAD_H__