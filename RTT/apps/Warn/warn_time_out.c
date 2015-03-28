#include "warn.h"


// 超时报警 

/* BELL,LED 置位,开	*/
#define TIME_OUT_SET	0x03000300 
#define TIME_OUT_MAX	5


#define NORMAL				0x00
#define TIMEOUT             0x01


#if ( WARN_TIME_OUT > 0 )

static rt_uint8_t 	timeout_warn_flag;
rt_uint8_t _get_timeout_warn_flag(void)
{
	return 	timeout_warn_flag;
}


void warn_time_out(Position p)
{
	rt_uint32_t	t = _get_rs485_running();
   	rt_uint32_t	d = t > p->item.last_rec_time ? t - p->item.last_rec_time : p->item.last_rec_time - t;
	

	if(d > TIME_OUT_MAX)
	{
		#if ( WARN_EN > 0 )
			timeout_warn_flag = TIMEOUT;
			RT_DEBUG_LOG(DEBUG_WARN, ("sensor time out warn.\n"));
			warn_judge_set(p, TIME_OUT_SET, WHICH_WARN_TIME_OUT);
		#endif	
	}
	else
	{
		#if ( WARN_EN > 0 )
			if(timeout_warn_flag == TIMEOUT)
			{
				timeout_warn_flag = NORMAL;
			}
		#endif
	}
}

#endif
