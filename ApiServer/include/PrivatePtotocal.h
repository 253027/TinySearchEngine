#ifndef __PRIVATEPROTOCAL_H_HEAD__
#define __PRIVATEPROTOCAL_H_HEAD__

#include <algorithm>
#include <arpa/inet.h>
#include <stdlib.h>
#include <workflow/ProtocolMessage.h>

class PrivateProtocal : public protocol::ProtocolMessage
{
public:
    PrivateProtocal();

    PrivateProtocal(PrivateProtocal &&val);

    PrivateProtocal &operator=(PrivateProtocal &&val);

    int setMessageBody(const void *body, size_t size);

    void getMessageBody(void **body, size_t *size);

    ~PrivateProtocal();

private:
    int encode(struct iovec vectors[], int max) override;

    int append(const void *buf, size_t size) override;

    char head[4];

    size_t head_received;

    char *body;

    size_t body_received;

    size_t body_size;
};

#endif //__PRIVATEPROTOCAL_H_HEAD__