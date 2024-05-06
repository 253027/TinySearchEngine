#include "../include/PrivatePtotocal.h"
#include <iostream>

PrivateProtocal::PrivateProtocal()
{
    this->head_received = 0;
    this->body = nullptr;
    this->body_size = 0;
}

PrivateProtocal::PrivateProtocal(PrivateProtocal &&val)
    : protocol::ProtocolMessage(std::move(val))
{
    ::memcpy(this->head, val.head, 4);
    this->head_received = val.head_received;
    this->body = val.body;
    this->body_received = val.body_received;
    this->body_size = val.body_size;

    val.head_received = 0;
    val.body = nullptr;
    val.body_size = 0;
}

PrivateProtocal &PrivateProtocal::operator=(PrivateProtocal &&val)
{
    if (this == &val)
        return *this;
    *(protocol::ProtocolMessage *)this = std::move(val);
    ::memcpy(this->head, val.head, 4);
    this->head_received = val.head_received;
    this->body = val.body;
    this->body_received = val.body_received;
    this->body_size = val.body_size;

    val.head_received = 0;
    val.body = nullptr;
    val.body_size = 0;
    return *this;
}

void PrivateProtocal::getMessageBody(void **body, size_t *size)
{
    *body = this->body;
    *size = this->body_size;
}

int PrivateProtocal::setMessageBody(const void *body, size_t size)
{
    void *p = ::malloc(sizeof(char) * size);
    if (!p)
        return -1;
    ::memcpy(p, body, size);
    if (this->body)
        ::free(this->body);
    this->body = (char *)p;
    this->body_size = size;
    this->head_received = 4;
    this->body_received = size;
    return 0;
}

PrivateProtocal::~PrivateProtocal()
{
    ::free(this->body);
}

int PrivateProtocal::encode(struct iovec vectors[], int max)
{
    uint32_t n = ::htonl(this->body_size);
    ::memcpy(this->head, &n, 4);
    vectors[0].iov_base = this->head;
    vectors[0].iov_len = 4;
    vectors[1].iov_base = this->body;
    vectors[1].iov_len = this->body_size;
    return 2;
}

int PrivateProtocal::append(const void *buf, size_t size)
{
    // 获取头部信息直到获取完毕
    if (this->head_received < 4)
    {
        void *p = &this->head[head_received];
        size_t need = std::min(4 - this->head_received, size);
        size -= need, this->head_received += need;
        memcpy(p, buf, need);
        if (size <= 0)
            return 0;
        buf = (const char *)buf + need;
        p = this->head;
        this->body_size = ntohl(*(u_int32_t *)p);
        if (this->body_size > this->size_limit)
        {
            errno = EMSGSIZE;
            return -1;
        }
        this->body = (char *)malloc(sizeof(char) * this->body_size);
        if (!this->body)
            return -1;
        this->body_received = 0;
    }

    size_t need = this->body_size - this->body_received;

    if (size > need)
    {
        errno = EBADMSG;
        return -1;
    }

    memcpy(this->body + this->body_received, buf, size);
    this->body_received += size;
    if (size < need)
        return 0;
    return 1;
}
