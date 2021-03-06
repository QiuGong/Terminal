#include <display.h>
#include "rs485.h"
#include "rtc.h"
#include "formula.h"
#include "strLib.h"
#include "warn.h"


#if ( DISPLAY_EN > 0 )

/**
 * 获取报警标志
 *
 * @param 
 *
 * @return 
 */
rt_uint8_t _get_warn_flag(void)
{		
	rt_uint8_t flag = U_NORMAL;
	
	// 超时
	if(get_timeout_warn_flag() == 1)
	{
		flag = U_TIMEOUT;
	}
	// 溶氧
	else if(get_value_warn_flag() == 2)
	{
		flag = U_DO;
	}
	// PH
	else if(get_value_warn_flag() == 3)
	{
		flag = U_PH;
	}
	// 温度
	else if(get_value_warn_flag() == 4)
	{
		flag = U_TEMP;
	}	

	return flag;	
}


/**
 * 显示屏接收线程
 *
 * @param 
 *
 * @return 
 */
void display_rec_thread_entry(void* parameter)
{
	_display_rec_thread_entry(parameter);
}


/**
 * 获取北京时间
 *
 * @param buf:6个字节，年月日时分秒。（备注：年-2000）
 *
 * @return 
 */
void _get_UTC8_time(rt_uint8_t *buf)
{
#if ( DS1302_EN > 0 )
	get_time(buf);
#endif
}


/**
 * 设置北京时间
 *
 * @param year,mon,day,hour,min,sec（备注：年-2000）
 *
 * @return 
 */
void _set_UTC8_time(rt_uint8_t year, rt_uint8_t mon, rt_uint8_t day, rt_uint8_t hour, rt_uint8_t min, rt_uint8_t sec)
{
#if ( DS1302_EN > 0 )
	set_rtc(year, mon, day, hour, min, sec);	
#endif
}


/**
 * 获取传感器显示值（温度，酸碱，溶氧）
 *
 * @param 
 *
 * @return 
 */
void _get_sensor_value(void)
{
#if ( RS485_EN > 0 )
	Position p = get_sensor_list();
	rt_uint8_t i = 0;

	while(p != RT_NULL)
	{	
		rt_uint16_t temp = 0, ph = 0, Do = 0;	

		i++;				
		if(p->item.is_show == 0)
		{
			// 公式计算
			temp = formula_temp_16(p->item.sensor2_ch2);
			ph 	 = formula_ph_16(p->item.sensor2_ch1);
			Do 	 = formula_do_16(p->item.sensor1_ch2);
	
			// 发送
			x02_dsp_request(i, temp, ph, Do);			
	
			// 延时5S
			rt_thread_delay(500);
		}
		p = p->next;
	}

	// 返回界面
	x02_dsp_request(0, 0, 0, 0);
#else

	// 返回界面
	rt_thread_delay(30);
	x02_dsp_request(0, 0, 0, 0);
#endif
}


/**
 * 获取传感器组数
 *
 * @param 
 *
 * @return 
 */
void _get_query_sensor_cnt(void)
{
#if ( RS485_EN > 0 )
	Position p = get_sensor_list();
	rt_uint8_t cnt = 0;

	while(p != RT_NULL)
	{		
		cnt++;
		p = p->next;
	}

	x03_dsp_request(cnt);
#else

	// 返回界面
	rt_thread_delay(30);
	x02_dsp_request(0, 0, 0, 0);
#endif
}


/**
 * 获取传感器设置值 （温度，酸碱，溶氧）
 *
 * @param which:哪一组
 *
 * @return 
 */
void _get_query_sensor_value(rt_uint8_t which)
{
#if ( RS485_EN > 0 )
	Position p = get_sensor_list();
	rt_uint8_t i = 0;

	while(p != RT_NULL)
	{	
		if((++i) == which)
		{
			x04_dsp_request (formula_temp_16(p->item.sensor2_ch2_min), 		formula_temp_16(p->item.sensor2_ch2_max),
							// PH 
					 		(formula_ph_16(p->item.sensor2_ch1_min)/10) + ((formula_ph_16(p->item.sensor2_ch1_min)%10) >= 5 ? 1:0),	
							(formula_ph_16(p->item.sensor2_ch1_max)/10) + ((formula_ph_16(p->item.sensor2_ch1_min)%10) >= 5 ? 1:0),
							// do 
					 		(formula_do_16(p->item.sensor1_ch2_min)/10), 	(formula_do_16(p->item.sensor1_ch2_max)/10),
							// backup 
					 		p->item.s1c2_set_backup == 0 ? 1 : 0);		
		}

		p = p->next;
	}
#else

	// 返回界面
	rt_thread_delay(30);
	x02_dsp_request(0, 0, 0, 0);
#endif
}


