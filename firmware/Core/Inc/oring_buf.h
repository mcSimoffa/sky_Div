#ifndef _ORING_BUF_H__
#define _ORING_BUF_H__
#include <stdbool.h>
#include "main.h"

bool oring_buf_put(uint32_t item);
bool oring_buf_get(uint32_t *item);

#endif //_ORING_BUF_H__
