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

	if((t > p->item.time5_start) && (t < p->item.time5_end))
	{
		#if ( WARN_EN > 0 )
			warn_judge_set(p, p->item.time5_set, WHICH_WARN_TIMER);
		#endif
	}

	if((t > p->item.time6_start) && (t < p->item.time6_end))
	{
		#if ( WARN_EN > 0 )
			warn_judge_set(p, p->item.time6_set, WHICH_WARN_TIMER);
		#endif
	}

	if((t > p->item.time7_start) && (t < p->item.time7_end))
	{
		#if ( WARN_EN > 0 )
			warn_judge_set(p, p->item.time7_set, WHICH_WARN_TIMER);
		#endif
	}

	if((t > p->item.time8_start) && (t < p->item.time8_end))
	{
		#if ( WARN_EN > 0 )
			warn_judge_set(p, p->item.time8_set, WHICH_WARN_TIMER);
		#endif
	}

	if((t > p->item.time9_start) && (t < p->item.time9_end))
	{
		#if ( WARN_EN > 0 )
			warn_judge_set(p, p->item.time9_set, WHICH_WARN_TIMER);
		#endif
	}

	if((t > p->item.time10_start) && (t < p->item.time10_end))
	{
		#if ( WARN_EN > 0 )
			warn_judge_set(p, p->item.time10_set, WHICH_WARN_TIMER);
		#endif
	}

	if((t > p->item.time11_start) && (t < p->item.time11_end))
	{
		#if ( WARN_EN > 0 )
			warn_judge_set(p, p->item.time11_set, WHICH_WARN_TIMER);
		#endif
	}

	if((t > p->item.time12_start) && (t < p->item.time12_end))
	{
		#if ( WARN_EN > 0 )
			warn_judge_set(p, p->item.time12_set, WHICH_WARN_TIMER);
		#endif
	}

	if((t > p->item.time13_start) && (t < p->item.time13_end))
	{
		#if ( WARN_EN > 0 )
			warn_judge_set(p, p->item.time13_set, WHICH_WARN_TIMER);
		#endif
	}

	if((t > p->item.time14_start) && (t < p->item.time14_end))
	{
		#if ( WARN_EN > 0 )
			warn_judge_set(p, p->item.time14_set, WHICH_WARN_TIMER);
		#endif
	}

	if((t > p->item.time15_start) && (t < p->item.time15_end))
	{
		#if ( WARN_EN > 0 )
			warn_judge_set(p, p->item.time15_set, WHICH_WARN_TIMER);
		#endif
	}

	if((t > p->item.time16_start) && (t < p->item.time16_end))
	{
		#if ( WARN_EN > 0 )
			warn_judge_set(p, p->item.time16_set, WHICH_WARN_TIMER);
		#endif
	}
}

#endif
