#pragma once

// Making sure C++ can include this header
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

typedef struct {
    void *data;
    size_t size;
} GTPElement;

typedef struct {
    uint64_t bodylen;
    void *body;
} GTPBuffer;

void GTPBuffer_AddElement(GTPBuffer *buffer, const char *name, const void *data, uint64_t size);
void GTPBuffer_AddStringElement(GTPBuffer *buffer, const char *name, const char *string);

GTPElement *GTPBuffer_GetElement(const GTPBuffer *buffer, const char *name);
const char *GTPBuffer_GetStringElement(const GTPBuffer *buffer, const char *name);

GTPBuffer *GTPBuffer_Alloc(void);
void GTPBuffer_Free(GTPBuffer *buffer);

// End of the extern C block 
#ifdef __cplusplus
}
#endif