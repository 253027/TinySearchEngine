#include "./include/tcpserver.h"
#include "./include/threadpool.h"
#include "./include/Dictionary.h"
#include "./include/WebPageQuery.h"
#include "./include/tcpcontroler.h"
#include "./include/eventloop.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

Dictionary *Dictionary::dic = nullptr;
WebPageQuery webquery("/home/aa/桌面/code/Search_Engine/QueryServer/conf/WebPageGenerator.conf");

void task(TcpControler &tcp, int type, const std::string &query)
{
    // std::cout << pthread_self() << std::endl;
    //  std::this_thread::sleep_for(std::chrono::seconds(1));
    std::string res;
    // 关键词推荐
    if (type == 1)
        res = Dictionary::GetInstance()->query(query);
    else if (type == 2)
        res = webquery.query(query);
    std::cout << res << "\n";
    // loop.appendSendPoll(std::bind(&TcpControler::send, std::ref(tcp), res, res.size()));
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

    Dictionary::GetInstance("./conf/Chinese.conf",
                            "./conf/English.conf");
    signal(SIGINT, stop);
    // server = new TcpServer("192.168.2.169", 9190);
    server = new TcpServer("127.0.0.1", 9191);
    pool = new ThreadPool(5, 50000);
    server->start();
    return 0;
}
