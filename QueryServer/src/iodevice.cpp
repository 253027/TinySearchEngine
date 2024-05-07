#include "../include/iodevice.h"
#include "../include/utility.h"

IoDevice::IoDevice(int socket) : _socket(socket) {}

int IoDevice::boundary_recv(std::string &buf)
{
    char buffer[1024];
    int ret = ::recv(_socket, buffer, 4, MSG_WAITALL);
    ERROR_CHECK(ret == -1, "boundary recv error which is -1");
    if (ret == 0)
        return 0;
    uint32_t len = *(uint32_t *)buffer, has_recv = 0;
    while ((ret = ::recv(_socket, buffer, std::min(1024U, len - has_recv), MSG_WAITALL)) > 0)
    {
        if (ret == -1 && (errno == EAGAIN))
            continue;
        else if (ret == -1)
            return -1;
        buf.append(buffer, ret), has_recv += ret;
    }
    return has_recv;
}

int IoDevice::boundary_send(const std::string &buf)
{
    uint32_t len = buf.size(), has_send = 0;
    int ret = ::send(_socket, &len, sizeof(len), 0);
    ERROR_CHECK(ret == -1, "boundary send size error");
    ret = send(buf);
    ERROR_CHECK(ret == -1, "boundary send data error");
    return ret;
}

int IoDevice::send(const std::string &buf)
{
    int ret;
    uint32_t len = buf.size(), has_send = 0;
    while ((ret = ::send(_socket, buf.data() + has_send, std::min(1024U, len - has_send), MSG_DONTWAIT)) > 0)
    {
        if (ret == -1 && (errno == EWOULDBLOCK))
            continue;
        else if (ret == -1)
            return -1;
        has_send += ret;
    }
    return has_send;
}

int IoDevice::recv(std::string &buf)
{
    int ret = 0, bytes_read = 0;
    char buffer[1024];
    while ((ret = ::read(_socket, buffer, 1024)) > 0)
    {
        if (ret == -1)
            return (errno == EAGAIN) ? bytes_read : -1;
        buf.append(buffer, ret), bytes_read += ret;
    }
    return bytes_read;
}
