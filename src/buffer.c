#include "GTP/buffer.h"

#include <stdlib.h>
#include <string.h>
#include <errno.h>

void GTPBuffer_AddElement(GTPBuffer *buffer, const char *name, const void *data, uint64_t datalen) {
    size_t nameOffset = buffer->bodylen;
    buffer->bodylen += strlen(name) + 1;

    size_t datalenOffset = buffer->bodylen;
    buffer->bodylen += sizeof(datalen);

    size_t dataOffset = buffer->bodylen;
    buffer->bodylen += datalen;

    // Realloc buffer to fit name and data
    buffer->body = realloc(buffer->body, buffer->bodylen);
    char *dataptr = buffer->body;
    
    // Copy name then data into new memory
    strcpy(dataptr + nameOffset, name);
    memcpy(dataptr + datalenOffset, &datalen, sizeof(datalen));
    memcpy(dataptr + dataOffset, data, datalen);
}

void GTPBuffer_AddStringElement(GTPBuffer *buffer, const char *name, const char *str) {
    GTPBuffer_AddElement(buffer, name, str, strlen(str) + 1);
}

GTPElement *GTPBuffer_GetElement(const GTPBuffer *buffer, const char *name) {
    const char *endptr = (const char *) buffer->body + buffer->bodylen;
    const char *dataptr = buffer->body;

    size_t namelen;
    size_t elementlen;

    while (dataptr < endptr) {
        namelen = strnlen(dataptr, endptr - dataptr) + 1;
        elementlen = *((uint64_t *) (dataptr + namelen));

        if (strcmp((char *) dataptr, name) == 0) {
            GTPElement *element = malloc(sizeof(GTPElement));
            element->data = (void *) (dataptr + namelen + sizeof(elementlen));
            element->size = elementlen;

            return element;
        }

        dataptr += namelen + sizeof(elementlen) + elementlen;
    }

    return NULL;
}

const char *GTPBuffer_GetStringElement(const GTPBuffer *buffer, const char *name) {
    return (const char *) GTPBuffer_GetElement(buffer, name)->data;
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