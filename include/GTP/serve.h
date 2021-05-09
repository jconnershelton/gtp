#pragma once

#include "GTP/buffer.h"
#include <stdbool.h>

void GTP_Serve(unsigned short port, GTPBuffer *(*processRequest)(const GTPBuffer *request, bool *serving));