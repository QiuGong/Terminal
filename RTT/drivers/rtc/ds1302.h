#ifndef DS1302_H
#define DS1302_H

#include <config.h>

void InitClock(void);
void ReadDS1302Clock(rt_uint8_t *p);
void WriteDS1302Clock(rt_uint8_t *p);
void ReadDSRam(rt_uint8_t *p, rt_uint8_t add, rt_uint8_t cnt);
void WriteDSRam(rt_uint8_t *p, rt_uint8_t add, rt_uint8_t cnt);

#endif

