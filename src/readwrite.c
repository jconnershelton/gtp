#include "GTP/buffer.h"

#include <stdlib.h>
#include <unistd.h>

GTPBuffer *_readGTPBuffer(int fd) {
    GTPBuffer *buffer = GTPBuffer_Alloc();
    read(fd, &buffer->bodylen, sizeof(uint64_t));
    
    buffer->body = malloc(buffer->bodylen);
    uint8_t *dataptr = buffer->body;

    int rounds = (int) (buffer->bodylen >> 11); // Equivalent to / 2048
    for (int r = 0; r < rounds; r++) {
        read(fd, dataptr, 2048);
        dataptr += 2048;
    }

    size_t leftover = buffer->bodylen & 2047; // Equivalent to % 2048
    read(fd, dataptr, leftover);

    return buffer;
}

void _writeGTPBuffer(GTPBuffer *buffer, int fd) {
    write(fd, &buffer->bodylen, sizeof(uint64_t));

    const uint8_t *dataptr = buffer->body;

    int rounds = (int) (buffer->bodylen >> 11); // Equivalent to / 2048
    for (int r = 0; r < rounds; r++) {
        write(fd, dataptr, 2048);
        dataptr += 2048;
    }

    size_t leftover = buffer->bodylen & 2047; // Euivalent to % 2048
    write(fd, dataptr, leftover);
}