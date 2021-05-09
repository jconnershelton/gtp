#include "GTP/serve.h"
#include "readwrite.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void GTP_Serve(unsigned short port, GTPBuffer *(*processBuffer)(const GTPBuffer *buffer, bool *serving)) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) return;
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    socklen_t addrlen = sizeof(addr);

    if (bind(sockfd, (struct sockaddr*) &addr, addrlen) != 0) return;
    if (listen(sockfd, 10) != 0) return; // Edit to increase max queue

    bool serving = true;
    while (serving) {
        int connfd = accept(sockfd, (struct sockaddr*) &addr, &addrlen);
        if (connfd == -1) continue;

        GTPBuffer *inBuffer = _readGTPBuffer(connfd);
        GTPBuffer *outBuffer = processBuffer(inBuffer, &serving);
        _writeGTPBuffer(outBuffer, connfd);

        GTPBuffer_Free(inBuffer);
        GTPBuffer_Free(outBuffer);
        
        close(connfd);
    }

    close(sockfd);
}

