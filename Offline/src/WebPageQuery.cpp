#include "../include/WebPageQuery.h"
#include "../include/utility.h"

WebPageQuery::WebPageQuery(const std::string &filepath)
    : config(new Configuration(filepath)),
      tool(new JieBaSpilt(config->getFilePath("stopWordFilePath")))
{
    loadIndexFile();
}

std::string WebPageQuery::query(const std::string &text)
{
    std::string res;
    auto base = IDF(text);
    for (auto &x : base)
    {
        if (!_index_dict.count(x.first))
            return "";
        for (auto &item : _index_dict[x.first])
        {
            ;
        }
    }

    return res;
}

std::vector<std::pair<std::string, double>> WebPageQuery::IDF(const std::string &word)
{
    std::vector<std::string> words = tool->cut(word);
    std::vector<std::pair<std::string, double>> weight;
    std::unordered_map<std::string, int> freq;
    for (auto &x : words)
        ++freq[x];
    for (auto &x : freq)
        weight.push_back(std::make_pair(x.first, std::log2(1 / (x.second + 1))));
    return weight;
}

void WebPageQuery::loadIndexFile()
{
    std::ifstream file;
    std::string filepath = config->getFilePath("indexStoreFilePath");
    file.open(filepath, std::ios::in);
    ERROR_CHECK(file.is_open() == false, "open %s failed", filepath.c_str());
    for (std::string line; std::getline(file, line);)
    {
        std::string name;
        int id;
        double freq;
        std::istringstream is(line);
        is >> name >> id >> freq;
        _index_dict[name].insert(std::make_pair(id, freq));
    }
}
