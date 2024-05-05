#include "./include/simhash/Simhasher.hpp"
#include "./include/WebPageGenerator.h"
#include "./include/utility.h"
#include <nlohmann/json.hpp>
#include <string>
#include <sstream>
#include "./include/tinyxml2.h"
#include "./include/DictionaryGenerator.h"
#include "./include/Dictionary.h"
#include "./include/WebPageQuery.h"

Dictionary *Dictionary::dic = nullptr;

// V2.0创建索引字典文件
// int main()
// {
//     DictionaryGenerator gen("/home/aa/桌面/code/Search_Engine/Offline/conf/English.conf");
//     gen.parse();
//     gen.createIndexDictionary();
//     return 0;
// }

// 字典查询文件
// int main(int argc, char **argv)
// {
//     Dictionary::GetInstance("/home/aa/桌面/code/Search_Engine/Offline/conf/Chinese.conf",
//                             "/home/aa/桌面/code/Search_Engine/Offline/conf/English.conf");
//     std::vector<std::string> res = Dictionary::GetInstance()->query("爱国");
//     for (auto &x : res)
//         std::cout << x << "\n";
//     return 0;
// }

// 网页偏移库
// int main(int argc, char **argv)
// {
//     WebPageGenerator ge("/home/aa/桌面/code/Search_Engine/Offline/conf/WebPageGenerator.conf");
//     ge.parse();
//     ge.removeDuplicates();
//     ge.createInvertIndex();
//     return 0;
// }

// 网页查询
int main()
{
    WebPageQuery query("/home/aa/桌面/code/Search_Engine/Offline/conf/WebPageGenerator.conf");
    std::string res = query.query("美食");
    std::ofstream out("temp.json");
    out << res;
    out.close();
    return 0;
}
