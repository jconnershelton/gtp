#include "GTP/buffer.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>

void GTPBuffer_AddElement(GTPBuffer *buffer, const char *name, const void *data, uint64_t datalen) {
    size_t namelen = strlen(name) + 1; // +1 for null terminator

    // Realloc buffer to fit name and data
    buffer->bodylen += namelen + datalen;
    buffer->body = realloc(buffer->body, buffer->bodylen);

    // Copy name then data into new memory
    strcpy((char *) buffer->body + buffer->bodylen - datalen - namelen, name);
    memcpy((uint8_t *) buffer->body + buffer->bodylen - datalen, data, datalen);
}

void GTPBuffer_AddStringElement(GTPBuffer *buffer, const char *name, const char *str) {
    GTPBuffer_AddElement(buffer, name, str, strlen(str) + 1);
}

GTPElement *GTPBuffer_GetElement(GTPBuffer *buffer, const char *name) {
    const uint8_t *endData = (const uint8_t *) buffer->body + buffer->bodylen;

    size_t namelen;
    size_t elementlen;

    const uint8_t *dataptr = buffer->body;
    while (dataptr < endData) {
        namelen = strnlen((char *) dataptr, buffer->bodylen);
        if (namelen == buffer->bodylen) {
            errno = 1;
            return NULL;
        }

        elementlen = *((uint64_t *) (dataptr + namelen + 1));

        if (strcmp((char *) dataptr, name) == 0) {
            GTPElement *element = malloc(sizeof(GTPElement));
            element->data = (void *) (dataptr + namelen + 1 + sizeof(uint64_t));
            element->size = elementlen;
            
            return element;
        }

        dataptr += namelen + 1 + elementlen + sizeof(uint64_t);
    }

    return NULL;
}

GTPBuffer *GTPBuffer_Alloc(void) {
    GTPBuffer *buffer = malloc(sizeof(GTPBuffer));
    buffer->body = NULL;
    buffer->bodylen = 0;
    return buffer;
}

void GTPBuffer_Free(GTPBuffer *buffer) {
    free(buffer->body);
    free(buffer);
}