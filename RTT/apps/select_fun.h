#ifndef __SELECT_FUN_H
#define __SELECT_FUN_H

#include <rtthread.h>
#include <config.h>


typedef struct
{
	rt_uint8_t	warn_value;
	rt_uint8_t	warn_rep;
	rt_uint8_t	warn_timer;
	rt_uint8_t	warn_relate;
	rt_uint8_t	warn_time_out;
	rt_uint8_t	warn_power_drop;

}SelectFun;


SelectFun get_select_fun(void);
void read_select_fun_from_flash(void);

#endif


