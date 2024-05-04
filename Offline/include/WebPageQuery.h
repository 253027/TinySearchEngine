#ifndef __WEBPAGEQUERY_HEAD_H__
#define __WEBPAGEQUERY_HEAD_H__

#include <cmath>
#include <memory>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "./Configuration.h"
#include "./JieBaSpilt.h"

class WebPageQuery
{
public:
    WebPageQuery(const std::string &filepath);

    std::string query(const std::string &text);

private:
    std::vector<std::pair<std::string, double>> IDF(const std::string &word);

    void loadIndexFile();

    std::shared_ptr<Configuration> config;

    std::shared_ptr<SpiltTool> tool;

    std::unordered_map<std::string, std::set<std::pair<int, double>>> _index_dict;
};

#endif //__WEBPAGEQUERY_HEAD_H__