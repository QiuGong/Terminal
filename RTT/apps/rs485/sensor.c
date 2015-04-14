#include <string.h>
#include <rs485.h>
#include "strLib.h"


#if ( RS485_EN > 0 )

static List sensor =  RT_NULL;

List _get_sensor_list(void)
{
	return sensor;
}


rt_uint8_t add_sensor(rt_uint8_t *id) 
{		
	// 判断sensor是否存在，不存在创建
	if(sensor == RT_NULL)
	{
		sensor = MakeEmpty(sensor);			
		memcpy(sensor->item.id, (const void *)id, ID_LEN);

		RT_DEBUG_LOG(DEBUG_RS485, ("add sensor.\n"));
		return RT_TRUE;
	}

	// 查找id是否存在，不存在添加
	if(Find(id, sensor) == RT_NULL)
	{
		Position p;
	
		// 判断sensor是否为最大值
		if(Length(sensor) == MAX_SENSOR)
		{
			RT_DEBUG_LOG(DEBUG_RS485, ("add sensor fail, because is MAX.\n"));
			return RT_FALSE;
		}
				
		// 添加
		p = FindPrevious(id, sensor);
		Insert(p);
		memcpy(p->next->item.id, (const void *)id, ID_LEN);
		RT_DEBUG_LOG(DEBUG_RS485, ("add sensor.\n"));
		return RT_TRUE;
	}

	return RT_FALSE;
}


Element* find_sensor(rt_uint8_t *b)
{
	Position p = Find(b, sensor);
	return &(p->item);				    
}


static rt_uint8_t delete_sensor(rt_uint8_t *b)
{
	Position P;	

	P = Find(b, sensor);
	if(P != RT_NULL)
	{
		void* p = Delete(b, sensor);			
		if(p == (void *)RT_UINT32_MAX)
		{
			sensor = RT_NULL;
		}
		else if(p != RT_NULL)
		{
			sensor = p;
		}
		RT_DEBUG_LOG(DEBUG_RS485, ("delete sensor.\n"));

		return RT_TRUE;
	}

	return RT_FALSE;
}


