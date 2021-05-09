#include "GTP/send.h"
#include "readwrite.h"

#include <sys/socket.h>
#include <arpa/inet.h>

GTPBuffer *GTP_Send(GTPBuffer *buffer, const char *addrStr, unsigned short port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) return NULL;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (inet_pton(AF_INET, addrStr, &addr.sin_addr) != 1) return NULL;
    if (connect(sockfd, (struct sockaddr*) &addr, sizeof(addr)) != 0) return NULL;

    _writeGTPBuffer(buffer, sockfd);
    return _readGTPBuffer(sockfd);
}
