#include "warn.h"
#include "select_fun.h"


#if ( WARN_EN > 0 )

// 标记采集器：阈值，定时。
static void judge_sensor_set(Position p, rt_uint32_t value, enum X01_DEV dev, rt_uint8_t which)
{
	// 当前设备位是否标志
	if(value & (0x00000001 << (dev - 1)))
	{
		// 当前开关位是否标志
		if(value & (0x00000001 << (dev - 1 + 16)))
		{
			if(which == WHICH_WARN_VALUE)
			{
				RT_DEBUG_LOG(DEBUG_WARN, ("value warn on.\n"));
				p->item.value_set |=  (value & (0x00000001 << (dev - 1)));
			}
			else if(which == WHICH_WARN_TIMER)
			{
				RT_DEBUG_LOG(DEBUG_WARN, ("time warn on.\n"));
				p->item.timer_set |=  (value & (0x00000001 << (dev - 1)));
			}
		}
		else if((value & (0x00000001 << (dev - 1 + 16))) == OFF)
		{
			if(which == WHICH_WARN_VALUE)
			{
				RT_DEBUG_LOG(DEBUG_WARN, ("value warn off.\n"));
				p->item.value_set &= ~(value & (0x00000001 << (dev - 1)));
			}
			else if(which == WHICH_WARN_TIMER)
			{
				RT_DEBUG_LOG(DEBUG_WARN, ("time warn off.\n"));
				p->item.timer_set &= ~(value & (0x00000001 << (dev - 1 )));
			}			
		}	
	}		
}


static rt_uint16_t	control_value_set;
static rt_uint16_t	control_timer_set;
static rt_uint16_t	control_relate_set;
static rt_uint16_t	control_time_out_set;
static rt_uint16_t	control_power_drop_set;
static void judge_control_set(rt_uint32_t value, enum WARN_CONTROL dev, rt_uint8_t which)
{
	// 当前设备位是否标志
	if(value & (0x00000001 << (dev - 1 + 8)))
	{		
		// 当前开关位是否标志
		if(value & (0x00000001 << (dev - 1 + 8 + 16)))
		{
			if(which == WHICH_WARN_VALUE)
			{
				control_value_set |=  (value & (0x00000001 << (dev - 1 + 8)));
			}
			else if(which == WHICH_WARN_TIMER)
			{
				control_timer_set |=  (value & (0x00000001 << (dev - 1 + 8)));
			}
			else if(which == WHICH_WARN_RELATE)
			{
				control_relate_set |=  (value & (0x00000001 << (dev - 1 + 8)));
			}
			else if(which == WHICH_WARN_TIME_OUT)
			{
				control_time_out_set |=  (value & (0x00000001 << (dev - 1 + 8)));
			}
			else if(which == WHICH_WARN_POWER_DROP)
			{
				control_power_drop_set |=  (value & (0x00000001 << (dev - 1 + 8)));
			}
		}
		else if((value & (0x00000001 << (dev - 1 + 8 + 16))) == OFF)
		{
			if(which == WHICH_WARN_VALUE)
			{
				control_value_set &= ~(value & (0x00000001 << (dev - 1 + 8)));
			}
			else if(which == WHICH_WARN_TIMER)
			{
				control_timer_set &= ~(value & (0x00000001 << (dev - 1 + 8)));
			}
			else if(which == WHICH_WARN_RELATE)
			{
				control_relate_set &= ~(value & (0x00000001 << (dev - 1 + 8)));
			}
			else if(which == WHICH_WARN_TIME_OUT)
			{
				control_time_out_set &= ~(value & (0x00000001 << (dev - 1 + 8)));
			}
			else if(which == WHICH_WARN_POWER_DROP)
			{
				control_power_drop_set &= ~(value & (0x00000001 << (dev - 1 + 8)));
			}			
		}	
	}		
}


void warn_judge_set(Position p, rt_uint32_t value, rt_uint8_t which)
{
	judge_sensor_set(p, value, LED, which);
	judge_sensor_set(p, value, CH1, which);
	judge_sensor_set(p, value, T2,  which);
	judge_sensor_set(p, value, T3,  which);
	judge_sensor_set(p, value, M_A, which);
	judge_sensor_set(p, value, M_B, which);
	judge_sensor_set(p, value, M_C, which);

	judge_control_set(value, WARN_CONTROL_LED, which);
	judge_control_set(value, WARN_CONTROL_BELL, which);	
}