void refresh_sensor_to_flash(void)
{
	rt_uint16_t len = 0;
	rt_uint8_t *buf = RT_NULL;
	Position p = sensor;


	buf = rt_malloc(get_max_len());
	if(buf == RT_NULL)
	{
		rt_kprintf("refresh_sensor_to_flash() rt_malloc failed.\n"); 
		return;
	}


	/* 填写数据 */
	buf[0] = '$';	len += 2;
	while(p != RT_NULL)
	{
		// ID
		len += positive(p->item.id, buf+len, ID_LEN);

		// IS_SHOW
		len += positive((rt_uint8_t *)&(p->item.is_show), buf+len, IS_SHOW_LEN);

		// 传感器
		len += negative((rt_uint8_t *)&(p->item.sensor1_ch1_min), buf+len, SENSOR_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.sensor1_ch1_max), buf+len, SENSOR_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.sensor1_ch1_mid), buf+len, SENSOR_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.sensor1_ch1_rep), buf+len, SENSOR_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.sensor1_ch1_set), buf+len, SENSOR_SET_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.s1c1_set_backup), buf+len, SENSOR_SET_PER_LEN);

		len += negative((rt_uint8_t *)&(p->item.sensor1_ch2_min), buf+len, SENSOR_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.sensor1_ch2_max), buf+len, SENSOR_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.sensor1_ch2_mid), buf+len, SENSOR_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.sensor1_ch2_rep), buf+len, SENSOR_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.sensor1_ch2_set), buf+len, SENSOR_SET_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.s1c2_set_backup), buf+len, SENSOR_SET_PER_LEN);

		len += negative((rt_uint8_t *)&(p->item.sensor2_ch1_min), buf+len, SENSOR_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.sensor2_ch1_max), buf+len, SENSOR_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.sensor2_ch1_mid), buf+len, SENSOR_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.sensor2_ch1_rep), buf+len, SENSOR_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.sensor2_ch1_set), buf+len, SENSOR_SET_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.s2c1_set_backup), buf+len, SENSOR_SET_PER_LEN);

		len += negative((rt_uint8_t *)&(p->item.sensor2_ch2_min), buf+len, SENSOR_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.sensor2_ch2_max), buf+len, SENSOR_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.sensor2_ch2_mid), buf+len, SENSOR_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.sensor2_ch2_rep), buf+len, SENSOR_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.sensor2_ch2_set), buf+len, SENSOR_SET_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.s2c2_set_backup), buf+len, SENSOR_SET_PER_LEN);

		// 定时器
		len += negative((rt_uint8_t *)&(p->item.time1_start), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time1_end), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time1_set), 	  buf+len, TIME_SET_PER_LEN);

		len += negative((rt_uint8_t *)&(p->item.time2_start), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time2_end), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time2_set), 	  buf+len, TIME_SET_PER_LEN);

		len += negative((rt_uint8_t *)&(p->item.time3_start), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time3_end), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time3_set), 	  buf+len, TIME_SET_PER_LEN);

		len += negative((rt_uint8_t *)&(p->item.time4_start), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time4_end), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time4_set), 	  buf+len, TIME_SET_PER_LEN);

		len += negative((rt_uint8_t *)&(p->item.time5_start), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time5_end), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time5_set), 	  buf+len, TIME_SET_PER_LEN);

		len += negative((rt_uint8_t *)&(p->item.time6_start), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time6_end), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time6_set), 	  buf+len, TIME_SET_PER_LEN);

		len += negative((rt_uint8_t *)&(p->item.time7_start), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time7_end), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time7_set), 	  buf+len, TIME_SET_PER_LEN);

		len += negative((rt_uint8_t *)&(p->item.time8_start), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time8_end), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time8_set), 	  buf+len, TIME_SET_PER_LEN);

		len += negative((rt_uint8_t *)&(p->item.time9_start), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time9_end), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time9_set), 	  buf+len, TIME_SET_PER_LEN);

		len += negative((rt_uint8_t *)&(p->item.time10_start), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time10_end), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time10_set), 	  buf+len, TIME_SET_PER_LEN);

		len += negative((rt_uint8_t *)&(p->item.time11_start), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time11_end), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time11_set), 	  buf+len, TIME_SET_PER_LEN);

		len += negative((rt_uint8_t *)&(p->item.time12_start), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time12_end), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time12_set), 	  buf+len, TIME_SET_PER_LEN);

		len += negative((rt_uint8_t *)&(p->item.time13_start), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time13_end), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time13_set), 	  buf+len, TIME_SET_PER_LEN);

		len += negative((rt_uint8_t *)&(p->item.time14_start), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time14_end), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time14_set), 	  buf+len, TIME_SET_PER_LEN);

		len += negative((rt_uint8_t *)&(p->item.time15_start), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time15_end), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time15_set), 	  buf+len, TIME_SET_PER_LEN);

		len += negative((rt_uint8_t *)&(p->item.time16_start), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time16_end), 	  buf+len, TIME_JUDGE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time16_set), 	  buf+len, TIME_SET_PER_LEN);

		// 关联控制
		len += negative((rt_uint8_t *)&(p->item.k1_relate), 	  buf+len, K_RELATE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.k2_relate), 	  buf+len, K_RELATE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.k3_relate), 	  buf+len, K_RELATE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.k4_relate), 	  buf+len, K_RELATE_PER_LEN);

		p = p->next;
	}
	buf[1] = len / PER_SENSOR_FLASH_LEN;
	refresh_to_flash(buf, len);


	rt_free(buf);
	RT_DEBUG_LOG(DEBUG_RS485, ("refresh sensor to flash.\n"));
}


