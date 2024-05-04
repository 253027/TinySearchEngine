#include "./include/simhash/Simhasher.hpp"
#include "./include/WebPageGenerator.h"
#include "./include/WordQuery.h"
#include "./include/utility.h"
#include <nlohmann/json.hpp>
#include <sstream>
#include "./include/tinyxml2.h"
#include "./include/DictionaryGenerator.h"

// V1.0创建索引字典文件
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

// V2.0创建索引字典文件
// int main()
// {
//     DictionaryGenerator gen("/home/aa/桌面/code/Search_Engine/Offline/conf/English.conf");
//     gen.parse();
//     gen.createIndexDirectory();
//     return 0;
// }

// 字典查询文件
// int main(int argc, char **argv)
// {
//     ERROR_CHECK(argc != 2, "args error which need 2 args : configuration file path");

//     std::ifstream in(argv[1]);
//     nlohmann::json parse;
//     in >> parse;
//     std::string chinese_store = parse["ChineseStoreFilePath"];
//     std::string chinese_store_index = parse["ChineseIndexStoreFilePath"];
//     std::string english_stroe = parse["EnglishStoreFilePath"];
//     std::string english_index_store = parse["EnglishIndexStoreFilePath"];
//     WordQuery query;
//     query.Initial(english_index_store, english_stroe, chinese_store_index, chinese_store);
//     std::vector<string> res = query.query("中国");
//     for (auto &x : res)
//         std::cout << x << "\n";
//     return 0;
// }

// 网页偏移库
// int main(int argc, char **argv)
// {
//     ERROR_CHECK(argc != 2, "args error which need one args : configuration file path");
//     std::ifstream in(argv[1]);
//     nlohmann::json parse;
//     in >> parse;
//     std::string parse_dir = parse["ParseDirPath"];
//     std::string IndexDictionaryFilepath = parse["IndexDictionaryFilepath"];
//     std::string DictionaryFilepath = parse["DictionaryFilepath"];
//     WebPageGenerator ge(parse_dir, DictionaryFilepath, IndexDictionaryFilepath);
//     // ge.parse();
//     // ge.removeDuplicates();
//     ge.createInvertIndex();

//     // std::ifstream tttt;
//     // std::string line;
//     // in.close();
//     // in.open(IndexDictionaryFilepath, std::ios::in);
//     // tttt.open(DictionaryFilepath, std::ios::in);
//     // while (std::getline(in, line))
//     // {
//     //     std::istringstream is(line);
//     //     int a, b, c;
//     //     is >> a >> b >> c;
//     //     tttt.seekg(b);
//     //     line.resize(c, '\0');
//     //     tttt.read(line.data(), c);
//     //     std::cout << line;
//     // }
//     return 0;
// }
