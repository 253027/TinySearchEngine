#include "../include/abstractcontroler.h"
#include "../include/inetaddress.h"

InetAddress AbstractControl::getRemoteAddress(int fd)
{
    return InetAddress::getRemoteAddress(fd);
}