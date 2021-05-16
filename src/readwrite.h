#pragma once

#include "GTP/buffer.h"

GTPBuffer *readGTPBuffer(int fd);
void writeGTPBuffer(GTPBuffer *buffer, int fd);