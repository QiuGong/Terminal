#ifndef __UTIL_H
#define __UTIL_H

#include <rtthread.h>

/*****************string.c*******************/
rt_uint16_t formula_temp_16(rt_uint16_t source);
rt_uint16_t formula_temp_8(rt_uint8_t source);
rt_uint16_t formula_ph_16(rt_uint16_t source);
rt_uint16_t formula_ph_8(rt_uint8_t source);
rt_uint16_t formula_do_16(rt_uint16_t source);
rt_uint16_t formula_do_8(rt_uint8_t source);

#endif
