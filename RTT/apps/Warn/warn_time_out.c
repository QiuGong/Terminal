#include "warn.h"


// 超时报警 

/* BELL,LED 置位,开	*/
#define TIME_OUT_SET	0x03000300 
#define TIME_OUT_MAX	5


#define NORMAL				0x00
#define TIMEOUT             0x01

// 报警延长时间
#define	WARN_DELAY_TIME		0x03


#if ( WARN_TIME_OUT > 0 )

static rt_uint8_t 	timeout_warn_flag;
static rt_uint32_t 	timeout_warn_flag_last_rec_time;
rt_uint8_t _get_timeout_warn_flag(void)
{
	return 	timeout_warn_flag;
}


void _set_timeout_warn_flag(rt_uint8_t value)
{
	timeout_warn_flag  = value;
}


void warn_time_out(Position p)
{
	rt_uint32_t	t = _get_rs485_running();
   	rt_uint32_t	d = t > p->item.last_rec_time ? t - p->item.last_rec_time : p->item.last_rec_time - t;
	

	if(d > TIME_OUT_MAX)
	{
		#if ( WARN_EN > 0 )
			timeout_warn_flag = TIMEOUT;
			timeout_warn_flag_last_rec_time = p->item.last_rec_time;
			RT_DEBUG_LOG(DEBUG_WARN, ("sensor time out warn.\n"));
			warn_judge_set(p, TIME_OUT_SET, WHICH_WARN_TIME_OUT);
		#endif	
	}
	else
	{
		#if ( WARN_EN > 0 )
			if(timeout_warn_flag == TIMEOUT)
			{
				if(p->item.last_rec_time - timeout_warn_flag_last_rec_time > WARN_DELAY_TIME)
				{
					timeout_warn_flag = NORMAL;
					timeout_warn_flag_last_rec_time = p->item.last_rec_time;
				}
			}
		#endif
	}
}

#endif