static void sensor_set(Position p)
{	
	// LED
	if((p->item.sensor1_ch1_set & (0x00000001 << (LED - 1))) ||
	   (p->item.sensor1_ch2_set & (0x00000001 << (LED - 1))) ||
	   (p->item.sensor2_ch1_set & (0x00000001 << (LED - 1))) ||
	   (p->item.sensor2_ch2_set & (0x00000001 << (LED - 1))) ||
	   (p->item.time1_set 		& (0x00000001 << (LED - 1))) ||
	   (p->item.time2_set 		& (0x00000001 << (LED - 1))) ||
	   (p->item.time3_set 		& (0x00000001 << (LED - 1))) ||
	   (p->item.time4_set 		& (0x00000001 << (LED - 1))))
	{		
		if((p->item.value_set & (0x00000001 << (LED - 1))) ||
		   (p->item.timer_set & (0x00000001 << (LED - 1))))
		{
			if((p->item.k & (0x0001 << (LED - 1))) == OFF)
			{
				x01_request(p->item.id, LED, ON);
				RT_DEBUG_LOG(DEBUG_WARN, ("LED ON.\n"));	
			}
		}
		else
		{
			if(p->item.k & (0x0001 << (LED - 1)))
			{
				x01_request(p->item.id, LED, OFF);
				RT_DEBUG_LOG(DEBUG_WARN, ("LED OFF.\n"));
			}
		}
	}

	
	// CH1
	if((p->item.sensor1_ch1_set & (0x00000001 << (CH1 - 1))) ||
	   (p->item.sensor1_ch2_set & (0x00000001 << (CH1 - 1))) ||
	   (p->item.sensor2_ch1_set & (0x00000001 << (CH1 - 1))) ||
	   (p->item.sensor2_ch2_set & (0x00000001 << (CH1 - 1))) ||
	   (p->item.time1_set 		& (0x00000001 << (CH1 - 1))) ||
	   (p->item.time2_set 		& (0x00000001 << (CH1 - 1))) ||
	   (p->item.time3_set 		& (0x00000001 << (CH1 - 1))) ||
	   (p->item.time4_set 		& (0x00000001 << (CH1 - 1))))
	{
		if((p->item.value_set & (0x00000001 << (CH1 - 1))) ||
		   (p->item.timer_set & (0x00000001 << (CH1 - 1))))
		{
			if((p->item.k & (0x0001 << (CH1 - 1))) == OFF)
			{
				x01_request(p->item.id, CH1, ON);
				RT_DEBUG_LOG(DEBUG_WARN, ("CH1 ON.\n"));	
			}
		}
		else
		{
			if(p->item.k & (0x0001 << (CH1 - 1)))
			{
				x01_request(p->item.id, CH1, OFF);
				RT_DEBUG_LOG(DEBUG_WARN, ("CH1 OFF.\n"));
			}
		}
	}


	// T2
	if((p->item.sensor1_ch1_set & (0x00000001 << (T2 - 1))) ||
	   (p->item.sensor1_ch2_set & (0x00000001 << (T2 - 1))) ||
	   (p->item.sensor2_ch1_set & (0x00000001 << (T2 - 1))) ||
	   (p->item.sensor2_ch2_set & (0x00000001 << (T2 - 1))) ||
	   (p->item.time1_set 		& (0x00000001 << (T2 - 1))) ||
	   (p->item.time2_set 		& (0x00000001 << (T2 - 1))) ||
	   (p->item.time3_set 		& (0x00000001 << (T2 - 1))) ||
	   (p->item.time4_set 		& (0x00000001 << (T2 - 1))))
	{
		if((p->item.value_set & (0x00000001 << (T2 - 1))) ||
		   (p->item.timer_set & (0x00000001 << (T2 - 1))))
		{
			if((p->item.k & (0x0001 << (T2 - 1))) == OFF)
			{
				x01_request(p->item.id, T2, ON);
				RT_DEBUG_LOG(DEBUG_WARN, ("T2 ON.\n"));	
			}
		}
		else
		{
			if(p->item.k & (0x0001 << (T2 - 1)))
			{
				x01_request(p->item.id, T2, OFF);
				RT_DEBUG_LOG(DEBUG_WARN, ("T2 OFF.\n"));
			}
		}
	}
	

	// T3
	if((p->item.sensor1_ch1_set & (0x00000001 << (T3 - 1))) ||
	   (p->item.sensor1_ch2_set & (0x00000001 << (T3 - 1))) ||
	   (p->item.sensor2_ch1_set & (0x00000001 << (T3 - 1))) ||
	   (p->item.sensor2_ch2_set & (0x00000001 << (T3 - 1))) ||
	   (p->item.time1_set 		& (0x00000001 << (T3 - 1))) ||
	   (p->item.time2_set 		& (0x00000001 << (T3 - 1))) ||
	   (p->item.time3_set 		& (0x00000001 << (T3 - 1))) ||
	   (p->item.time4_set 		& (0x00000001 << (T3 - 1))))
	{
		if((p->item.value_set & (0x00000001 << (T3 - 1))) ||
		   (p->item.timer_set & (0x00000001 << (T3 - 1))))
		{
			if((p->item.k & (0x0001 << (T3 - 1))) == OFF)
			{
				x01_request(p->item.id, T3, ON);
				RT_DEBUG_LOG(DEBUG_WARN, ("T3 ON.\n"));	
			}
		}
		else
		{
			if(p->item.k & (0x0001 << (T3 - 1)))
			{
				x01_request(p->item.id, T3, OFF);
				RT_DEBUG_LOG(DEBUG_WARN, ("T3 OFF.\n"));
			}
		}
	}
	

	// M_A
	if((p->item.sensor1_ch1_set & (0x00000001 << (M_A - 1))) ||
	   (p->item.sensor1_ch2_set & (0x00000001 << (M_A - 1))) ||
	   (p->item.sensor2_ch1_set & (0x00000001 << (M_A - 1))) ||
	   (p->item.sensor2_ch2_set & (0x00000001 << (M_A - 1))) ||
	   (p->item.time1_set 		& (0x00000001 << (M_A - 1))) ||
	   (p->item.time2_set 		& (0x00000001 << (M_A - 1))) ||
	   (p->item.time3_set 		& (0x00000001 << (M_A - 1))) ||
	   (p->item.time4_set 		& (0x00000001 << (M_A - 1))))
	{
		if((p->item.value_set & (0x00000001 << (M_A - 1))) ||
		   (p->item.timer_set & (0x00000001 << (M_A - 1))))
		{
			if((p->item.k & (0x0001 << (M_A - 1))) == OFF)
			{
				x01_request(p->item.id, M_A, ON);
				RT_DEBUG_LOG(DEBUG_WARN, ("M_A ON.\n"));	
			}
		}
		else
		{
			if(p->item.k & (0x0001 << (M_A - 1)))
			{
				x01_request(p->item.id, M_A, OFF);
				RT_DEBUG_LOG(DEBUG_WARN, ("M_A OFF.\n"));
			}
		}
	}
	

	// M_B
	if((p->item.sensor1_ch1_set & (0x00000001 << (M_B - 1))) ||
	   (p->item.sensor1_ch2_set & (0x00000001 << (M_B - 1))) ||
	   (p->item.sensor2_ch1_set & (0x00000001 << (M_B - 1))) ||
	   (p->item.sensor2_ch2_set & (0x00000001 << (M_B - 1))) ||
	   (p->item.time1_set 		& (0x00000001 << (M_B - 1))) ||
	   (p->item.time2_set 		& (0x00000001 << (M_B - 1))) ||
	   (p->item.time3_set 		& (0x00000001 << (M_B - 1))) ||
	   (p->item.time4_set 		& (0x00000001 << (M_B - 1))))
	{
		if((p->item.value_set & (0x00000001 << (M_B - 1))) ||
		   (p->item.timer_set & (0x00000001 << (M_B - 1))))
		{
			if((p->item.k & (0x0001 << (M_B - 1))) == OFF)
			{
				x01_request(p->item.id, M_B, ON);
				RT_DEBUG_LOG(DEBUG_WARN, ("M_B ON.\n"));	
			}
		}
		else
		{
			if(p->item.k & (0x0001 << (M_B - 1)))
			{
				x01_request(p->item.id, M_B, OFF);
				RT_DEBUG_LOG(DEBUG_WARN, ("M_B OFF.\n"));
			}
		}
	}
	

	// M_C
	if((p->item.sensor1_ch1_set & (0x00000001 << (M_C - 1))) ||
	   (p->item.sensor1_ch2_set & (0x00000001 << (M_C - 1))) ||
	   (p->item.sensor2_ch1_set & (0x00000001 << (M_C - 1))) ||
	   (p->item.sensor2_ch2_set & (0x00000001 << (M_C - 1))) ||
	   (p->item.time1_set 		& (0x00000001 << (M_C - 1))) ||
	   (p->item.time2_set 		& (0x00000001 << (M_C - 1))) ||
	   (p->item.time3_set 		& (0x00000001 << (M_C - 1))) ||
	   (p->item.time4_set 		& (0x00000001 << (M_C - 1))))
	{
		if((p->item.value_set & (0x00000001 << (M_C - 1))) ||
		   (p->item.timer_set & (0x00000001 << (M_C - 1))))
		{
			if((p->item.k & (0x0001 << (M_C - 1))) == OFF)
			{
				x01_request(p->item.id, M_C, ON);
				RT_DEBUG_LOG(DEBUG_WARN, ("M_C ON.\n"));	
			}
		}
		else
		{
			if(p->item.k & (0x0001 << (M_C - 1)))
			{
				x01_request(p->item.id, M_C, OFF);
				RT_DEBUG_LOG(DEBUG_WARN, ("M_C OFF.\n"));
			}
		}
	}


	// 清 0
	p->item.value_set = 0;
	p->item.timer_set = 0;
}