void read_sensor_from_flash(void)
{
	rt_uint16_t len = 0;
	rt_uint8_t *buf = RT_NULL;

	
	buf = rt_malloc(get_max_len());
	if(buf == RT_NULL)
	{
		rt_kprintf("read_sensor_flash() rt_malloc failed.\n");
		return;
	}	


	/* 读取数据 */
	read_from_flash(buf, get_max_len());
	if(buf[len] == '$')
	{
		rt_uint8_t i;

		for(i=0, len=2; i<buf[1]; i++)
		{
			Position p;
						
			// 修改数据
			add_sensor(buf + len);
			p = Find(buf + len, sensor);
			len += ID_LEN;

			// 是否显示
			len += negative(buf+len, (rt_uint8_t *)&(p->item.is_show), IS_SHOW_LEN);

			// 传感器
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor1_ch1_min), SENSOR_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor1_ch1_max), SENSOR_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor1_ch1_mid), SENSOR_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor1_ch1_rep), SENSOR_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor1_ch1_set), SENSOR_SET_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.s1c1_set_backup), SENSOR_SET_PER_LEN);
	
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor1_ch2_min), SENSOR_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor1_ch2_max), SENSOR_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor1_ch2_mid), SENSOR_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor1_ch2_rep), SENSOR_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor1_ch2_set), SENSOR_SET_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.s1c2_set_backup), SENSOR_SET_PER_LEN);
	
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor2_ch1_min), SENSOR_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor2_ch1_max), SENSOR_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor2_ch1_mid), SENSOR_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor2_ch1_rep), SENSOR_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor2_ch1_set), SENSOR_SET_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.s2c1_set_backup), SENSOR_SET_PER_LEN);
	
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor2_ch2_min), SENSOR_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor2_ch2_max), SENSOR_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor2_ch2_mid), SENSOR_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor2_ch2_rep), SENSOR_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor2_ch2_set), SENSOR_SET_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.s2c2_set_backup), SENSOR_SET_PER_LEN);
	
			// 定时器
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time1_start), 	TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time1_end), 	TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time1_set), 	TIME_SET_PER_LEN);
	
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time2_start), 	TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time2_end), 	TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time2_set), 	TIME_SET_PER_LEN);
	
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time3_start), 	TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time3_end), 	TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time3_set), 	TIME_SET_PER_LEN);
	
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time4_start), 	TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time4_end), 	TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time4_set), 	TIME_SET_PER_LEN);
	
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time5_start), 	TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time5_end), 	TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time5_set), 	TIME_SET_PER_LEN);
	
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time6_start), 	TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time6_end), 	TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time6_set), 	TIME_SET_PER_LEN);
	
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time7_start), 	TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time7_end), 	TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time7_set), 	TIME_SET_PER_LEN);
	
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time8_start), 	TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time8_end), 	TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time8_set), 	TIME_SET_PER_LEN);
	
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time9_start), 	TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time9_end), 	TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time9_set), 	TIME_SET_PER_LEN);
	
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time10_start), TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time10_end), 	TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time10_set), 	TIME_SET_PER_LEN);
	
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time11_start), TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time11_end), 	TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time11_set), 	TIME_SET_PER_LEN);
	
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time12_start), TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time12_end), 	TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time12_set), 	TIME_SET_PER_LEN);
	
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time13_start), TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time13_end), 	TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time13_set), 	TIME_SET_PER_LEN);
	
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time14_start), TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time14_end), 	TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time14_set), 	TIME_SET_PER_LEN);
	
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time15_start), TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time15_end), 	TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time15_set), 	TIME_SET_PER_LEN);
	
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time16_start), TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time16_end), 	TIME_JUDGE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time16_set), 	TIME_SET_PER_LEN);

			// 关联
			len += negative(buf+len, (rt_uint8_t *)&(p->item.k1_relate), 	K_RELATE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.k2_relate), 	K_RELATE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.k3_relate), 	K_RELATE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.k4_relate), 	K_RELATE_PER_LEN);
		}	
	}


	rt_free(buf);
}


#ifdef RT_USING_FINSH
#include <finsh.h>
void list_sensor()
{
	Position p = sensor;
	rt_uint8_t i = 0;

	while(p != RT_NULL)
	{	
		print_sensor(p, &i);
		p = p->next;
	}
}
FINSH_FUNCTION_EXPORT(list_sensor, e.g: list_sensor())


void sensor_control(rt_uint8_t id, rt_uint8_t dev, rt_uint8_t control)
{	
	Position p = sensor;
	rt_uint8_t i = 0;	

	while(p != RT_NULL)
	{
		if((++i) == id)
		{
			_x01_request(p->item.id, (enum X01_DEV)dev, (enum X01_CONTROL)control);
			return;
		}							   

		p = p->next;
	}	
}
FINSH_FUNCTION_EXPORT(sensor_control, e.g: sensor_control(1,1,0))


