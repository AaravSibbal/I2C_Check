#ifndef DEF_H
#define DEF_H

#include <stdint.h>

#define __IO (volatile)
#define CREATE_ADDRESS(base, offset) (*(__IO uint32_t *)((base) + (offset)))
#define TRUE 1
#define FALSE 0

#endif