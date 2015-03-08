#include <rs485.h>
#include <string.h>


#if ( RS485_EN > 0 )
static rt_uint32_t rs485_timer = 0;


static void fill_request(rt_uint8_t *b, rt_uint8_t *id, rt_uint8_t cmd, rt_uint8_t *d)
{
    memset(b, 0x00, ALL_LEN);
    
    b[0] = HEAD;
	memcpy(b + HEAD_LEN, id, ID_LEN);
    b[HEAD_LEN + ID_LEN] = cmd;
    memcpy(b + HEAD_LEN + ID_LEN + CMD_LEN, d, DATA_LEN);
    b[ALL_LEN - 1] = END;
}


void x01_request(rt_uint8_t *id, enum X01_DEV dev, enum X01_CONTROL control)
{
    rt_uint8_t b[ALL_LEN];

    rt_uint8_t d[DATA_LEN];
	d[DATA1] = dev;
    d[DATA2] = control;

    fill_request(b, id, TURN_CMD, d);
    re485_send(b, ALL_LEN);
}


static void x02_x03_x04_request(rt_uint8_t *id, rt_uint8_t cmd)
{
    rt_uint8_t b[ALL_LEN];
    rt_uint8_t d[DATA_LEN];

	memset(d, 0x00, DATA_LEN);

    fill_request(b, id, cmd, d);
    re485_send(b, ALL_LEN);
}


rt_uint32_t get_rs485_timer(void)
{
	return rs485_timer;
}


extern List sensor;
void rs485_send_thread_entry(void *parameter)
{	
	while (RT_TRUE)
	{
		Position p = sensor;
	
		if(sensor == NULL)
		{
		 	read_sensor_from_flash();
			p = sensor;
		}
	
		while(p != NULL){
	
			p->item.sensor1_ch1 = 0;
			p->item.sensor1_ch2 = 0;
			x02_x03_x04_request(p->item.id, SENSOR_1);
			rt_thread_delay(10);
	
			p->item.sensor2_ch1 = 0;
			p->item.sensor2_ch2 = 0;
			x02_x03_x04_request(p->item.id, SENSOR_2);
			rt_thread_delay(10);
	
			p->item.k = 0;
			x02_x03_x04_request(p->item.id, DETECT_K);
			rt_thread_delay(10);
		
			p = p->next;
		}
		
		rt_thread_delay(300);
		rs485_timer ++;		
	}
}

#endif
