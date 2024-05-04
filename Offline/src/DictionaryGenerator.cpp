#include "../include/DictionaryGenerator.h"
#include "../include/utility.h"

DictionaryGenerator::DictionaryGenerator(const std::string &filepath)
    : config(new Configuration(filepath))
{
    ;
}

void DictionaryGenerator::parse()
{
    std::ofstream file; // 建立词频文件
    std::string filepath = config->getFilePath("storedFilePath");
    if (config->getFilePath("type") == "English")
        tool.reset(new EnglishSpilt(config->getFilePath("stopWordFilePath")));
    else if (config->getFilePath("type") == "Chinese")
        tool.reset(new JieBaSpilt(config->getFilePath("stopWordFilePath")));
    else
        ERROR_CHECK(1 == 1, "error configuration file");
    file.open(filepath, std::ios::trunc | std::ios::out);
    ERROR_CHECK(file.is_open() == false, "open directory failed. %s\n", filepath.c_str());
    recursionParse(config->getFilePath("parseDirectoryPath"));
    for (auto &x : count)
        file << x.first << " " << x.second << std::endl;
    file.close();
}

void DictionaryGenerator::createIndexDirectory()
{
    int line = 0;
    std::string filepath = config->getFilePath("indexStoreFilePath");
    std::ofstream file(filepath);
    ERROR_CHECK(file.is_open() == false, "create %s failed", filepath.c_str());
    std::unordered_map<std::string, std::unordered_set<int>> res;
    for (auto &x : count)
    {
        auto &str = x.first;
        for (int i = 0; i < str.size(); i++)
        {
            if (str[i] & 0x80)
                res[str.substr(i, 3)].insert(line), i += 2;
            else
                res[str.substr(i, 1)].insert(line);
        }
        line++;
    }
    for (auto &item : res)
    {
        file << item.first;
        for (auto &t : item.second)
            file << " " << t;
        file << std::endl;
    }
    file.close();
}

void DictionaryGenerator::recursionParse(const std::string &filepath)
{
    DIR *dir = ::opendir(filepath.c_str());
    ERROR_CHECK(dir == NULL, "open directory %s failed", filepath.c_str());
    for (struct dirent *path = ::readdir(dir); path; path = ::readdir(dir))
    {
        if (!::strcmp(".", path->d_name) || !::strcmp("..", path->d_name))
            continue;
        if (path->d_type == ::DT_DIR)
        {
            recursionParse(path->d_name);
            continue;
        }
        // 打开并读取文件
        std::ifstream file;
        std::string _filepath = filepath + "/" + path->d_name;
        file.open(_filepath, std::ios::in);
        ERROR_CHECK(file.is_open() == false, "open file %s failed", _filepath.c_str());
        for (std::string line; std::getline(file, line);)
        {
            std::vector<std::string> words = tool->cut(line);
            for (int i = 0; i < words.size(); i++)
                ++count[words[i]];
        }
        file.close();
    }
}