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
#include <arpa/inet.h>

Dictionary *Dictionary::dic = nullptr;
WebPageQuery webquery("./conf/WebPageGenerator.conf");

void task(void *eventloop, int socket, int type, const std::string &query)
{
    // std::cout << pthread_self() << std::endl;
    //  std::this_thread::sleep_for(std::chrono::seconds(1));
    std::string res;
    // 关键词推荐
    if (type == 1)
        res = Dictionary::GetInstance()->query(query), type = 100;
    else if (type == 2)
        res = webquery.query(query), type = 200;
    std::cout << res << "\n";
    std::string data(8 + res.size(), '\0');
    *(int *)data.data() = ::htonl(4 + res.size());
    *((int *)data.data() + 1) = type;
    ::memcpy(data.data() + 8, res.data(), res.size());

    EventLoop *loop = (EventLoop *)eventloop;
    loop->appendSendPoll(std::make_pair(socket, data));
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
    server = new TcpServer("192.168.2.169", 9190);
    // server = new TcpServer("127.0.0.1", 9191);
    pool = new ThreadPool(5, 50000);
    server->start();
    return 0;
}