static void control_set(void)
{
	// WARN_CONTROL_LED
	if((control_value_set 		& (0x00000001 << (WARN_CONTROL_LED - 1 + 8))) || 
	   (control_timer_set 		& (0x00000001 << (WARN_CONTROL_LED - 1 + 8))) ||
	   (control_relate_set 		& (0x00000001 << (WARN_CONTROL_LED - 1 + 8))) ||
	   (control_time_out_set 	& (0x00000001 << (WARN_CONTROL_LED - 1 + 8))) ||
	   (control_power_drop_set 	& (0x00000001 << (WARN_CONTROL_LED - 1 + 8))))
	{
		#if ( WARN_LED > 0 )
			if(warn_led_status() == WARN_LED_OFF)
			{
				warn_led_on();
			}
		#endif	
	}
	else
	{
		#if ( WARN_LED > 0 )
			if(warn_led_status() == WARN_LED_ON)
			{
				warn_led_off();
			}
		#endif
	}

	// WARN_CONTROL_BELL
	if((control_value_set 		& (0x00000001 << (WARN_CONTROL_BELL - 1 + 8))) || 
	   (control_timer_set 		& (0x00000001 << (WARN_CONTROL_BELL - 1 + 8))) ||
	   (control_relate_set 		& (0x00000001 << (WARN_CONTROL_BELL - 1 + 8))) ||
	   (control_time_out_set 	& (0x00000001 << (WARN_CONTROL_BELL - 1 + 8))) ||
	   (control_power_drop_set 	& (0x00000001 << (WARN_CONTROL_BELL - 1 + 8))))
	{
		#if ( WARN_BELL > 0 )
			if(warn_bell_status() == WARN_BELL_OFF)
			{
				warn_bell_on();
			}
		#endif
	}
	else
	{
		#if ( WARN_BELL > 0 )	
			if(warn_bell_status() == WARN_BELL_ON)
			{
				warn_bell_off();
			}
		#endif
	}

	// 清 0
	control_value_set = 0;
	control_timer_set = 0;
	control_relate_set = 0;
	control_time_out_set = 0;
	control_power_drop_set = 0;
}


