#ifndef __GSM_DRIVER_H__
#define __GSM_DRIVER_H__

#include <rtthread.h>
#include "config.h"


void light_init(void);
void light_off(void);
void light_on(void);

void gsm_driver_init(void);
void gsm_reset(void);

#endif
