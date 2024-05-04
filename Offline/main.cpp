#include "./include/simhash/Simhasher.hpp"
#include "./include/WebPageGenerator.h"
#include "./include/utility.h"
#include <nlohmann/json.hpp>
#include <string>
#include <sstream>
#include "./include/tinyxml2.h"
#include "./include/DictionaryGenerator.h"
#include "./include/Dictionary.h"

Dictionary *Dictionary::dic = nullptr;

// V2.0创建索引字典文件
// int main()
// {
//     DictionaryGenerator gen("/home/aa/桌面/code/Search_Engine/Offline/conf/English.conf");
//     gen.parse();
//     gen.createIndexDirectory();
//     return 0;
// }

// 字典查询文件
int main(int argc, char **argv)
{
    Dictionary::GetInstance("/home/aa/桌面/code/Search_Engine/Offline/conf/Chinese.conf",
                            "/home/aa/桌面/code/Search_Engine/Offline/conf/English.conf");
    std::vector<std::string> res = Dictionary::GetInstance()->query("爱国");
    for (auto &x : res)
        std::cout << x << "\n";
    return 0;
}

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
//     ge.parse();
// ge.removeDuplicates();
// ge.createInvertIndex();
//
// std::ifstream tttt;
// std::string line;
// in.close();
// in.open(IndexDictionaryFilepath, std::ios::in);
// tttt.open(DictionaryFilepath, std::ios::in);
// while (std::getline(in, line))
// {
//     std::istringstream is(line);
//     int a, b, c;
//     is >> a >> b >> c;
//     tttt.seekg(b);
//     line.resize(c, '\0');
//     tttt.read(line.data(), c);
//     std::cout << line;
// }
//     return 0;
// }
