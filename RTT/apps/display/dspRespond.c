#include <display.h>


#if ( DISPLAY_EN > 0 )

void x01_dsp_respond(rt_uint8_t *b)
{	
	rt_uint16_t year = (b[0]<<8) | b[1];
	_set_UTC8_time(year - 2000, b[2], b[3], b[4], b[5], b[6]);				
}

void x02_dsp_respond(rt_uint8_t *b)
{
	_get_sensor_value();						
}

void x03_dsp_respond(rt_uint8_t *b)
{
	_get_query_sensor_cnt();						
}

void x04_dsp_respond(rt_uint8_t *b)
{
	_get_query_sensor_value(b[0]);					
}

void x05_dsp_respond(rt_uint8_t *b)
{
	_set_query_sensor_value(b);						
}

void x06_dsp_respond(rt_uint8_t *b)
{
	_get_time_cnt();						
}

void x07_dsp_respond(rt_uint8_t *b)
{
	_get_time_value(b[0]);						
}

void x08_dsp_respond(rt_uint8_t *b)
{
	_set_time_value(b);				
}

#endif
