#include "../include/WebPageQuery.h"
#include "../include/utility.h"
#include "../include/tinyxml2.h"
#include "../include/json.hpp"

WebPageQuery::WebPageQuery(const std::string &filepath)
    : config(new Configuration(filepath)),
      tool(new JieBaSpilt(config->getFilePath("stopWordFilePath")))
{
    loadIndexFile();
}

std::string WebPageQuery::query(const std::string &text)
{
    nlohmann::ordered_json res;
    int index = 0;
    auto nums = get(text);
    if (nums.size() <= 0)
        return "";
    std::vector<int> id(nums.size());
    std::vector<std::tuple<std::string, std::string, std::string>> memo;
    std::iota(id.begin(), id.end(), 0);
    sort(id.begin(), id.end(), [&](const int &a, const int &b)
         { return nums[a].second > nums[b].second; });

    using namespace tinyxml2;
    std::ifstream file, dict_file;
    std::string filepath = config->getFilePath("indexStoreFilePath");
    file.open(filepath, std::ios::in);
    ERROR_CHECK(file.is_open() == false, "open %s failed", filepath.c_str());

    filepath = config->getFilePath("storedFilePath");
    dict_file.open(filepath, std::ios::in);
    ERROR_CHECK(dict_file.is_open() == false, "open %s failed", filepath.c_str());
    XMLDocument doc;

    for (std::string line; std::getline(file, line);)
    {
        std::istringstream is(line);
        long long id, pos, length;
        is >> id >> pos >> length;
        if (id < nums[index].first)
            continue;
        index++, dict_file.seekg(pos);
        std::string data(length, '\0');
        dict_file.read(data.data(), length);
        doc.Parse(data.c_str());
        XMLElement *title = doc.FirstChildElement("doc")->FirstChildElement("title");
        XMLElement *link = doc.FirstChildElement("doc")->FirstChildElement("url");
        XMLElement *content = doc.FirstChildElement("doc")->FirstChildElement("content");
        memo.push_back(std::make_tuple(title->GetText() ? title->GetText() : "",
                                       link->GetText() ? link->GetText() : "",
                                       content->GetText() ? content->GetText() : ""));
    }

    for (int i = 0; i < id.size(); i++)
    {
        res["result"].push_back({{"title", std::get<0>(memo[id[i]])},
                                 {"link", std::get<1>(memo[id[i]])},
                                 {"content", std::get<2>(memo[id[i]])}});
    }
    return res.dump();
}

std::vector<std::pair<int, double>> WebPageQuery::get(const std::string &text)
{
    double base_mod = 0;
    std::vector<std::pair<int, double>> ret;
    auto base = IDF(text);
    std::sort(base.begin(), base.end());
    // 文章ID-<单词-权重>
    std::unordered_map<int, std::set<std::pair<std::string, double>>> memo;
    for (auto &x : base)
    {
        if (!_index_dict.count(x.first))
            return {};
        for (auto &item : _index_dict[x.first])
            memo[item.first].insert(std::make_pair(x.first, item.second));
        base_mod += x.second * x.second;
    }
    base_mod = (double)sqrt(base_mod);
    for (auto &x : memo)
    {
        if (x.second.size() < base.size())
            continue;
        int index = 0;
        double res_mod = 0, res = 0;
        for (auto &item : x.second)
        {
            res += base[index].second * item.second;
            res_mod += item.second * item.second;
        }
        res_mod = sqrt(res_mod);
        ret.push_back(std::make_pair(x.first, res / (sqrt(res_mod) * base_mod)));
    }
    sort(ret.begin(), ret.end());
    return ret;
}

std::vector<std::pair<std::string, double>> WebPageQuery::IDF(const std::string &word)
{
    std::vector<std::string> words = tool->cut(word);
    std::vector<std::pair<std::string, double>> weight;
    std::unordered_map<std::string, int> freq;
    for (auto &x : words)
        ++freq[x];
    for (auto &x : freq)
        weight.push_back(std::make_pair(x.first, log2(1.0 / (x.second + 1.0))));
    return weight;
}

void WebPageQuery::loadIndexFile()
{
    std::ifstream file;
    std::string filepath = config->getFilePath("invertIndexStoreFilePath");
    file.open(filepath, std::ios::in);
    ERROR_CHECK(file.is_open() == false, "open %s failed", filepath.c_str());
    for (std::string line; std::getline(file, line);)
    {
        std::string name;
        int id;
        double freq;
        std::istringstream is(line);
        is >> name;
        while (is >> id >> freq)
            _index_dict[name].insert(std::make_pair(id, freq));
    }
}
