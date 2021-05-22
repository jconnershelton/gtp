#include "GTP/serve.h"
#include "readwrite.h"

#include <pthread.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdio.h> // TODO: REMOVE

static bool GTP_running = false;

static pthread_t GTP_server_thread;
static struct sockaddr_in addr;
static int sockfd;

static void *serveLoop(void *args) {
    GTPBuffer *(*processBuffer)(const GTPBuffer *buffer) = args;
    socklen_t addrlen = sizeof(addr);

    while (GTP_running) {
        int connfd = accept(sockfd, (struct sockaddr*) &addr, &addrlen);
        if (connfd == -1) continue;

        GTPBuffer *inBuffer = readGTPBuffer(connfd);
        GTPBuffer *outBuffer = processBuffer(inBuffer);
        writeGTPBuffer(outBuffer, connfd);

        GTPBuffer_Free(inBuffer);
        GTPBuffer_Free(outBuffer);
        
        close(connfd);
    }

    return NULL;
}

void GTP_Serve(unsigned short port, GTPBuffer *(*processBuffer)(const GTPBuffer *buffer)) {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) return;
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    socklen_t addrlen = sizeof(addr);

    if (bind(sockfd, (struct sockaddr *) &addr, addrlen) != 0) return;
    if (listen(sockfd, 10) != 0) return; // Edit to increase max queue
    
    GTP_running = true;
    pthread_create(&GTP_server_thread, NULL, serveLoop, processBuffer);
}

void GTP_Close(void) {
    GTP_running = false;
    close(sockfd);
    pthread_join(GTP_server_thread, NULL);
}
