#include "../include/WordQuery.h"

WordQuery::WordQuery() {}

void WordQuery::Initial(const string &en_index_file, const string &en_dic_file, const string &zh_index_file, const string &zh_dic_file)
{
    createIndexDictionary(en_index_file, "english");

    createIndexDictionary(zh_index_file, "chinese");

    createEnglishDictionary(en_dic_file);

    createChineseDictionary(zh_dic_file);
}

std::vector<string> WordQuery::query(const string &queryword)
{
    int size = queryword.size(), en_memo = 0;
    int en_min_value = -1;
    std::unordered_set<string> zh_memo;
    for (int i = 0; i < size; i++)
    {
        if (queryword[i] & 0x80)
            zh_memo.insert(queryword.substr(i, 3)), i += 2;
        else if (::isalpha(queryword[i]))
            en_memo |= 1 << (queryword[i] - 'a');
    }

    auto &en = _english_index_dictionary;
    auto &zh = _chinese_index_dictionary;

    // 预处理找到最小的英文集合
    for (int i = 0; i < 32; i++)
    {
        if (!((en_memo >> i) & 1))
            continue;
        if (en_min_value == -1 || en[string(1, i + 'a')].size() < en[string(1, en_min_value + 'a')].size())
            en_min_value = i;
    }
    // 统计出现过的英文单词
    std::unordered_set<int> en_set;
    string min_en_name = string(1, en_min_value + 'a');
    for (auto &x : en[min_en_name])
    {
        int state = std::get<2>(_english_dictionary[x]);
        if ((state & en_memo) != en_memo)
            continue;
        en_set.insert(x);
    }

    // 预处理找到最小中文单词集合
    en_min_value = -1;
    for (auto &x : zh_memo)
    {
        if (en_min_value == -1 || zh[x].size() > en_min_value)
            en_min_value = zh[x].size(), min_en_name = x;
    }
    std::unordered_set<int> zh_set;
    for (auto &x : zh[min_en_name])
    {
        bool ok = true;
        for (auto &t : zh_memo)
        {
            if (zh[t].count(x))
                continue;
            ok = false;
            break;
        }
        if (ok)
            zh_set.insert(x);
    }

    std::priority_queue<value, std::vector<value>, cmp> heap;
    std::vector<string> queryword_set = spilt(queryword);
    for (auto &x : zh_set)
    {
        string name = std::get<0>(_chinese_dictionary[x]);
        int freq = std::get<1>(_chinese_dictionary[x]);
        heap.push(value(minDistance(spilt(name), queryword_set), freq, name));
    }

    for (auto &x : en_set)
    {
        string name = std::get<0>(_english_dictionary[x]);
        int freq = std::get<1>(_english_dictionary[x]);
        heap.push(value(minDistance(spilt(name), queryword_set), freq, name));
    }

    std::vector<string> res;
    for (int i = 0; i < 5 && !heap.empty(); i++)
    {
        res.push_back(heap.top().word);
        heap.pop();
    }
    return res;
}

int WordQuery::minDistance(std::vector<string> s, std::vector<string> t)
{
    int n = s.size(), m = t.size();
    std::vector<std::vector<int>> f(n + 1, std::vector<int>(m + 1));
    for (int j = 0; j <= m; ++j)
        f[0][j] = j;
    for (int i = 0; i < n; ++i)
    {
        f[i + 1][0] = i + 1;
        for (int j = 0; j < m; ++j)
            f[i + 1][j + 1] = s[i] == t[j] ? f[i][j] : std::min(std::min(f[i][j + 1], f[i + 1][j]), f[i][j]) + 1;
    }
    return f[n][m];
}

void WordQuery::createIndexDictionary(const string &filepath, const string &filetype)
{
    string line, word;
    std::ifstream in;
    std::unordered_map<string, std::unordered_set<int>> temp;
    in.open(filepath.c_str(), std::ios::in);
    ERROR_CHECK(in.is_open() == false, "open %s index file failed\n", filetype.c_str());
    while (std::getline(in, line))
    {
        string name;
        std::istringstream str(line);
        str >> name;
        while (str >> word)
            temp[name].insert(std::stoi(word));
    }
    in.close();
    filetype == "english" ? _english_index_dictionary.swap(temp) : _chinese_index_dictionary.swap(temp);
}

void WordQuery::createEnglishDictionary(const string &filepath)
{
    string line;
    std::ifstream in;
    in.open(filepath.c_str(), std::ios::in);
    ERROR_CHECK(in.is_open() == false, "open english file failed\n");
    while (std::getline(in, line))
    {
        std::istringstream str(line);
        string name, nums;
        str >> name >> nums;
        int hash = 0;
        for (auto &x : name)
            hash |= 1 << (x - 'a');
        _english_dictionary.push_back(std::make_tuple(name, std::stoi(nums), hash));
    }
    in.close();
}

void WordQuery::createChineseDictionary(const string &filepath)
{
    string line;
    std::ifstream in;
    in.open(filepath.c_str(), std::ios::in);
    ERROR_CHECK(in.is_open() == false, "open chinese file failed\n");
    while (std::getline(in, line))
    {
        std::istringstream str(line);
        string name, nums;
        str >> name >> nums;
        _chinese_dictionary.push_back(std::make_pair(name, std::stoi(nums)));
    }
    in.close();
}

std::vector<string> WordQuery::spilt(const string &s)
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
