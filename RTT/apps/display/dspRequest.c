#include <display.h>
#include <string.h>
#include "strLib.h"


#if ( DISPLAY_EN > 0 )

static void fill_request(rt_uint8_t *b, rt_uint8_t cmd, rt_uint8_t *d)
{
    memset(b, 0x00, U_ALL_LEN);
    
    b[0] = U_HEAD;
    b[U_HEAD_LEN] = cmd;
    memcpy(b + U_HEAD_LEN + U_CMD_LEN, d, U_DATA_LEN);
    b[U_ALL_LEN - 1] = U_END;
}


static void x01_dsp_request(void)
{
    uint8_t b[U_ALL_LEN];
    uint8_t d[U_DATA_LEN];

    memset(d, 0x00, U_DATA_LEN);
	get_UTC8_time(d+1);
    d[0] = (2000 + bcd8_to_hex8(d[1]))>>8;
	d[1] = (2000 + bcd8_to_hex8(d[1]));
	d[2] = bcd8_to_hex8(d[2]); 
	d[3] = bcd8_to_hex8(d[3]);
	d[4] = bcd8_to_hex8(d[4]); 
	d[5] = bcd8_to_hex8(d[5]);
	d[6] = bcd8_to_hex8(d[6]);
	d[7] = 0;
   
    fill_request(b, U_TIME_CMD, d);
    _display_send(b, U_ALL_LEN);
}


void x02_dsp_request(rt_uint8_t which, rt_uint16_t temp, rt_uint16_t ph, rt_uint16_t Do)
{
	uint8_t b[U_ALL_LEN];
    uint8_t d[U_DATA_LEN];

    memset(d, 0x00, U_DATA_LEN);
    d[0] = which;
	d[1] = temp << 8; d[2] = temp;
	d[3] = ph << 8;   d[4] = ph;
	d[5] = Do << 8;   d[6] = Do;
    fill_request(b, U_SENSOR_CMD, d);
    _display_send(b, U_ALL_LEN);
}


void x03_dsp_request(rt_uint8_t cnt)
{
	uint8_t b[U_ALL_LEN];
    uint8_t d[U_DATA_LEN];

    memset(d, 0x00, U_DATA_LEN);
    d[0] = cnt;
    fill_request(b, U_QUERY_SENSOR_CMD, d);
    _display_send(b, U_ALL_LEN);
}


void x04_dsp_request(rt_uint8_t temp_min, rt_uint8_t temp_max, 
					 rt_uint8_t ph_min, rt_uint8_t ph_max, 
					 rt_uint8_t do_min, rt_uint8_t do_max, 
					 rt_uint8_t relate)
{
	uint8_t b[U_ALL_LEN];
    uint8_t d[U_DATA_LEN];

    memset(d, 0x00, U_DATA_LEN);
    d[0] = temp_min; d[1] = temp_max;
	d[2] = ph_min; d[3] = ph_max;
	d[4] = do_min; d[5] = do_max; 
	d[6] = relate;  
    fill_request(b, U_QUERY_SENSOR_VALUE_CMD, d);
    _display_send(b, U_ALL_LEN);
}


void x06_dsp_request(rt_uint8_t cnt)
{
	uint8_t b[U_ALL_LEN];
    uint8_t d[U_DATA_LEN];

    memset(d, 0x00, U_DATA_LEN);
    d[0] = cnt;
    fill_request(b, U_QUERY_TIMER_CMD, d);
    _display_send(b, U_ALL_LEN);
}


void x07_dsp_request(rt_uint8_t which, rt_uint8_t  h1, rt_uint8_t m1, rt_uint8_t h2, rt_uint8_t m2)
{
	uint8_t b[U_ALL_LEN];
    uint8_t d[U_DATA_LEN];

    memset(d, 0x00, U_DATA_LEN);
    d[0] = which;
	d[1] = h1; d[2] = m1;
	d[3] = h2; d[4] = m2;
    fill_request(b, U_QUERY_TIMER_VALUE_CMD, d);
    _display_send(b, U_ALL_LEN);
}


void display_send_thread_entry(void *parameter)
{	
	while (RT_TRUE)
	{		
		rt_thread_delay(100);
		x01_dsp_request();		
	}
}

#endif