/**
 * 设置传感器设置值 （温度，酸碱，溶氧）
 *
 * @param b:8位数据（定义详见协议）
 *
 * @return 
 */
void _set_query_sensor_value(rt_uint8_t *b)
{
#if ( RS485_EN > 0 )
	Position p = get_sensor_list();
	rt_uint8_t i = 0;

	while(p != RT_NULL)
	{	
		if((++i) == b[0])
		{
			p->item.sensor2_ch2_min = formula_temp_8(b[1]);
			p->item.sensor2_ch2_max = formula_temp_8(b[2]);
			p->item.sensor2_ch1_min	= formula_ph_8(b[3]); 
			p->item.sensor2_ch1_max	= formula_ph_8(b[4]); 
			p->item.sensor1_ch2_min	= formula_do_8(b[5]); 
			p->item.sensor1_ch2_max	= formula_do_8(b[6]); 
			set_backup(p, 2, b[7]); 
		}
		p = p->next;
	}

	refresh_sensor_to_flash();
#else

	// 返回界面
	rt_thread_delay(30);
	x02_dsp_request(0, 0, 0, 0);
#endif
}


/**
 * 获取定时器组数
 *
 * @param 
 *
 * @return 
 */
void _get_time_cnt(void)
{
#if ( RS485_EN > 0 )
	Position p = get_sensor_list();
	rt_uint8_t cnt = 0;

	while(p != RT_NULL)
	{		
		cnt++;
		p = p->next;
	}

	x06_dsp_request(cnt);
#else

	// 返回界面
	rt_thread_delay(30);
	x02_dsp_request(0, 0, 0, 0);
#endif
}


/**
 * 获取定时器设置值
 *
 * @param which:哪一组
 *
 * @return 
 */