void del_sensor(rt_uint8_t id)
{	
	Position p = sensor;
	rt_uint8_t i = 0;	

	while(p != RT_NULL)
	{
		if((++i) == id)
		{
			if(delete_sensor(p->item.id) == RT_TRUE)
			{
				refresh_sensor_to_flash();
			}
			return;
		}

		p = p->next;
	}	
}
FINSH_FUNCTION_EXPORT(del_sensor, e.g: del_sensor(1))


void add_ssr_hex(rt_uint8_t b1,rt_uint8_t b2,rt_uint8_t b3,rt_uint8_t b4, rt_uint8_t b5, rt_uint8_t b6,
				 rt_uint8_t b7,rt_uint8_t b8,rt_uint8_t b9,rt_uint8_t b10,rt_uint8_t b11,rt_uint8_t b12)
{	
	rt_uint8_t buf[ID_LEN];

	buf[0]  = hex8_to_bcd8(b1); 	buf[1]  = hex8_to_bcd8(b2); 	
	buf[2]  = hex8_to_bcd8(b3); 	buf[3]  = hex8_to_bcd8(b4);
	buf[4]  = hex8_to_bcd8(b5); 	buf[5]  = hex8_to_bcd8(b6); 	
	buf[6]  = hex8_to_bcd8(b7); 	buf[7]  = hex8_to_bcd8(b8);
	buf[8]  = hex8_to_bcd8(b9); 	buf[9]  = hex8_to_bcd8(b10); 	
	buf[10] = hex8_to_bcd8(b11); 	buf[11] = hex8_to_bcd8(b12);	

	if(add_sensor(buf) == RT_TRUE)
	{
		refresh_sensor_to_flash();	
	}	
}
FINSH_FUNCTION_EXPORT(add_ssr_hex, e.g: add_ssr_hex(79,0,6,0,22,0,0,0,0,0,34,30))


void add_ssr_bcd(rt_uint8_t b1,rt_uint8_t b2,rt_uint8_t b3,rt_uint8_t b4, rt_uint8_t b5, rt_uint8_t b6,
				 rt_uint8_t b7,rt_uint8_t b8,rt_uint8_t b9,rt_uint8_t b10,rt_uint8_t b11,rt_uint8_t b12)
{	
	rt_uint8_t buf[ID_LEN];

	buf[0]  = b1; 	buf[1]  = b2; 	buf[2]  = b3; 	buf[3]  = b4;
	buf[4]  = b5; 	buf[5]  = b6; 	buf[6]  = b7; 	buf[7]  = b8;
	buf[8]  = b9; 	buf[9]  = b10; 	buf[10] = b11; 	buf[11] = b12;	

	if(add_sensor(buf) == RT_TRUE)
	{
		refresh_sensor_to_flash();	
	}	
}
FINSH_FUNCTION_EXPORT(add_ssr_bcd, e.g: add_ssr_bcd(121,0,6,0,34,0,0,0,0,0,52,48))


void sensor_show(rt_uint8_t id, rt_uint8_t show)
{
	Position p = sensor;
	rt_uint8_t i = 0;	

	while(p != RT_NULL)
	{
		if((++i) == id)
		{
			p->item.is_show = show;
			refresh_sensor_to_flash();
			return;
		}

		p = p->next;
	}
}
FINSH_FUNCTION_EXPORT(sensor_show, e.g: sensor_show(1,1))


