#include "./include/tcpserver.h"
#include "./include/threadpool.h"
#include "./include/Dictionary.h"
#include "./include/DictionaryGenerator.h"
#include "./include/WebPageGenerator.h"
#include "./include/WebPageQuery.h"
#include "./include/tcpcontroler.h"
#include "./include/eventloop.h"
#include "./include/CashManger.h"
#include "./include/utility.h"
#include "./include/Cache.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <arpa/inet.h>

TcpServer *server;
ThreadPool *pool;
CashManger *CashManger::_instance = nullptr;
Dictionary *Dictionary::dic = nullptr;
// WebPageQuery webquery("./conf/WebPageGenerator.conf");

void task(int index, void *eventloop, int socket, int type, const std::string &query)
{
    // Cache &cache = CashManger::GetInstance()->get(index);
    // std::string res;
    // std::cout << "线程 " << index << " 进行查询: " << query << "\n";
    // int ret = 1; // cache.query(query, res);
    // if (ret == 1)
    // {
    //     // 关键词推荐
    //     if (type == 1)
    //         res = Dictionary::GetInstance()->query(query), type = 100;
    //     else if (type == 2)
    //         res = webquery.query(query), type = 200;
    //     std::cout << "无缓存";
    // }
    // cache.insert(query, res);
    // std::cout << "查询"
    //           << "\n";
    // std::cout << index << " 取得 " << res.size() << "\n";
    // std::string data(8 + res.size(), '\0');
    // *(int *)data.data() = ::htonl(4 + res.size());
    // *((int *)data.data() + 1) = type;
    // ::memcpy(data.data() + 8, res.data(), res.size());

    // EventLoop *loop = (EventLoop *)eventloop;
    // loop->appendSendPoll(std::make_pair(socket, data));
}

// V2.0创建索引字典文件
int main(int argc, char **argv)
{
    if (argc != 2)
        ERROR_CHECK(argc != 2, "argument error, useage %s <filename.conf>", argv[0]);
    DictionaryGenerator gen(argv[1]);
    gen.parse();
    gen.createIndexDictionary();
    return 0;
}

// 字典查询文件
// int main(int argc, char **argv)
// {
//     Dictionary::GetInstance("./conf/Chinese.conf",
//                             "./conf/English.conf");
//     std::vector<std::string> res = Dictionary::GetInstance()->query("民族");
//     for (auto &x : res)
//         std::cout << x << "\n";
//     return 0;
// }

// 网页偏移库
// int main(int argc, char **argv)
// {
//     if (argc != 2)
//         ERROR_CHECK(argc != 2, "argument error, useage %s <filename.conf>", argv[0]);
//     WebPageGenerator ge(argv[1]);
//     ge.parse();
//     ge.removeDuplicates();
//     ge.createInvertIndex();
//     return 0;
// }

// 网页查询
// int main()
// {
//     WebPageQuery query("/home/aa/桌面/code/Search_Engine/QueryServer/conf/WebPageGenerator.conf");
//     std::string res = query.query("天气");
//     std::ofstream out("temp.json");
//     out << nlohmann::ordered_json().parse(res).dump(4);
//     out.close();
//     return 0;
// }
