#include "warn.h"


// Ó³Éä

//ID,  ×éºÅ
#define id(x)	((x & 0xff00)>>8)	
#define	num(x)	(x & 0x00ff)	


#if ( WARN_VALUE > 0 )

static rt_uint16_t	get_value(rt_uint8_t id, rt_uint8_t num)
{
	Position p = _get_sensor();
	rt_uint8_t i = 0;	

	while(p != RT_NULL)
	{
		if((++i) == id)
		{
			if(num == 1)
			{
				return p->item.sensor1_ch1;
			}
			else if(num == 2)
			{
				return p->item.sensor1_ch2;
			}
			else if(num == 3)
			{
				return p->item.sensor2_ch1;
			}
			else if(num == 4)
			{
				return p->item.sensor2_ch2;
			}
		}

		p = p->next;
	}

	return 0;
}


void warn_rep(Position p)
{
	if(p->item.sensor1_ch1_rep > 0)
	{
		rt_uint8_t id  = id(p->item.sensor1_ch1_rep);
		rt_uint8_t num = num(p->item.sensor1_ch1_rep);
		p->item.sensor1_ch1 = get_value(id, num);
	}

	if(p->item.sensor1_ch2_rep > 0)
	{
		rt_uint8_t id  = id(p->item.sensor1_ch2_rep);
		rt_uint8_t num = num(p->item.sensor1_ch2_rep);
		p->item.sensor1_ch2 = get_value(id, num);
	}

	if(p->item.sensor2_ch1_rep > 0)
	{
		rt_uint8_t id  = id(p->item.sensor2_ch1_rep);
		rt_uint8_t num = num(p->item.sensor2_ch1_rep);
		p->item.sensor2_ch1 = get_value(id, num);
	}

	if(p->item.sensor2_ch2_rep > 0)
	{
		rt_uint8_t id  = id(p->item.sensor2_ch2_rep);
		rt_uint8_t num = num(p->item.sensor2_ch2_rep);
		p->item.sensor2_ch2 = get_value(id, num);
	}
}

#endif
