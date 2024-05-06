#include "../include/eventloop.h"
#include "../include/epoll.h"
#include "../include/channel.h"
#include "../include/tcpcontroler.h"
#include "../include/utility.h"
#include "../include/threadpool.h"
#include "../include/utility.h"
#include <sys/eventfd.h>
#include <arpa/inet.h>

extern ThreadPool *pool;
extern void task(void *eventloop, int socket, int type, const std::string &query);

EventLoop::EventLoop(int server_sock) : _server_sock(server_sock), stop(false), _epoll(new Epoll())
{
    event_fd = ::eventfd(10, 0);
    printf("%d\n", event_fd);
}

EventLoop::~EventLoop()
{
    for (auto &x : _connect_map)
        close(x.first);
}

void EventLoop::loop()
{
    while (!stop)
    {
        std::vector<Channel *> event = _epoll->epoll_wait();
        for (auto &x : event)
            x->CallBack();
    }
}

void EventLoop::setStop()
{
    stop = true;
}

void EventLoop::updateChannel(Channel *ch)
{
    _epoll->updateChannel(ch);
}

void EventLoop::recordNewConnection(int fd, TcpControler *tcp)
{
    _connect_map[fd].reset(tcp);
}

void EventLoop::handleReadConnection(int client_sock)
{
    auto connection = _connect_map.find(client_sock);
    if (connection == _connect_map.end())
        return;
    auto tcp = connection->second;
    std::string buf;
    int ret = tcp->recv(buf);
    ERROR_CHECK(ret == -1, "tcp read error which socket id is %d", client_sock);
    if (ret == 0)
    {
        close(client_sock);
        _connect_map.erase(connection);
        printf("client closed\n\n");
        return;
    }
    // tcp->send(buf);
    printf("socket id: %d\n", client_sock);
    // 添加头部时需要转换为网络字节序！！！
    //  workflow的协议格式是4字节头部 + 需要的消息，但是发送消息时为了区分报文，故在自定义协议头部加4字节协议用于接受协议消息
    int len = ntohl(*(int *)buf.data());
    int type = *((int *)buf.data() + 1);
    std::string str(buf.data() + 8, len - 4);
    pool->appendThreadPool(std::bind(&task, this, client_sock, type, str));
}

void EventLoop::appendSendPoll(const std::pair<int, std::string> &data)
{
    {
        std::lock_guard<std::mutex> lock(_send_mutex);
        _send_pool.push_back(data);
    }
    this->notify();
}

void EventLoop::send()
{
    printf("触发 %d\n", __LINE__);
    std::vector<std::pair<int, std::string>> memo;
    {
        std::lock_guard<std::mutex> lock(_send_mutex);
        memo.swap(_send_pool);
    }
    for (auto &x : memo)
    {
        _connect_map[x.first]->send(x.second);
        printf("发送 %d\n", x.second.size());
    }
}

void EventLoop::notify()
{
    uint64_t one = 1;
    ssize_t ret = write(event_fd, &one, sizeof(uint64_t));
    ERROR_CHECK(ret != sizeof(uint64_t), "notify failed");
    printf("触发 %d\n", __LINE__);
}
