#pragma once

// Making sure C++ can include this header
#ifdef __cplusplus
extern "C" {
#endif

#include "GTP/buffer.h"

GTPBuffer *GTP_Send(GTPBuffer *buffer, const char *addrStr, unsigned short port);

// End of the extern C block
#ifdef __cplusplus
}
#endif