void _get_time_value(rt_uint8_t which)
{
#if ( RS485_EN > 0 )
	Position p = get_sensor_list();
	rt_uint8_t i = 0;

	while(p != RT_NULL)
	{	
		if((++i) == which)
		{
			x07_dsp_request(1, bcd8_to_hex8(p->item.time1_start>>8), bcd8_to_hex8(p->item.time1_start), 
								  bcd8_to_hex8(p->item.time1_end>>8), bcd8_to_hex8(p->item.time1_end));				
			rt_thread_delay(30);

			x07_dsp_request(2, bcd8_to_hex8(p->item.time2_start>>8), bcd8_to_hex8(p->item.time2_start), 
								  bcd8_to_hex8(p->item.time2_end>>8), bcd8_to_hex8(p->item.time2_end));				
			rt_thread_delay(30);

			x07_dsp_request(3, bcd8_to_hex8(p->item.time3_start>>8), bcd8_to_hex8(p->item.time3_start), 
								  bcd8_to_hex8(p->item.time3_end>>8), bcd8_to_hex8(p->item.time3_end));				
			rt_thread_delay(30);

			x07_dsp_request(4, bcd8_to_hex8(p->item.time4_start>>8), bcd8_to_hex8(p->item.time4_start), 
								  bcd8_to_hex8(p->item.time4_end>>8), bcd8_to_hex8(p->item.time4_end));				
			rt_thread_delay(30);

			x07_dsp_request(5, bcd8_to_hex8(p->item.time5_start>>8), bcd8_to_hex8(p->item.time5_start), 
								  bcd8_to_hex8(p->item.time5_end>>8), bcd8_to_hex8(p->item.time5_end));				
			rt_thread_delay(30);

			x07_dsp_request(6, bcd8_to_hex8(p->item.time6_start>>8), bcd8_to_hex8(p->item.time6_start), 
								  bcd8_to_hex8(p->item.time6_end>>8), bcd8_to_hex8(p->item.time6_end));				
			rt_thread_delay(30);

			x07_dsp_request(7, bcd8_to_hex8(p->item.time7_start>>8), bcd8_to_hex8(p->item.time7_start), 
								  bcd8_to_hex8(p->item.time7_end>>8), bcd8_to_hex8(p->item.time7_end));				
			rt_thread_delay(30);

 			x07_dsp_request(8, bcd8_to_hex8(p->item.time8_start>>8), bcd8_to_hex8(p->item.time8_start), 
								  bcd8_to_hex8(p->item.time8_end>>8), bcd8_to_hex8(p->item.time8_end));				
			rt_thread_delay(30);

			x07_dsp_request(9, bcd8_to_hex8(p->item.time9_start>>8), bcd8_to_hex8(p->item.time9_start), 
								  bcd8_to_hex8(p->item.time9_end>>8), bcd8_to_hex8(p->item.time9_end));				
			rt_thread_delay(30);

			x07_dsp_request(10, bcd8_to_hex8(p->item.time10_start>>8), bcd8_to_hex8(p->item.time10_start), 
								  bcd8_to_hex8(p->item.time10_end>>8), bcd8_to_hex8(p->item.time10_end));				
			rt_thread_delay(30);

			x07_dsp_request(11, bcd8_to_hex8(p->item.time11_start>>8), bcd8_to_hex8(p->item.time11_start), 
								  bcd8_to_hex8(p->item.time11_end>>8), bcd8_to_hex8(p->item.time11_end));				
			rt_thread_delay(30);

			x07_dsp_request(12, bcd8_to_hex8(p->item.time12_start>>8), bcd8_to_hex8(p->item.time12_start), 
								  bcd8_to_hex8(p->item.time12_end>>8), bcd8_to_hex8(p->item.time12_end));				
			rt_thread_delay(30);

			x07_dsp_request(13, bcd8_to_hex8(p->item.time13_start>>8), bcd8_to_hex8(p->item.time13_start), 
								  bcd8_to_hex8(p->item.time13_end>>8), bcd8_to_hex8(p->item.time13_end));				
			rt_thread_delay(30);

			x07_dsp_request(14, bcd8_to_hex8(p->item.time14_start>>8), bcd8_to_hex8(p->item.time14_start), 
								  bcd8_to_hex8(p->item.time14_end>>8), bcd8_to_hex8(p->item.time14_end));				
			rt_thread_delay(30);

 			x07_dsp_request(15, bcd8_to_hex8(p->item.time15_start>>8), bcd8_to_hex8(p->item.time15_start), 
								  bcd8_to_hex8(p->item.time15_end>>8), bcd8_to_hex8(p->item.time15_end));				
			rt_thread_delay(30);

			x07_dsp_request(16, bcd8_to_hex8(p->item.time16_start>>8), bcd8_to_hex8(p->item.time16_start), 
								  bcd8_to_hex8(p->item.time16_end>>8), bcd8_to_hex8(p->item.time16_end));				
			rt_thread_delay(30);
		}

		p = p->next;
	}
#else

	// 返回界面
	rt_thread_delay(30);
	x02_dsp_request(0, 0, 0, 0);
#endif
}


/**
 * 设置定时器设置值 
 *
 * @param b:6位数据（定义详见协议）
 *
 * @return 
 */