#define FRONT	0xFF00FF00
#define LATER	0x00FF00FF
void set_backup(Position p, rt_uint8_t ch, rt_uint8_t status)
{
	if(ch == 1)
	{
		if(status == 1)
		{													 
			if(p->item.s1c1_set_backup == 0)
			{
				p->item.s1c1_set_backup = p->item.sensor1_ch1_set & LATER;
				p->item.sensor1_ch1_set = p->item.sensor1_ch1_set & FRONT;
			}					
		}
		else if(status == 0)
		{					
			if(p->item.s1c1_set_backup != 0)
			{
				p->item.sensor1_ch1_set = p->item.s1c1_set_backup | p->item.sensor1_ch1_set;
				p->item.s1c1_set_backup = p->item.s1c1_set_backup & FRONT;
			}
		}	
	}
	else if(ch == 2)
	{
		if(status == 1)
		{													 
			if(p->item.s1c2_set_backup == 0)
			{
				p->item.s1c2_set_backup = p->item.sensor1_ch2_set & LATER;
				p->item.sensor1_ch2_set = p->item.sensor1_ch2_set & FRONT;
			}					
		}
		else if(status == 0)
		{					
			if(p->item.s1c2_set_backup != 0)
			{
				p->item.sensor1_ch2_set = p->item.s1c2_set_backup | p->item.sensor1_ch2_set;
				p->item.s1c2_set_backup = p->item.s1c2_set_backup & FRONT;
			}
		}	
	}
	else if(ch == 3)
	{
		if(status == 1)
		{													 
			if(p->item.s2c1_set_backup == 0)
			{
				p->item.s2c1_set_backup = p->item.sensor2_ch1_set & LATER;
				p->item.sensor2_ch1_set = p->item.sensor2_ch1_set & FRONT;
			}					
		}
		else if(status == 0)
		{					
			if(p->item.s2c1_set_backup != 0)
			{
				p->item.sensor2_ch1_set = p->item.s2c1_set_backup | p->item.sensor2_ch1_set;
				p->item.s2c1_set_backup = p->item.s2c1_set_backup & FRONT;
			}
		}	
	}
	else if(ch == 4)
	{
		if(status == 1)
		{													 
			if(p->item.s2c2_set_backup == 0)
			{
				p->item.s2c2_set_backup = p->item.sensor2_ch2_set & LATER;
				p->item.sensor2_ch2_set = p->item.sensor2_ch2_set & FRONT;
			}					
		}
		else if(status == 0)
		{					
			if(p->item.s2c2_set_backup != 0)
			{
				p->item.sensor2_ch2_set = p->item.s2c2_set_backup | p->item.sensor2_ch2_set;
				p->item.s2c2_set_backup = p->item.s2c2_set_backup & FRONT;
			}
		}	
	}
	refresh_sensor_to_flash();
}


void sensor_set_bkp(rt_uint8_t id, rt_uint8_t ch, rt_uint8_t status)
{
	Position p = sensor;
	rt_uint8_t i = 0;	

	while(p != RT_NULL)
	{
		if((++i) == id)
		{
			set_backup(p, ch, status);
			return;
		}

		p = p->next;
	}
}
FINSH_FUNCTION_EXPORT(sensor_set_bkp, e.g: sensor_set_bkp(1,1,1))


void sensor_set(rt_uint8_t id, rt_uint8_t ch, rt_uint16_t min, rt_uint16_t max, rt_uint16_t mid, rt_uint8_t rep_id, rt_uint8_t rep_num, rt_uint16_t set)
{																																																				  
	Position p = sensor;
	rt_uint8_t i = 0;	

	while(p != RT_NULL)
	{
		if((++i) == id)
		{
			if(ch == 1)
			{
				p->item.sensor1_ch1_min = min;
				p->item.sensor1_ch1_max = max;
				p->item.sensor1_ch1_mid = mid;
				p->item.sensor1_ch1_rep = (rep_id<<8) | rep_num;
				p->item.sensor1_ch1_set = set;
			}
			else if(ch == 2)
			{
				p->item.sensor1_ch2_min = min;
				p->item.sensor1_ch2_max = max;
				p->item.sensor1_ch2_mid = mid;
				p->item.sensor1_ch2_rep = (rep_id<<8) | rep_num;
				p->item.sensor1_ch2_set = set;
			}
			else if(ch == 3)
			{
				p->item.sensor2_ch1_min = min;
				p->item.sensor2_ch1_max = max;
				p->item.sensor2_ch1_mid = mid;
				p->item.sensor2_ch1_rep = (rep_id<<8) | rep_num;
				p->item.sensor2_ch1_set = set;
			}
			else if(ch == 4)
			{
				p->item.sensor2_ch2_min = min;
				p->item.sensor2_ch2_max = max;
				p->item.sensor2_ch2_mid = mid;
				p->item.sensor2_ch2_rep = (rep_id<<8) | rep_num;
				p->item.sensor2_ch2_set = set;
			}	
			refresh_sensor_to_flash();
			return;
		}

		p = p->next;
	}
}
FINSH_FUNCTION_EXPORT(sensor_set, e.g: sensor_set(1,1,100,500,50,1,1,255))


