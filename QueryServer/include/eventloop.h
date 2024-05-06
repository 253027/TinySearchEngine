#ifndef __EVENTLOOP_HEAD_H__
#define __EVENTLOOP_HEAD_H__

#include <vector>
#include <memory>
#include <unistd.h>
#include <string>
#include <mutex>
#include <unordered_map>
#include <functional>

class Epoll;
class Channel;
class TcpControler;
class ThreadPool;

class EventLoop
{
public:
    EventLoop(int server_sock);

    ~EventLoop();

    void loop();

    void setStop();

    void updateChannel(Channel *ch);

    void recordNewConnection(int fd, TcpControler *tcp);

    void handleReadConnection(int client_sock);

    void appendSendPoll(const std::pair<int, std::string> &data);

    void send();
    // 通知文件描述符
    void notify();

    int event_fd;

private:
    bool stop;

    int _server_sock;

    std::shared_ptr<Epoll> _epoll;

    std::unordered_map<int, std::shared_ptr<TcpControler>> _connect_map;

    // 待发送数据互斥锁
    std::mutex _send_mutex;

    // 待发送数据集合
    std::vector<std::pair<int, std::string>> _send_pool;
};

#endif //__EVENTLOOP_HEAD_H__