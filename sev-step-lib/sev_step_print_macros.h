#ifndef SEV_STEP_PRINT_MACROS
#define SEV_STEP_PRINT_MACROS

#include <stdio.h>

#define flf_printf(fmt, ...) printf("%s:%d: %s: " fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__);
#endif 