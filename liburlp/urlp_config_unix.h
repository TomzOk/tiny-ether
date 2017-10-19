#ifndef URLP_CONFIG_LINUX_EMU_H_
#define URLP_CONFIG_LINUX_EMU_H_

#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define URLP_CONFIG_ANYSIZE_ARRAY 1
#define URLP_IS_BIGENDIAN 1

#define urlp_malloc_fn malloc
#define urlp_free_fn free
#define urlp_clz_fn __builtin_clz

#endif