void _set_time_value(rt_uint8_t *b)
{
#if ( RS485_EN > 0 )
	Position p = get_sensor_list();
	rt_uint8_t i = 0;

	while(p != RT_NULL)
	{	
		if((++i) == b[0])
		{
			if(b[1] == 1)
			{
				p->item.time1_start = (hex8_to_bcd8(b[2]) << 8) | hex8_to_bcd8(b[3]);
				p->item.time1_end   = (hex8_to_bcd8(b[4]) << 8) | hex8_to_bcd8(b[5]);
			}
			else if(b[1] == 2)
			{
				p->item.time2_start = (hex8_to_bcd8(b[2]) << 8) | hex8_to_bcd8(b[3]);
				p->item.time2_end   = (hex8_to_bcd8(b[4]) << 8) | hex8_to_bcd8(b[5]);
			}
			else if(b[1] == 3)
			{
				p->item.time3_start = (hex8_to_bcd8(b[2]) << 8) | hex8_to_bcd8(b[3]);
				p->item.time3_end   = (hex8_to_bcd8(b[4]) << 8) | hex8_to_bcd8(b[5]);
			}
			else if(b[1] == 4)
			{
				p->item.time4_start = (hex8_to_bcd8(b[2]) << 8) | hex8_to_bcd8(b[3]);
				p->item.time4_end   = (hex8_to_bcd8(b[4]) << 8) | hex8_to_bcd8(b[5]);
			}
			else if(b[1] == 5)
			{
				p->item.time5_start = (hex8_to_bcd8(b[2]) << 8) | hex8_to_bcd8(b[3]);
				p->item.time5_end   = (hex8_to_bcd8(b[4]) << 8) | hex8_to_bcd8(b[5]);
			}
			else if(b[1] == 6)
			{
				p->item.time6_start = (hex8_to_bcd8(b[2]) << 8) | hex8_to_bcd8(b[3]);
				p->item.time6_end   = (hex8_to_bcd8(b[4]) << 8) | hex8_to_bcd8(b[5]);
			}
			else if(b[1] == 7)
			{
				p->item.time7_start = (hex8_to_bcd8(b[2]) << 8) | hex8_to_bcd8(b[3]);
				p->item.time7_end   = (hex8_to_bcd8(b[4]) << 8) | hex8_to_bcd8(b[5]);
			}
			else if(b[1] == 8)
			{
				p->item.time8_start = (hex8_to_bcd8(b[2]) << 8) | hex8_to_bcd8(b[3]);
				p->item.time8_end   = (hex8_to_bcd8(b[4]) << 8) | hex8_to_bcd8(b[5]);
			}
			else if(b[1] == 9)
			{
				p->item.time9_start = (hex8_to_bcd8(b[2]) << 8) | hex8_to_bcd8(b[3]);
				p->item.time9_end   = (hex8_to_bcd8(b[4]) << 8) | hex8_to_bcd8(b[5]);
			}
			else if(b[1] == 10)
			{
				p->item.time10_start = (hex8_to_bcd8(b[2]) << 8) | hex8_to_bcd8(b[3]);
				p->item.time10_end   = (hex8_to_bcd8(b[4]) << 8) | hex8_to_bcd8(b[5]);
			}
			else if(b[1] == 11)
			{
				p->item.time11_start = (hex8_to_bcd8(b[2]) << 8) | hex8_to_bcd8(b[3]);
				p->item.time11_end   = (hex8_to_bcd8(b[4]) << 8) | hex8_to_bcd8(b[5]);
			}
			else if(b[1] == 12)
			{
				p->item.time12_start = (hex8_to_bcd8(b[2]) << 8) | hex8_to_bcd8(b[3]);
				p->item.time12_end   = (hex8_to_bcd8(b[4]) << 8) | hex8_to_bcd8(b[5]);
			}
			else if(b[1] == 13)
			{
				p->item.time13_start = (hex8_to_bcd8(b[2]) << 8) | hex8_to_bcd8(b[3]);
				p->item.time13_end   = (hex8_to_bcd8(b[4]) << 8) | hex8_to_bcd8(b[5]);
			}
			else if(b[1] == 14)
			{
				p->item.time14_start = (hex8_to_bcd8(b[2]) << 8) | hex8_to_bcd8(b[3]);
				p->item.time14_end   = (hex8_to_bcd8(b[4]) << 8) | hex8_to_bcd8(b[5]);
			}
			else if(b[1] == 15)
			{
				p->item.time15_start = (hex8_to_bcd8(b[2]) << 8) | hex8_to_bcd8(b[3]);
				p->item.time15_end   = (hex8_to_bcd8(b[4]) << 8) | hex8_to_bcd8(b[5]);
			}
			else if(b[1] == 16)
			{
				p->item.time16_start = (hex8_to_bcd8(b[2]) << 8) | hex8_to_bcd8(b[3]);
				p->item.time16_end   = (hex8_to_bcd8(b[4]) << 8) | hex8_to_bcd8(b[5]);
			}			

			refresh_sensor_to_flash();
		}
		p = p->next;
	}
#else

	// 返回界面
	rt_thread_delay(30);
	x02_dsp_request(0, 0, 0, 0);
#endif
}


#ifdef RT_USING_FINSH
#include <finsh.h>
static void dsp_warn(void)
{
	if(_get_warn_flag() == U_TIMEOUT)
	{
		rt_kprintf("TIMEOUT");	
	}
	// 溶氧
	else if(_get_warn_flag() == U_DO)
	{
		rt_kprintf("DO");
	}
	// PH
	else if(_get_warn_flag() == U_PH)
	{
		rt_kprintf("PH");
	}
	// 温度
	else if(_get_warn_flag() == U_TEMP)
	{
		rt_kprintf("TEMP");
	}
	// 正常
	else
	{
		rt_kprintf("NORMAL");
	}
}

FINSH_FUNCTION_EXPORT(dsp_warn, e.g: dsp_warn())
#endif

#endif