void sensor_time(rt_uint8_t id, rt_uint8_t ch, rt_uint16_t start, rt_uint16_t end, rt_uint16_t set)
{
	Position p = sensor;
	rt_uint8_t i = 0;
		

	while(p != RT_NULL)
	{
		if((++i) == id)
		{
			if(ch == 1)
			{
				p->item.time1_start = hex16_to_bcd16(start);
				p->item.time1_end = hex16_to_bcd16(end);
				p->item.time1_set = set;	
			}
			else if(ch == 2)
			{
				p->item.time2_start = hex16_to_bcd16(start);
				p->item.time2_end = hex16_to_bcd16(end);
				p->item.time2_set = set;
			}
			else if(ch == 3)
			{
				p->item.time3_start = hex16_to_bcd16(start);
				p->item.time3_end = hex16_to_bcd16(end);
				p->item.time3_set = set;				
			}
			else if(ch == 4)
			{
				p->item.time4_start = hex16_to_bcd16(start);
				p->item.time4_end = hex16_to_bcd16(end);
				p->item.time4_set = set;
			}
			else if(ch == 5)
			{
				p->item.time5_start = hex16_to_bcd16(start);
				p->item.time5_end = hex16_to_bcd16(end);
				p->item.time5_set = set;
			}			
			else if(ch == 6)
			{
				p->item.time6_start = hex16_to_bcd16(start);
				p->item.time6_end = hex16_to_bcd16(end);
				p->item.time6_set = set;
			}			
			else if(ch == 7)
			{
				p->item.time7_start = hex16_to_bcd16(start);
				p->item.time7_end = hex16_to_bcd16(end);
				p->item.time7_set = set;
			}			
			else if(ch == 8)
			{
				p->item.time8_start = hex16_to_bcd16(start);
				p->item.time8_end = hex16_to_bcd16(end);
				p->item.time8_set = set;
			}
			else if(ch == 9)
			{
				p->item.time9_start = hex16_to_bcd16(start);
				p->item.time9_end = hex16_to_bcd16(end);
				p->item.time9_set = set;
			}			
			else if(ch == 10)
			{
				p->item.time10_start = hex16_to_bcd16(start);
				p->item.time10_end = hex16_to_bcd16(end);
				p->item.time10_set = set;
			}			
			else if(ch == 11)
			{
				p->item.time11_start = hex16_to_bcd16(start);
				p->item.time11_end = hex16_to_bcd16(end);
				p->item.time11_set = set;
			}			
			else if(ch == 12)
			{
				p->item.time12_start = hex16_to_bcd16(start);
				p->item.time12_end = hex16_to_bcd16(end);
				p->item.time12_set = set;
			}			
			else if(ch == 13)
			{
				p->item.time13_start = hex16_to_bcd16(start);
				p->item.time13_end = hex16_to_bcd16(end);
				p->item.time13_set = set;
			}
			else if(ch == 14)
			{
				p->item.time14_start = hex16_to_bcd16(start);
				p->item.time14_end = hex16_to_bcd16(end);
				p->item.time14_set = set;
			}
			else if(ch == 15)
			{
				p->item.time15_start = hex16_to_bcd16(start);
				p->item.time15_end = hex16_to_bcd16(end);
				p->item.time15_set = set;
			}			
			else if(ch == 16)
			{
				p->item.time16_start = hex16_to_bcd16(start);
				p->item.time16_end = hex16_to_bcd16(end);
				p->item.time16_set = set;
			}						
										
			return;
		}

		p = p->next;
	}

	refresh_sensor_to_flash();
}
FINSH_FUNCTION_EXPORT(sensor_time, e.g: sensor_time(1,1,820,1530,255))


void sensor_relate(rt_uint8_t id, rt_uint8_t ch, rt_uint16_t set)
{
	Position p = sensor;
	rt_uint8_t i = 0;
		

	while(p != RT_NULL)
	{
		if((++i) == id)
		{
			if(ch == 1)
			{
				p->item.k1_relate = set;	
			}
			else if(ch == 2)
			{
				p->item.k2_relate = set;
			}
			else if(ch == 3)
			{
				p->item.k3_relate = set;			
			}
			else if(ch == 4)
			{
				p->item.k4_relate = set;
			}	
			refresh_sensor_to_flash();
			return;
		}

		p = p->next;
	}
}
FINSH_FUNCTION_EXPORT(sensor_relate, e.g: sensor_relate(1,1,255))
#endif

#endif
