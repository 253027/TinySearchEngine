#include "./include/tcpserver.h"
#include "./include/threadpool.h"
#include "./include/Dictionary.h"
#include "./include/WebPageQuery.h"
#include "./include/tcpcontroler.h"
#include "./include/eventloop.h"
#include "./include/CashManger.h"
#include "./include/Cache.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <arpa/inet.h>

Dictionary *Dictionary::dic = nullptr;
CashManger *CashManger::_instance = nullptr;
WebPageQuery webquery("./conf/WebPageGenerator.conf");

void task(int index, void *eventloop, int socket, int type, const std::string &query)
{
    // Cache &cache = CashManger::GetInstance()->get(index);
    std::string res;
    std::cout << "线程 " << index << " 进行查询: " << query << "\n";
    int ret = 1; // cache.query(query, res);
    if (ret == 1)
    {
        // 关键词推荐
        if (type == 1)
            res = Dictionary::GetInstance()->query(query), type = 100;
        else if (type == 2)
            res = webquery.query(query), type = 200;
        std::cout << "无缓存";
    }
    // cache.insert(query, res);
    std::cout << "查询"
              << "\n";
    std::cout << index << " 取得 " << res.size() << "\n";
    std::string data(8 + res.size(), '\0');
    *(int *)data.data() = ::htonl(4 + res.size());
    *((int *)data.data() + 1) = type;
    ::memcpy(data.data() + 8, res.data(), res.size());

    EventLoop *loop = (EventLoop *)eventloop;
    loop->appendSendPoll(std::make_pair(socket, data));
}

void update(int sig)
{
    static int swap_size = 0;
    int size = CashManger::GetInstance()->size();
    for (int i = 0; i < size; i++)
    {
        CashManger::GetInstance()->get(i).swap();
    }
    std::cout << "第" << ++swap_size << "次交换"
              << "\n";
    alarm(15);
}

TcpServer *server;
ThreadPool *pool;

void stop(int sig)
{
    if (sig != SIGINT && sig != SIGKILL)
        return;
    server->stop();
    pool->stop();
    std::cout << "server closed" << std::endl;
}

int main()
{
    // pid_t pid = fork();
    // if (pid < 0)
    //     exit(EXIT_FAILURE);
    // if (pid > 0)
    //     exit(EXIT_SUCCESS);
    // if (setsid() < 0)
    //     exit(EXIT_FAILURE);
    // for (long x = sysconf(_SC_OPEN_MAX); x >= 0; x--)
    //     ::close(x);
    // ::open("/dev/null", O_RDWR);
    // dup(0);
    // dup(0);
    CashManger::GetInstance();
    Dictionary::GetInstance("./conf/Chinese.conf",
                            "./conf/English.conf");

    signal(SIGALRM, update);
    signal(SIGINT, stop);
    // alarm(15);

    // server = new TcpServer("192.168.2.169", 9190);
    server = new TcpServer("127.0.0.1", 9191);
    pool = new ThreadPool(5, 50000);
    server->start();
    return 0;
}
