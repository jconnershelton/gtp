#pragma once

#include "GTP/buffer.h"

GTPBuffer *_readGTPBuffer(int fd);
void _writeGTPBuffer(GTPBuffer *buffer, int fd);