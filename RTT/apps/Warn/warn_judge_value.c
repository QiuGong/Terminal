#include "warn.h"


#if ( WARN_VALUE > 0 )

void warn_judge_value(Position p)
{
	// sensor1_ch1 С����Сֵ���������ֵ��������
	if((p->item.sensor1_ch1 < p->item.sensor1_ch1_min) || (p->item.sensor1_ch1 > p->item.sensor1_ch1_max))
	{
		#if ( WARN_EN > 0 )	
			warn_judge_set(p, p->item.sensor1_ch1_set, WHICH_WARN_VALUE);
		#endif	
	}

	// sensor1_ch2 С����Сֵ���������ֵ�������ȣ�
	if((p->item.sensor1_ch2 < p->item.sensor1_ch2_min) || (p->item.sensor1_ch2 > p->item.sensor1_ch2_max))
	{
		#if ( WARN_EN > 0 )
			warn_judge_set(p, p->item.sensor1_ch2_set, WHICH_WARN_VALUE);
		#endif
	}

	// sensor2_ch1 С����Сֵ���������ֵ��PH��
	if((p->item.sensor2_ch1 < p->item.sensor2_ch1_min) || (p->item.sensor2_ch1 > p->item.sensor2_ch1_max))
	{
		#if ( WARN_EN > 0 )
			warn_judge_set(p, p->item.sensor2_ch1_set, WHICH_WARN_VALUE);
		#endif
	}

	// sensor2_ch2 С����Сֵ���������ֵ���¶ȣ�
	if((p->item.sensor2_ch2 < p->item.sensor2_ch2_min) || (p->item.sensor2_ch2 > p->item.sensor2_ch2_max))
	{
		#if ( WARN_EN > 0 )		
			warn_judge_set(p, p->item.sensor2_ch2_set, WHICH_WARN_VALUE);
		#endif
	}
}

#endif
