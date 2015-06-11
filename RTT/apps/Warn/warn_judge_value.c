#include "warn.h"

// （1）最大值，最小值 报警
// （2）中间值 设置
// （3）报警延迟 


// 中间值
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
#define is_true_15(x) 		(x &  (0x0001 << (15 - 1)))
#define true_15(x) 			(x |= (0x0001 << (15 - 1)))
#define false_15(x)			(x &= 0xBFFF)

#define NORMAL				0x00
#define S1_C1              	0x01
#define S1_C2               0x02
#define S2_C1               0x03
#define S2_C2               0x04

// 报警延长时间
#define	WARN_DELAY_TIME		0x03


// 延迟报警
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
	// sensor1_ch1 小于最小值，大于最大值（电流）
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
		#if ( WARN_CNT > 0 )
			if(p->item.sensor1_ch2_cnt >=  MAX_CNT)
			{		
				#if ( WARN_EN > 0 )
					value_warn_flag = S1_C2;
					value_warn_flag_last_rec_time = p->item.last_rec_time;
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
		#if ( WARN_CNT > 0 )
			if(p->item.sensor2_ch1_cnt >=  MAX_CNT)
			{		
				#if ( WARN_EN > 0 )
					value_warn_flag = S2_C1;
					value_warn_flag_last_rec_time = p->item.last_rec_time;
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
		 #if ( WARN_CNT > 0 )
			if(p->item.sensor2_ch2_cnt >=  MAX_CNT)
			{		
				#if ( WARN_EN > 0 )
					value_warn_flag = S2_C2;
					value_warn_flag_last_rec_time = p->item.last_rec_time;
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
