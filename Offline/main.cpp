#include "./include/DictionaryGenerator.h"
#include "./include/WebPageGenerator.h"
#include "./include/WordQuery.h"
#include "./include/utility.h"
#include <nlohmann/json.hpp>
#include "./include/tinyxml2.h"

// 创建索引字典文件
// int main(int argc, char **argv)
// {
//     ERROR_CHECK(argc != 3, "args error which need 2 args : configuration file path, 1(chinese), 0 (english)");
//     char *cwd;
//     char buf[1024];
//
//     // 获取当前工作目录
//     cwd = getcwd(buf, sizeof(buf));
//     std::cout << cwd << "\n";
//     std::ifstream in(argv[1]);
//     nlohmann::json parse;
//     in >> parse;
//     std::string chinese_parse_dir = parse["ChineseParseDirPath"];
//     std::string chinese_store = parse["ChineseStoreFilePath"];
//     std::string chinese_store_index = parse["ChineseIndexStoreFilePath"];
//
//     std::string english_parse_dir = parse["EnglishParseDirPath"];
//     std::string english_stroe = parse["EnglishStoreFilePath"];
//     std::string english_index_store = parse["EnglishIndexStoreFilePath"];
//     std::string english_stop = parse["EnglishStopFilePath"];
//     if (::strcmp(argv[2], "0") == 0)
//     {
//         DictionaryGenerator generator(english_stop, english_stroe);
//         generator.parseEnglish(english_parse_dir);
//         generator.createIndexDirectory(english_index_store);
//     }
//     else
//     {
//         DictionaryGenerator generator(chinese_store);
//         generator.parseChinese(chinese_parse_dir);
//         generator.createIndexDirectory(chinese_store_index);
//     }
//     return 0;
// }

// 字典查询文件
// int main(int argc, char **argv)
// {
//     ERROR_CHECK(argc != 2, "args error which need 2 args : configuration file path");
//
//     std::ifstream in(argv[1]);
//     nlohmann::json parse;
//     in >> parse;
//     std::string chinese_store = parse["ChineseStoreFilePath"];
//     std::string chinese_store_index = parse["ChineseIndexStoreFilePath"];
//     std::string english_stroe = parse["EnglishStoreFilePath"];
//     std::string english_index_store = parse["EnglishIndexStoreFilePath"];
//     WordQuery query;
//     query.Initial(english_index_store, english_stroe, chinese_store_index, chinese_store);
//     std::vector<string> res = query.query("人民");
//     for (auto &x : res)
//         std::cout << x << "\n";
//     return 0;
// }

int main(int argc, char **argv)
{
    ERROR_CHECK(argc != 2, "args error which need one args : configuration file path");
    std::ifstream in(argv[1]);
    nlohmann::json parse;
    in >> parse;
    std::string parse_dir = parse["ParseDirPath"];
    std::string IndexDictionaryFilepath = parse["IndexDictionaryFilepath"];
    std::string DictionaryFilepath = parse["DictionaryFilepath"];
    WebPageGenerator ge(parse_dir, DictionaryFilepath, IndexDictionaryFilepath);
    ge.parse();
    in.close();
    in.open(DictionaryFilepath, std::ios::in);
    char buf[4096];
    in.read(buf, 1994);
    buf[1994] = '\0';
    std::cout << buf << "\n";
    return 0;
}