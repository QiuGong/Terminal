#include <display.h>
#include "rtc.h"


#if ( DISPLAY_EN > 0 )

void x01_dsp_respond(rt_uint8_t *b)
{	
#if ( DS1302_EN > 0 )
	rt_uint16_t year = (b[0]<<8) | b[1];
	set_rtc(year - 2000, b[2], b[3], b[4], b[5], b[6]);	
#endif			
}

void x02_dsp_respond(rt_uint8_t *b)
{
	get_sensor_value();						
}

void x03_dsp_respond(rt_uint8_t *b)
{
	get_query_sensor_cnt();						
}

void x04_dsp_respond(rt_uint8_t *b)
{
	get_query_sensor_value(b[0]);					
}

void x05_dsp_respond(rt_uint8_t *b)
{
	set_query_sensor_value(b);						
}

void x06_dsp_respond(rt_uint8_t *b)
{
	get_time_cnt();						
}

void x07_dsp_respond(rt_uint8_t *b)
{
	get_time_value(b[0]);						
}

void x08_dsp_respond(rt_uint8_t *b)
{
	set_time_value(b);				
}

#endif
