#pragma once

// Making sure C++ can include this header
#ifdef __cplusplus
extern "C" {
#endif

#include "GTP/buffer.h"
#include <stdbool.h>

void GTP_Serve(unsigned short port, GTPBuffer *(*processRequest)(const GTPBuffer *request));
void GTP_Close(void);

// End of the extern C block
#ifdef __cplusplus
}
#endif