extern List sensor;
static void warn_callBack(void *parameter)
{
	Position p = sensor;
	while(p != RT_NULL)
	{
		#if ( WARN_TIMER > 0 )
		if(fun.warn_timer == 1)
		{
			warn_judge_timer(p);
		}
		#endif
		
		#if ( WARN_VALUE > 0 )
		if(fun.warn_value == 1)
		{
			warn_judge_value(p);
		}
		#endif

		#if ( WARN_TIME_OUT > 0 )
		if(fun.warn_time_out == 1)
		{
			warn_time_out(p);
		}
		#endif

		#if ( WARN_RELATE > 0 )
		if(fun.warn_relate == 1)
		{
			warn_relate(p);
		}
		#endif

		sensor_set(p);		
		p = p->next;
	}


	#if ( WARN_POWER_DROP > 0 )
	if(fun.warn_power_drop == 1)
	{
		warn_power_drop();
	}
	#endif
	
	control_set();		
}


static struct rt_timer time_warn;
void init_time_warn(void)
{
	RT_DEBUG_LOG(DEBUG_WARN, ("init time warn.\n"));
	
	rt_timer_init(&time_warn, "warn", warn_callBack, RT_NULL, 300, RT_TIMER_FLAG_SOFT_TIMER | RT_TIMER_FLAG_PERIODIC); 
   	rt_timer_start(&time_warn);
}
#endif
