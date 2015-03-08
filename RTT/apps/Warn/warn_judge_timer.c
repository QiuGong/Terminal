#include "warn.h"
#include "rtc.h"


#if ( WARN_TIMER > 0 )

void warn_judge_timer(Position p)
{
	rt_uint16_t t = get_hh_mm();
	
	if((t > p->item.time1_start) && (t < p->item.time1_end))
	{
		#if ( WARN_EN > 0 )	
			warn_judge_set(p, p->item.time1_set, WHICH_WARN_TIMER);
		#endif		
	}

	if((t > p->item.time2_start) && (t < p->item.time2_end))
	{
		#if ( WARN_EN > 0 )	
			warn_judge_set(p, p->item.time2_set, WHICH_WARN_TIMER);
		#endif
	}

	if((t > p->item.time3_start) && (t < p->item.time3_end))
	{
		#if ( WARN_EN > 0 )
			warn_judge_set(p, p->item.time3_set, WHICH_WARN_TIMER);
		#endif
	}

	if((t > p->item.time4_start) && (t < p->item.time4_end))
	{
		#if ( WARN_EN > 0 )
			warn_judge_set(p, p->item.time4_set, WHICH_WARN_TIMER);
		#endif
	}
}

#endif
