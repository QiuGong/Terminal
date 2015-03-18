#include "warn.h"


#if ( WARN_EN > 0 )
	#define MIDDLE_VALUE		1
#else
	#define MIDDLE_VALUE		0
#endif


// 最大值标志
#define is_true_16(x) 		(x &  (0x0001 << (16 - 1)))
#define true_16(x) 			(x |= (0x0001 << (16 - 1)))
#define false_16(x)			(x &= 0x7FFF)

// 最小值标志
#define is_true_15(x) 		(x & (0x0001 << (15 - 1)))
#define true_15(x) 			(x |= (0x0001 << (15 - 1)))
#define false_15(x)			(x &= 0xBFFF)


#define NORMAL				0x00
#define S1_C1              	0x01
#define S1_C2               0x02
#define S2_C1               0x03
#define S2_C2               0x04


#if ( WARN_VALUE > 0 )

static rt_uint8_t 	value_warn_flag;
rt_uint8_t _get_value_warn_flag(void)
{
	return 	value_warn_flag;
}


void warn_judge_value(Position p)
{
	// sensor1_ch1 小于最小值，大于最大值（电流）
	if(((p->item.sensor1_ch1 < p->item.sensor1_ch1_min) && (p->item.sensor1_ch1_min != 0)) 
	|| ((p->item.sensor1_ch1 > p->item.sensor1_ch1_max) && (p->item.sensor1_ch1_max != 0)))
	{
		#if ( WARN_EN > 0 )
			value_warn_flag = S1_C1;
			warn_judge_set(p, p->item.sensor1_ch1_set, WHICH_WARN_VALUE);
		#endif

		#if ( MIDDLE_VALUE > 0 )
			// 设置
			if(p->item.sensor1_ch1_mid > 0)
			{
				// 未标置
				if((is_true_16(p->item.sensor1_ch1_mid) == RT_FALSE) && (is_true_15(p->item.sensor1_ch1_mid) == RT_FALSE))
				{
					// 设置最小值
					if(p->item.sensor1_ch1 < p->item.sensor1_ch1_min)
					{
						p->item.sensor1_ch1_min += p->item.sensor1_ch1_mid;
						true_15(p->item.sensor1_ch1_mid);
						RT_DEBUG_LOG(DEBUG_RS485, ("s1_ch1 trigger min += mid.\n"));
					}
					// 设置最大值
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
		#if ( WARN_EN > 0 )
			if(value_warn_flag == S1_C1)
			{
				value_warn_flag = NORMAL;
			}
		#endif
		
		#if ( MIDDLE_VALUE > 0 )
			// 设置
			if(p->item.sensor1_ch1_mid > 0)
			{
				// 设置最小值
				if(is_true_15(p->item.sensor1_ch1_mid))
				{
					false_15(p->item.sensor1_ch1_mid);
					p->item.sensor1_ch1_min -= p->item.sensor1_ch1_mid;
					RT_DEBUG_LOG(DEBUG_RS485, ("s1_ch1 restore min -= mid.\n"));
				}
				// 设置最大值
				else if(is_true_16(p->item.sensor1_ch1_mid))
				{
					false_16(p->item.sensor1_ch1_mid);
					p->item.sensor1_ch1_max += p->item.sensor1_ch1_mid;						
					RT_DEBUG_LOG(DEBUG_RS485, ("s1_ch1 restore min += mid.\n"));
				}
			}
		#endif	
	}

	// sensor1_ch2 小于最小值，大于最大值（溶氧度）
	if(((p->item.sensor1_ch2 < p->item.sensor1_ch2_min) && (p->item.sensor1_ch2_min != 0)) 
	|| ((p->item.sensor1_ch2 > p->item.sensor1_ch2_max) && (p->item.sensor1_ch2_max != 0)))
	{
		#if ( WARN_EN > 0 )
			value_warn_flag = S1_C2;
			warn_judge_set(p, p->item.sensor1_ch2_set, WHICH_WARN_VALUE);
		#endif

		#if ( MIDDLE_VALUE > 0 )
			// 设置
			if(p->item.sensor1_ch2_mid > 0)
			{
				// 未标置
				if((is_true_16(p->item.sensor1_ch2_mid) == RT_FALSE) && (is_true_15(p->item.sensor1_ch2_mid) == RT_FALSE))
				{
					// 设置最小值
					if(p->item.sensor1_ch2 < p->item.sensor1_ch2_min)
					{
						p->item.sensor1_ch2_min += p->item.sensor1_ch2_mid;
						true_15(p->item.sensor1_ch2_mid);
						RT_DEBUG_LOG(DEBUG_RS485, ("s1_ch2 trigger min += mid.\n"));
					}
					// 设置最大值
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
		#if ( WARN_EN > 0 )
			if(value_warn_flag == S1_C2)
			{
				value_warn_flag = NORMAL;
			}
		#endif
		
		#if ( MIDDLE_VALUE > 0 )
			// 设置
			if(p->item.sensor1_ch2_mid > 0)
			{
				// 设置最小值
				if(is_true_15(p->item.sensor1_ch2_mid))
				{
					false_15(p->item.sensor1_ch2_mid);
					p->item.sensor1_ch2_min -= p->item.sensor1_ch2_mid;
					RT_DEBUG_LOG(DEBUG_RS485, ("s1_ch2 restore min -= mid.\n"));
				}
				// 设置最大值
				else if(is_true_16(p->item.sensor1_ch2_mid))
				{
					false_16(p->item.sensor1_ch2_mid);
					p->item.sensor1_ch2_max += p->item.sensor1_ch2_mid;						
					RT_DEBUG_LOG(DEBUG_RS485, ("s1_ch2 restore min += mid.\n"));
				}
			}
		#endif	
	}

	// sensor2_ch1 小于最小值，大于最大值（PH）
	if(((p->item.sensor2_ch1 < p->item.sensor2_ch1_min) && (p->item.sensor2_ch1_min != 0)) 
	|| ((p->item.sensor2_ch1 > p->item.sensor2_ch1_max) && (p->item.sensor2_ch1_max != 0)))
	{
		#if ( WARN_EN > 0 )
			value_warn_flag = S2_C1;
			warn_judge_set(p, p->item.sensor2_ch1_set, WHICH_WARN_VALUE);
		#endif

		#if ( MIDDLE_VALUE > 0 )
			// 设置
			if(p->item.sensor2_ch1_mid > 0)
			{
				// 未标置
				if((is_true_16(p->item.sensor2_ch1_mid) == RT_FALSE) && (is_true_15(p->item.sensor2_ch1_mid) == RT_FALSE))
				{
					// 设置最小值
					if(p->item.sensor2_ch1 < p->item.sensor2_ch1_min)
					{
						p->item.sensor2_ch1_min += p->item.sensor2_ch1_mid;
						true_15(p->item.sensor2_ch1_mid);
						RT_DEBUG_LOG(DEBUG_RS485, ("s2_ch1 trigger min += mid.\n"));
					}
					// 设置最大值
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
		#if ( WARN_EN > 0 )
			if(value_warn_flag == S2_C1)
			{
				value_warn_flag = NORMAL;
			}
		#endif
		
		#if ( MIDDLE_VALUE > 0 )
			// 设置
			if(p->item.sensor2_ch1_mid > 0)
			{
				// 设置最小值
				if(is_true_15(p->item.sensor2_ch1_mid))
				{
					false_15(p->item.sensor2_ch1_mid);
					p->item.sensor2_ch1_min -= p->item.sensor2_ch1_mid;
					RT_DEBUG_LOG(DEBUG_RS485, ("s2_ch1 restore min -= mid.\n"));
				}
				// 设置最大值
				else if(is_true_16(p->item.sensor2_ch1_mid))
				{
					false_16(p->item.sensor2_ch1_mid);
					p->item.sensor2_ch1_max += p->item.sensor2_ch1_mid;						
					RT_DEBUG_LOG(DEBUG_RS485, ("s2_ch1 restore min += mid.\n"));
				}
			}
		#endif	
	}

	// sensor2_ch2 小于最小值，大于最大值（温度）
	if(((p->item.sensor2_ch2 < p->item.sensor2_ch2_min) && (p->item.sensor2_ch2_min != 0)) 
	|| ((p->item.sensor2_ch2 > p->item.sensor2_ch2_max) && (p->item.sensor2_ch2_max != 0)))
	{
		#if ( WARN_EN > 0 )
			value_warn_flag = S2_C2;
			warn_judge_set(p, p->item.sensor2_ch2_set, WHICH_WARN_VALUE);
		#endif

		#if ( MIDDLE_VALUE > 0 )
			// 设置
			if(p->item.sensor2_ch2_mid > 0)
			{
				// 未标置
				if((is_true_16(p->item.sensor2_ch2_mid) == RT_FALSE) && (is_true_15(p->item.sensor2_ch2_mid) == RT_FALSE))
				{
					// 设置最小值
					if(p->item.sensor2_ch2 < p->item.sensor2_ch2_min)
					{
						p->item.sensor2_ch2_min += p->item.sensor2_ch2_mid;
						true_15(p->item.sensor2_ch2_mid);
						RT_DEBUG_LOG(DEBUG_RS485, ("s2_ch2 trigger min += mid.\n"));
					}
					// 设置最大值
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
		#if ( WARN_EN > 0 )
			if(value_warn_flag == S2_C2)
			{
				value_warn_flag = NORMAL;
			}
		#endif
		
		#if ( MIDDLE_VALUE > 0 )
			// 设置
			if(p->item.sensor2_ch2_mid > 0)
			{
				// 设置最小值
				if(is_true_15(p->item.sensor2_ch2_mid))
				{
					false_15(p->item.sensor2_ch2_mid);
					p->item.sensor2_ch2_min -= p->item.sensor2_ch2_mid;
					RT_DEBUG_LOG(DEBUG_RS485, ("s2_ch2 restore min -= mid.\n"));
				}
				// 设置最大值
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
