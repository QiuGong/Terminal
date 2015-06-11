#include "warn.h"

// ��1�����ֵ����Сֵ ����
// ��2���м�ֵ ����
// ��3�������ӳ� 


// �м�ֵ
#if ( WARN_EN > 0 )
	#define MIDDLE_VALUE		1
#else
	#define MIDDLE_VALUE		0
#endif

// ���ֵ��־
#define is_true_16(x) 		(x &  (0x0001 << (16 - 1)))
#define true_16(x) 			(x |= (0x0001 << (16 - 1)))
#define false_16(x)			(x &= 0x7FFF)

// ��Сֵ��־
#define is_true_15(x) 		(x &  (0x0001 << (15 - 1)))
#define true_15(x) 			(x |= (0x0001 << (15 - 1)))
#define false_15(x)			(x &= 0xBFFF)

#define NORMAL				0x00
#define S1_C1              	0x01
#define S1_C2               0x02
#define S2_C1               0x03
#define S2_C2               0x04

// �����ӳ�ʱ��
#define	WARN_DELAY_TIME		0x03


// �ӳٱ���
#if ( WARN_EN > 0 )
	#define WARN_CNT			1
#else
	#define WARN_CNT			0
#endif

#define MAX_CNT               	3


#if ( WARN_VALUE > 0 )

static rt_uint8_t 	value_warn_flag;
static rt_uint32_t 	value_warn_flag_last_rec_time;
rt_uint8_t _get_value_warn_flag(void)
{
	return 	value_warn_flag;
}


void _set_value_warn_flag(rt_uint8_t value)
{
	value_warn_flag = value;
}


