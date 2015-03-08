#include "warn.h"
#include "rtc.h"


#if ( WARN_TIME_OUT > 0 )

/* BELL,LED ÖÃÎ»,¿ª	*/
#define TIME_OUT_SET	0x03000300 
#define TIME_OUT_MAX	5


void warn_time_out(Position p)
{
	rt_uint32_t	t = get_rs485_timer();
   	rt_uint32_t	d = t > p->item.last_rec_time ? t - p->item.last_rec_time : p->item.last_rec_time - t;
	
	if(d > TIME_OUT_MAX)
	{
		#if ( WARN_EN > 0 )
			RT_DEBUG_LOG(DEBUG_WARN, ("sensor time out warn.\n"));
			warn_judge_set(p, TIME_OUT_SET, WHICH_WARN_TIME_OUT);
		#endif	
	}
}

#endif
