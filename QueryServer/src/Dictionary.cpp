#include "../include/Dictionary.h"
#include "../include/utility.h"
using namespace std;

Dictionary *Dictionary::GetInstance(const std::string zh_filepath, const std::string en_filepath)
{
    if (!dic)
        dic = new Dictionary(zh_filepath, en_filepath);
    return dic;
}

std::string Dictionary::query(const std::string &word)
{
    vector<string> res;
    vector<string> single_word = spilt(word);
    unordered_set<string> zh, en;
    int bitvalue = 0;
    // 拆词分成字分别存储
    for (int i = 0; i < single_word.size(); i++)
    {
        if (single_word[i][0] & 0x80)
            zh.insert(single_word[i]);
        else
        {
            en.insert(single_word[i]);
            bitvalue |= 1 << (single_word[i][0] - 'a');
        }
    }
    // 得到中英文词的最小集合
    string zh_min = getMinIndex(zh, _zh_index_dict);
    string en_min = getMinIndex(en, _en_index_dict);
    // 统计出现的行号便于后面计算最小编辑距离
    unordered_set<int> row_zh, row_en;
    // 英文
    for (auto &x : _en_index_dict[en_min])
    {
        if ((std::get<2>(_en_dict[x]) & bitvalue) != bitvalue)
            continue;
        row_en.insert(x);
    }
    // 中文
    for (auto &x : _zh_index_dict[zh_min])
    {
        bool ok = true;
        for (auto &t : zh)
        {
            if (_zh_index_dict[t].count(x))
                continue;
            ok = false;
            break;
        }
        if (ok)
            row_zh.insert(x);
    }

    priority_queue<value, vector<value>, cmp> heap;
    for (auto &x : row_zh)
    {
        string name = std::get<0>(_zh_dict[x]);
        int freq = std::get<1>(_zh_dict[x]);
        heap.push(value(minDistance(spilt(name), single_word), freq, name));
    }

    for (auto &x : row_en)
    {
        string name = std::get<0>(_en_dict[x]);
        int freq = std::get<1>(_en_dict[x]);
        heap.push(value(minDistance(spilt(name), single_word), freq, name));
    }

    nlohmann::ordered_json js;

    for (int i = 0; i < 5 && !heap.empty(); i++)
    {
        js["result"].push_back({{"content", heap.top().word}});
        // res.push_back(heap.top().word);
        heap.pop();
    }

    return js.dump();
}

Dictionary::Dictionary(const std::string &zh_filepath, const std::string &en_filepath)
    : zh_config(new Configuration(zh_filepath)), en_config(new Configuration(en_filepath))
{
    createIndexDictionary(en_config);

    createIndexDictionary(zh_config);

    createEnglishDictionary();

    createChineseDictionary();
}

Dictionary::~Dictionary() {}

std::vector<std::string> Dictionary::spilt(const std::string &s)
{
    std::vector<string> res;
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] & 0x80)
            res.push_back(s.substr(i, 3)), i += 2;
        else if (::isalpha(s[i]))
            res.push_back(s.substr(i, 1));
    }
    return res;
}

int Dictionary::minDistance(std::vector<std::string> source, std::vector<std::string> query)
{
    int n = source.size(), m = query.size();
    vector<vector<int>> f(n + 1, vector<int>(m + 1));
    for (int j = 0; j <= m; ++j)
        f[0][j] = j;
    for (int i = 0; i < n; ++i)
    {
        f[i + 1][0] = i + 1;
        for (int j = 0; j < m; ++j)
            f[i + 1][j + 1] = source[i] == query[j] ? f[i][j] : min(min(f[i][j + 1], f[i + 1][j]), f[i][j]) + 1;
    }
    return f[n][m];
}

void Dictionary::createIndexDictionary(std::shared_ptr<Configuration> &config)
{
    ifstream file;
    unordered_map<string, unordered_set<int>> temp;
    string filepath = config->getFilePath("indexStoreFilePath");
    file.open(filepath, std::ios::in);
    ERROR_CHECK(file.is_open() == false, "open %s failed\n", filepath.c_str());
    for (string line, word; std::getline(file, line);)
    {
        string name;
        istringstream str(line);
        str >> name;
        while (str >> word)
            temp[name].insert(std::stoi(word));
    }
    file.close();
    config->getFilePath("type") == "English" ? _en_index_dict.swap(temp) : _zh_index_dict.swap(temp);
}

void Dictionary::createEnglishDictionary()
{
    ifstream file;
    string filepath = en_config->getFilePath("storedFilePath");
    file.open(filepath, std::ios::in);
    ERROR_CHECK(file.is_open() == false, "open %s failed", filepath.c_str());
    for (string line, word; std::getline(file, line);)
    {
        istringstream is(line);
        long long nums;
        int hash;
        string name;
        is >> name >> nums;
        for (auto &x : name)
            hash |= 1 << (x - 'a');
        _en_dict.push_back(std::make_tuple(name, nums, hash));
    }
    file.close();
}

void Dictionary::createChineseDictionary()
{
    ifstream file;
    string filepath = zh_config->getFilePath("storedFilePath");
    file.open(filepath, std::ios::in);
    ERROR_CHECK(file.is_open() == false, "open %s failed", filepath.c_str());
    for (string line; std::getline(file, line);)
    {
        std::istringstream str(line);
        long long nums = 0;
        string name;
        str >> name >> nums;
        _zh_dict.push_back(std::make_pair(name, nums));
    }
    file.close();
}

std::string Dictionary::getMinIndex(std::unordered_set<std::string> &query, std::unordered_map<std::string, std::unordered_set<int>> &dict)
{
    string name;
    int value = -1;
    for (auto &x : query)
    {
        if (value != -1 && dict[x].size() >= value)
            continue;
        name = x, value = dict[x].size();
    }
    return name;
}

Dictionary::value::value(const int &a, const int &b, const std::string &c)
    : mid_dis(a), mid_freq(b), word(c)
{
    ;
}

bool Dictionary::cmp::operator()(const struct value &a, const struct value &b) const
{
    if (a.mid_dis == b.mid_dis)
    {
        if (a.mid_freq == b.mid_freq)
            return a.word > b.word;
        return a.mid_freq > b.mid_freq;
    }
    return a.mid_dis > b.mid_dis;
}
