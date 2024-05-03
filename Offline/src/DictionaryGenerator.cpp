#include "../include/DictionaryGenerator.h"
#include "../include/utility.h"

DictionaryGenerator::DictionaryGenerator(const std::string &filepath)
{
    this->storefilepath = filepath;
}

DictionaryGenerator::DictionaryGenerator(const std::string &stop_words_filepath, const std::string &filepath)
{
    std::ifstream in(stop_words_filepath);
    ERROR_CHECK(in.is_open() == false, "stop words file open failed\n");
    std::string line;
    while (std::getline(in, line))
        _stop_words.insert(line);
    in.close();
    this->storefilepath = filepath;
}

void DictionaryGenerator::parseEnglish(const std::string &directory_path)
{
    std::ofstream put; // 写入
    put.open(storefilepath, std::ios::out | std::ios::trunc);
    std::function<void(const std::string &)> dfs = [&](const std::string &filepath) -> void
    {
        DIR *dir = ::opendir(filepath.c_str());
        ERROR_CHECK(dir == NULL, "open directory failed. %s\n", errno);
        for (struct dirent *t = ::readdir(dir); t; t = readdir(dir))
        {
            if (filepath == "." || filepath == "..")
                continue;
            if (t->d_type == ::DT_DIR)
            {
                dfs(t->d_name);
                continue;
            }
            std::string line;
            std::ifstream get; // 读取
            get.open(filepath + "/" + t->d_name, std::ios::in);
            ERROR_CHECK(get.is_open() == false, "open file %s falied.\n", filepath.c_str());
            while (std::getline(get, line))
            {
                for (auto &x : line)
                {
                    if (!::isalpha(x))
                    {
                        x = ' ';
                        continue;
                    }
                    x |= (1 << 5);
                }
                char buf[line.size() + 1];
                ::strcpy(buf, line.c_str());
                for (char *token = ::strtok(buf, " "); token; token = ::strtok(NULL, " "))
                    ++count[token];
            }
            get.close();
        }
        closedir(dir);
    };
    dfs(directory_path);
    for (auto &x : count)
        put << x.first << " " << x.second << std::endl;
    put.close();
}

void DictionaryGenerator::parseChinese(const std::string &directory_path)
{
    const char *const DICT_PATH = "./include/cppjieba/dict/jieba.dict.utf8";
    const char *const HMM_PATH = "./include/cppjieba/dict/hmm_model.utf8";
    const char *const USER_DICT_PATH = "./include/cppjieba/dict/user.dict.utf8";
    const char *const IDF_PATH = "./include/cppjieba/dict/idf.utf8";
    const char *const STOP_WORD_PATH = "./include/cppjieba/dict/stop_words.utf8";
    cppjieba::Jieba jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH);

    std::unordered_set<std::string> m_stop;

    std::ofstream put; // 写入

    // 预处理分隔符
    std::string line;
    std::ifstream in("./include/cppjieba/dict/stop_words.utf8");
    ERROR_CHECK(in.is_open() == false, "open stop_words.utf8 failed");
    for (int i = 0; i < 180; i++)
    {
        std::getline(in, line);
        m_stop.insert(line);
    }

    put.open(storefilepath, std::ios::out | std::ios::trunc);
    std::function<void(const std::string &)> dfs = [&](const std::string &filepath) -> void
    {
        DIR *dir = ::opendir(filepath.c_str());
        ERROR_CHECK(dir == NULL, "open directory failed.");
        for (struct dirent *t = ::readdir(dir); t; t = readdir(dir))
        {
            if (filepath == "." || filepath == "..")
                continue;
            if (t->d_type == ::DT_DIR)
            {
                dfs(t->d_name);
                continue;
            }
            std::ifstream get; // 读取
            get.open(filepath + "/" + t->d_name, std::ios::in);
            ERROR_CHECK(get.is_open() == false, "open file %s falied.\n", filepath.c_str());
            while (std::getline(get, line))
            {
                std::vector<std::string> words; // 分词结果
                jieba.Cut(line, words, true);
                for (auto &x : words)
                {
                    if (!(x.c_str()[0] & 0x80))
                    {
                        std::for_each(x.begin(), x.end(), [](char &c)
                                      { if(isalpha(c)) c |= (1 << 5); else c = ' '; });
                        char buf[x.size() + 1];
                        ::strcpy(buf, x.c_str());
                        for (char *token = ::strtok(buf, " "); token; token = ::strtok(NULL, " "))
                            ++count[token];
                        continue;
                    }
                    if (m_stop.count(x))
                        continue;
                    ++count[x];
                }
            }
        }
        closedir(dir);
    };
    dfs(directory_path);
    for (auto &x : count)
    {
        if (x.first.find(""))
            continue;
        put << x.first << " " << x.second << std::endl;
    }
    put.close();
}

void DictionaryGenerator::createIndexDirectory(const std::string &fileapth)
{
    int index = -1;
    std::unordered_map<std::string, std::unordered_set<int>> memo;
    for (auto &x : count)
    {
        ++index;
        auto &str = x.first;
        auto &nums = x.second;
        for (int i = 0; i < str.size(); i++)
        {
            if (str[i] & 0x80)
                memo[str.substr(i, 3)].insert(index), i += 2;
            else
                memo[str.substr(i, 1)].insert(index);
        }
    }
    std::ofstream out(fileapth);
    ERROR_CHECK(out.is_open() == false, "create index directory file falied");
    for (auto &t : memo)
    {
        auto &str = t.first;
        auto &row = t.second;
        out << str;
        for (auto &x : row)
            out << " " << x;
        out << std::endl;
    }
    out.close();
}
