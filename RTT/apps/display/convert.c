#include <display.h>
#include "rs485.h"
#include "util.h"
#include "strLib.h"


#if ( DISPLAY_EN > 0 )

void get_sensor_value(void)
{
#if ( RS485_EN > 0 )
	Position p = sensor;
	rt_uint8_t i = 0;

	while(p != RT_NULL)
	{	
		rt_uint16_t temp = 0, ph = 0, Do = 0;	

		// 公式计算
		temp = formula_temp_16(p->item.sensor2_ch2);
		ph 	 = formula_ph_16(p->item.sensor2_ch1);
		Do 	 = formula_do_16(p->item.sensor1_ch2);

		// 发送
		x02_dsp_request((++i), temp, ph, Do);
		p = p->next;

		// 延时3S
		rt_thread_delay(300);
	}

	// 返回界面
	x02_dsp_request(0, 0, 0, 0);
#else

	// 返回界面
	rt_thread_delay(30);
	x02_dsp_request(0, 0, 0, 0);
#endif
}


void get_query_sensor_cnt(void)
{
#if ( RS485_EN > 0 )
	Position p = sensor;
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


// 关联未做？？？？？？
void get_query_sensor_value(rt_uint8_t which)
{
#if ( RS485_EN > 0 )
	Position p = sensor;
	rt_uint8_t i = 0;

	while(p != RT_NULL)
	{	
		if((++i) == which)
		{
			x04_dsp_request((formula_temp_16(p->item.sensor2_ch2_min)/10), 	(formula_temp_16(p->item.sensor2_ch2_max)/10), 
					 		(formula_ph_16(p->item.sensor2_ch1_min)/10),	(formula_ph_16(p->item.sensor2_ch1_max)/10), 
					 		(formula_do_16(p->item.sensor1_ch2_min)/10), 	(formula_do_16(p->item.sensor1_ch2_max)/10), 
					 		0);		
		}

		p = p->next;
	}
#else

	// 返回界面
	rt_thread_delay(30);
	x02_dsp_request(0, 0, 0, 0);
#endif
}


// 关联未做？？？？？？
void set_query_sensor_value(rt_uint8_t *b)
{
#if ( RS485_EN > 0 )
	Position p = sensor;
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
			b[7]					= b[7];  
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


void get_time_cnt(void)
{
#if ( RS485_EN > 0 )
	Position p = sensor;
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


void get_time_value(rt_uint8_t which)
{
#if ( RS485_EN > 0 )
	Position p = sensor;
	rt_uint8_t i = 0;

	while(p != RT_NULL)
	{	
		if((++i) == which)
		{
			x07_dsp_request(0x01, bcd8_to_hex8(p->item.time1_start>>8), bcd8_to_hex8(p->item.time1_start), 
								  bcd8_to_hex8(p->item.time1_end>>8), bcd8_to_hex8(p->item.time1_end));				
			rt_thread_delay(30);

			x07_dsp_request(0x02, bcd8_to_hex8(p->item.time2_start>>8), bcd8_to_hex8(p->item.time2_start), 
								  bcd8_to_hex8(p->item.time2_end>>8), bcd8_to_hex8(p->item.time2_end));				
			rt_thread_delay(30);

			x07_dsp_request(0x03, bcd8_to_hex8(p->item.time3_start>>8), bcd8_to_hex8(p->item.time3_start), 
								  bcd8_to_hex8(p->item.time3_end>>8), bcd8_to_hex8(p->item.time3_end));				
			rt_thread_delay(30);

			x07_dsp_request(0x04, bcd8_to_hex8(p->item.time4_start>>8), bcd8_to_hex8(p->item.time4_start), 
								  bcd8_to_hex8(p->item.time4_end>>8), bcd8_to_hex8(p->item.time4_end));				
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


void set_time_value(rt_uint8_t *b)
{
#if ( RS485_EN > 0 )
	Position p = sensor;
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
#endif