void warn_judge_value(Position p)
{
	// sensor1_ch1 С����Сֵ���������ֵ��������
	if(((p->item.sensor1_ch1 < p->item.sensor1_ch1_min) && (p->item.sensor1_ch1_min != 0)) 
	|| ((p->item.sensor1_ch1 > p->item.sensor1_ch1_max) && (p->item.sensor1_ch1_max != 0)))
	{
		#if ( WARN_CNT > 0 )
			if(p->item.sensor1_ch1_cnt >=  MAX_CNT)
			{
				#if ( WARN_EN > 0 )
					value_warn_flag = S1_C1;
					value_warn_flag_last_rec_time = p->item.last_rec_time;
					warn_judge_set(p, p->item.sensor1_ch1_set, WHICH_WARN_VALUE);
				#endif
		
				#if ( MIDDLE_VALUE > 0 )
					// ����
					if(p->item.sensor1_ch1_mid > 0)
					{
						// δ����
						if((is_true_16(p->item.sensor1_ch1_mid) == RT_FALSE) && (is_true_15(p->item.sensor1_ch1_mid) == RT_FALSE))
						{
							// ������Сֵ
							if(p->item.sensor1_ch1 < p->item.sensor1_ch1_min)
							{
								p->item.sensor1_ch1_min += p->item.sensor1_ch1_mid;
								true_15(p->item.sensor1_ch1_mid);
								RT_DEBUG_LOG(DEBUG_RS485, ("s1_ch1 trigger min += mid.\n"));
							}
							// �������ֵ
							else if(p->item.sensor1_ch1 > p->item.sensor1_ch1_max)
							{
								p->item.sensor1_ch1_max -= p->item.sensor1_ch1_mid;
								true_16(p->item.sensor1_ch1_mid);
								RT_DEBUG_LOG(DEBUG_RS485, ("s1_ch1 trigger min -= mid.\n"));
							}
						}
					}
				#endif
			}
			else
			{
				if(p->item.sensor1_ch1_cnt++ > MAX_CNT)
				{
					p->item.sensor1_ch1_cnt = MAX_CNT;
				}
			}
		#endif	
	}
	else
	{
		#if ( WARN_CNT > 0 )
			p->item.sensor1_ch1_cnt = 0;
		#endif

		#if ( WARN_EN > 0 )
			if(value_warn_flag == S1_C1)
			{				
				if(p->item.last_rec_time - value_warn_flag_last_rec_time > WARN_DELAY_TIME)
				{
					value_warn_flag = NORMAL;
					value_warn_flag_last_rec_time = p->item.last_rec_time;
				}
			}
		#endif
		
		#if ( MIDDLE_VALUE > 0 )
			// ����
			if(p->item.sensor1_ch1_mid > 0)
			{
				// ������Сֵ
				if(is_true_15(p->item.sensor1_ch1_mid))
				{
					false_15(p->item.sensor1_ch1_mid);
					p->item.sensor1_ch1_min -= p->item.sensor1_ch1_mid;
					RT_DEBUG_LOG(DEBUG_RS485, ("s1_ch1 restore min -= mid.\n"));
				}
				// �������ֵ
				else if(is_true_16(p->item.sensor1_ch1_mid))
				{
					false_16(p->item.sensor1_ch1_mid);
					p->item.sensor1_ch1_max += p->item.sensor1_ch1_mid;						
					RT_DEBUG_LOG(DEBUG_RS485, ("s1_ch1 restore min += mid.\n"));
				}
			}
		#endif				
	}

	// sensor1_ch2 С����Сֵ���������ֵ�������ȣ�
	if(((p->item.sensor1_ch2 < p->item.sensor1_ch2_min) && (p->item.sensor1_ch2_min != 0)) 
	|| ((p->item.sensor1_ch2 > p->item.sensor1_ch2_max) && (p->item.sensor1_ch2_max != 0)))
	{
		#if ( WARN_CNT > 0 )
			if(p->item.sensor1_ch2_cnt >=  MAX_CNT)
			{		
				#if ( WARN_EN > 0 )
					value_warn_flag = S1_C2;
					value_warn_flag_last_rec_time = p->item.last_rec_time;
					warn_judge_set(p, p->item.sensor1_ch2_set, WHICH_WARN_VALUE);
				#endif
		
				#if ( MIDDLE_VALUE > 0 )
					// ����
					if(p->item.sensor1_ch2_mid > 0)
					{
						// δ����
						if((is_true_16(p->item.sensor1_ch2_mid) == RT_FALSE) && (is_true_15(p->item.sensor1_ch2_mid) == RT_FALSE))
						{
							// ������Сֵ
							if(p->item.sensor1_ch2 < p->item.sensor1_ch2_min)
							{
								p->item.sensor1_ch2_min += p->item.sensor1_ch2_mid;
								true_15(p->item.sensor1_ch2_mid);
								RT_DEBUG_LOG(DEBUG_RS485, ("s1_ch2 trigger min += mid.\n"));
							}
							// �������ֵ
							else if(p->item.sensor1_ch2 > p->item.sensor1_ch2_max)
							{
								p->item.sensor1_ch2_max -= p->item.sensor1_ch2_mid;
								true_16(p->item.sensor1_ch2_mid);
								RT_DEBUG_LOG(DEBUG_RS485, ("s1_ch2 trigger min -= mid.\n"));
							}
						}
					}
				#endif
			}
			else
			{
				if(p->item.sensor1_ch2_cnt++ > MAX_CNT)
				{
					p->item.sensor1_ch2_cnt = MAX_CNT;
				}
			}
		#endif			
	}
	else
	{
		#if ( WARN_CNT > 0 )
			p->item.sensor1_ch2_cnt = 0;
		#endif

		#if ( WARN_EN > 0 )
			if(value_warn_flag == S1_C2)
			{
				if(p->item.last_rec_time - value_warn_flag_last_rec_time > WARN_DELAY_TIME)
				{
					value_warn_flag = NORMAL;
					value_warn_flag_last_rec_time = p->item.last_rec_time;
				}
			}
		#endif
		
		#if ( MIDDLE_VALUE > 0 )
			// ����
			if(p->item.sensor1_ch2_mid > 0)
			{
				// ������Сֵ
				if(is_true_15(p->item.sensor1_ch2_mid))
				{
					false_15(p->item.sensor1_ch2_mid);
					p->item.sensor1_ch2_min -= p->item.sensor1_ch2_mid;
					RT_DEBUG_LOG(DEBUG_RS485, ("s1_ch2 restore min -= mid.\n"));
				}
				// �������ֵ
				else if(is_true_16(p->item.sensor1_ch2_mid))
				{
					false_16(p->item.sensor1_ch2_mid);
					p->item.sensor1_ch2_max += p->item.sensor1_ch2_mid;						
					RT_DEBUG_LOG(DEBUG_RS485, ("s1_ch2 restore min += mid.\n"));
				}
			}
		#endif						
	}

	// sensor2_ch1 С����Сֵ���������ֵ��PH��
	if(((p->item.sensor2_ch1 < p->item.sensor2_ch1_min) && (p->item.sensor2_ch1_min != 0)) 
	|| ((p->item.sensor2_ch1 > p->item.sensor2_ch1_max) && (p->item.sensor2_ch1_max != 0)))
	{
		#if ( WARN_CNT > 0 )
			if(p->item.sensor2_ch1_cnt >=  MAX_CNT)
			{		
				#if ( WARN_EN > 0 )
					value_warn_flag = S2_C1;
					value_warn_flag_last_rec_time = p->item.last_rec_time;
					warn_judge_set(p, p->item.sensor2_ch1_set, WHICH_WARN_VALUE);
				#endif
		
				#if ( MIDDLE_VALUE > 0 )
					// ����
					if(p->item.sensor2_ch1_mid > 0)
					{
						// δ����
						if((is_true_16(p->item.sensor2_ch1_mid) == RT_FALSE) && (is_true_15(p->item.sensor2_ch1_mid) == RT_FALSE))
						{
							// ������Сֵ
							if(p->item.sensor2_ch1 < p->item.sensor2_ch1_min)
							{
								p->item.sensor2_ch1_min += p->item.sensor2_ch1_mid;
								true_15(p->item.sensor2_ch1_mid);
								RT_DEBUG_LOG(DEBUG_RS485, ("s2_ch1 trigger min += mid.\n"));
							}
							// �������ֵ
							else if(p->item.sensor2_ch1 > p->item.sensor2_ch1_max)
							{
								p->item.sensor2_ch1_max -= p->item.sensor2_ch1_mid;
								true_16(p->item.sensor2_ch1_mid);
								RT_DEBUG_LOG(DEBUG_RS485, ("s2_ch1 trigger min -= mid.\n"));
							}
						}
					}
				#endif
			}		
		 	else
			{
				if(p->item.sensor2_ch1_cnt++ > MAX_CNT)
				{
					p->item.sensor2_ch1_cnt = MAX_CNT;
				}
			}
		#endif
	}
	else
	{
		#if ( WARN_CNT > 0 )
			p->item.sensor2_ch1_cnt = 0;
		#endif
						
		#if ( WARN_EN > 0 )
			if(value_warn_flag == S2_C1)
			{
				if(p->item.last_rec_time - value_warn_flag_last_rec_time > WARN_DELAY_TIME)
				{
					value_warn_flag = NORMAL;
					value_warn_flag_last_rec_time = p->item.last_rec_time;
				}
			}
		#endif
		
		#if ( MIDDLE_VALUE > 0 )
			// ����
			if(p->item.sensor2_ch1_mid > 0)
			{
				// ������Сֵ
				if(is_true_15(p->item.sensor2_ch1_mid))
				{
					false_15(p->item.sensor2_ch1_mid);
					p->item.sensor2_ch1_min -= p->item.sensor2_ch1_mid;
					RT_DEBUG_LOG(DEBUG_RS485, ("s2_ch1 restore min -= mid.\n"));
				}
				// �������ֵ
				else if(is_true_16(p->item.sensor2_ch1_mid))
				{
					false_16(p->item.sensor2_ch1_mid);
					p->item.sensor2_ch1_max += p->item.sensor2_ch1_mid;						
					RT_DEBUG_LOG(DEBUG_RS485, ("s2_ch1 restore min += mid.\n"));
				}
			}
		#endif	
	}

	// sensor2_ch2 С����Сֵ���������ֵ���¶ȣ�
	if(((p->item.sensor2_ch2 < p->item.sensor2_ch2_min) && (p->item.sensor2_ch2_min != 0)) 
	|| ((p->item.sensor2_ch2 > p->item.sensor2_ch2_max) && (p->item.sensor2_ch2_max != 0)))
	{
		 #if ( WARN_CNT > 0 )
			if(p->item.sensor2_ch2_cnt >=  MAX_CNT)
			{		
				#if ( WARN_EN > 0 )
					value_warn_flag = S2_C2;
					value_warn_flag_last_rec_time = p->item.last_rec_time;
					warn_judge_set(p, p->item.sensor2_ch2_set, WHICH_WARN_VALUE);
				#endif
		
				#if ( MIDDLE_VALUE > 0 )
					// ����
					if(p->item.sensor2_ch2_mid > 0)
					{
						// δ����
						if((is_true_16(p->item.sensor2_ch2_mid) == RT_FALSE) && (is_true_15(p->item.sensor2_ch2_mid) == RT_FALSE))
						{
							// ������Сֵ
							if(p->item.sensor2_ch2 < p->item.sensor2_ch2_min)
							{
								p->item.sensor2_ch2_min += p->item.sensor2_ch2_mid;
								true_15(p->item.sensor2_ch2_mid);
								RT_DEBUG_LOG(DEBUG_RS485, ("s2_ch2 trigger min += mid.\n"));
							}
							// �������ֵ
							else if(p->item.sensor2_ch2 > p->item.sensor2_ch2_max)
							{
								p->item.sensor2_ch2_max -= p->item.sensor2_ch2_mid;
								true_16(p->item.sensor2_ch2_mid);
								RT_DEBUG_LOG(DEBUG_RS485, ("s2_ch2 trigger min -= mid.\n"));
							}
						}
					}
				#endif
			}
			else
			{
				if(p->item.sensor2_ch2_cnt++ > MAX_CNT)
				{
					p->item.sensor2_ch2_cnt = MAX_CNT;
				}
			}
		#endif			
	}
	else
	{
		#if ( WARN_CNT > 0 )
			p->item.sensor2_ch2_cnt = 0;
		#endif
						
		#if ( WARN_EN > 0 )
			if(value_warn_flag == S2_C2)
			{
				if(p->item.last_rec_time - value_warn_flag_last_rec_time > WARN_DELAY_TIME)
				{
					value_warn_flag = NORMAL;
					value_warn_flag_last_rec_time = p->item.last_rec_time;
				}
			}
		#endif
		
		#if ( MIDDLE_VALUE > 0 )
			// ����
			if(p->item.sensor2_ch2_mid > 0)
			{
				// ������Сֵ
				if(is_true_15(p->item.sensor2_ch2_mid))
				{
					false_15(p->item.sensor2_ch2_mid);
					p->item.sensor2_ch2_min -= p->item.sensor2_ch2_mid;
					RT_DEBUG_LOG(DEBUG_RS485, ("s2_ch2 restore min -= mid.\n"));
				}
				// �������ֵ
				else if(is_true_16(p->item.sensor2_ch2_mid))
				{
					false_16(p->item.sensor2_ch2_mid);
					p->item.sensor2_ch2_max += p->item.sensor2_ch2_mid;						
					RT_DEBUG_LOG(DEBUG_RS485, ("s2_ch2 restore min += mid.\n"));
				}
			}
		#endif						
	}
}

#endif
