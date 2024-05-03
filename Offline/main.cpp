#include "./include/DictionaryGenerator.h"
#include "./include/utility.h"
#include <nlohmann/json.hpp>
#include <unistd.h>

// 创建索引字典文件
int main(int argc, char **argv)
{
    ERROR_CHECK(argc != 3, "args error which need 2 args : configuration file path, 1(chinese), 0 (english)");
    char *cwd;
    char buf[1024];

    // 获取当前工作目录
    cwd = getcwd(buf, sizeof(buf));
    std::cout << cwd << "\n";
    std::ifstream in(argv[1]);
    nlohmann::json parse;
    in >> parse;
    std::string chinese_parse_dir = parse["ChineseParseDirPath"];
    std::string chinese_store = parse["ChineseStoreFilePath"];
    std::string chinese_store_index = parse["ChineseIndexStoreFilePath"];

    std::string english_parse_dir = parse["EnglishParseDirPath"];
    std::string english_stroe = parse["EnglishStoreFilePath"];
    std::string english_index_store = parse["EnglishIndexStoreFilePath"];
    std::string english_stop = parse["EnglishStopFilePath"];
    if (::strcmp(argv[2], "0") == 0)
    {
        DictionaryGenerator generator(english_stop, english_stroe);
        generator.parseEnglish(english_parse_dir);
        generator.createIndexDirectory(english_index_store);
    }
    else
    {
        DictionaryGenerator generator(chinese_store);
        generator.parseChinese(chinese_parse_dir);
        generator.createIndexDirectory(chinese_store_index);
    }
    return 0;
}