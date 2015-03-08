#include <rs485.h>
#include "rtc.h"

#if ( RS485_EN > 0 )

void x01_respond(rt_uint8_t *b)
{
					
}


void x02_respond(rt_uint8_t *b)
{
	Element *e = find_sensor(b);
	e->sensor1_ch1 = ((rt_uint16_t) (b[ID_LEN + CMD_LEN + DATA1] << 8)) | b[ID_LEN + CMD_LEN + DATA2];
	e->sensor1_ch2 = ((rt_uint16_t) (b[ID_LEN + CMD_LEN + DATA3] << 8)) | b[ID_LEN + CMD_LEN + DATA4];
}


void x03_respond(rt_uint8_t *b)
{
	Element *e = find_sensor(b);
	e->sensor2_ch1 = ((rt_uint16_t) (b[ID_LEN + CMD_LEN + DATA1] << 8)) | b[ID_LEN + CMD_LEN + DATA2];
	e->sensor2_ch2 = ((rt_uint16_t) (b[ID_LEN + CMD_LEN + DATA3] << 8)) | b[ID_LEN + CMD_LEN + DATA4];	
}


void x04_respond(rt_uint8_t *b)
{
	Element *e = find_sensor(b);
	e->k = b[ID_LEN + CMD_LEN + DATA2] | b[ID_LEN + CMD_LEN + DATA1]<<8;
}


void rec_last_time(rt_uint8_t *b)
{
	Element *e = find_sensor(b);
	e->last_rec_time = get_rs485_timer();
}

#endif
