#include "../include/DictionaryGenerator.h"
#include "../include/utility.h"

DictionaryGenerator::DictionaryGenerator(const std::string &stop_words_filepath)
{
    std::ifstream in(stop_words_filepath);
    ERROR_CHECK(in.is_open() == false, "stop words file open failed");
    std::string line;
    while (std::getline(in, line))
        _stop_words.insert(line);
    in.close();
}

void DictionaryGenerator::parseEnglish(const std::string &directory_path)
{
    std::ofstream put; // 写入
    std::unordered_map<std::string, int> count;
    put.open("./data/res.dat", std::ios::out | std::ios::trunc);
    std::function<void(const std::string &)> dfs = [&](const std::string &filepath) -> void
    {
        DIR *dir = ::opendir(filepath.c_str());
        ERROR_CHECK(dir == NULL, "open directory failed. %s", errno);
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
            ERROR_CHECK(get.is_open() == false, "open file %s falied.", filepath.c_str());
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
}
