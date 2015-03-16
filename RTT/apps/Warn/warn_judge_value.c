#include "warn.h"


#if ( WARN_VALUE > 0 )

void warn_judge_value(Position p)
{
	// sensor1_ch1 小于最小值，大于最大值（电流）
	if((p->item.sensor1_ch1 < p->item.sensor1_ch1_min) || (p->item.sensor1_ch1 > p->item.sensor1_ch1_max))
	{
		#if ( WARN_EN > 0 )	
			warn_judge_set(p, p->item.sensor1_ch1_set, WHICH_WARN_VALUE);
		#endif	
	}

	// sensor1_ch2 小于最小值，大于最大值（溶氧度）
	if((p->item.sensor1_ch2 < p->item.sensor1_ch2_min) || (p->item.sensor1_ch2 > p->item.sensor1_ch2_max))
	{
		#if ( WARN_EN > 0 )
			warn_judge_set(p, p->item.sensor1_ch2_set, WHICH_WARN_VALUE);
		#endif
	}

	// sensor2_ch1 小于最小值，大于最大值（PH）
	if((p->item.sensor2_ch1 < p->item.sensor2_ch1_min) || (p->item.sensor2_ch1 > p->item.sensor2_ch1_max))
	{
		#if ( WARN_EN > 0 )
			warn_judge_set(p, p->item.sensor2_ch1_set, WHICH_WARN_VALUE);
		#endif
	}

	// sensor2_ch2 小于最小值，大于最大值（温度）
	if((p->item.sensor2_ch2 < p->item.sensor2_ch2_min) || (p->item.sensor2_ch2 > p->item.sensor2_ch2_max))
	{
		#if ( WARN_EN > 0 )		
			warn_judge_set(p, p->item.sensor2_ch2_set, WHICH_WARN_VALUE);
		#endif
	}
}

#endif
