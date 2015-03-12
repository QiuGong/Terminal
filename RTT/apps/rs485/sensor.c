#include <string.h>
#include <rs485.h>
#include "flash.h"
#include "strLib.h"
#include "rtc.h"
#include "select_fun.h"
#include "util.h"

#if ( RS485_EN > 0 )

List sensor =  RT_NULL;

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


rt_uint8_t	delete_sensor(rt_uint8_t *b)
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
	rt_uint8_t len = 0;
	rt_uint8_t *buf = RT_NULL;
	Position p = sensor;


	buf = rt_malloc(RS485_LEN);
	if(buf == RT_NULL)
	{
		rt_kprintf("refresh_sensor_to_flash() rt_malloc failed.\n"); 
		return;
	}


	/* 填写数据 */
	buf[0] = '$';	len += 2;
	while(p != RT_NULL)
	{
		len += positive(p->item.id, buf+len, ID_LEN);

		// 传感器最大最小值
		len += negative((rt_uint8_t *)&(p->item.sensor1_ch1_min), buf+len, SENSOR_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.sensor1_ch1_max), buf+len, SENSOR_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.sensor1_ch1_set), buf+len, SENSOR_SET_PER_LEN);

		len += negative((rt_uint8_t *)&(p->item.sensor1_ch2_min), buf+len, SENSOR_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.sensor1_ch2_max), buf+len, SENSOR_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.sensor1_ch2_set), buf+len, SENSOR_SET_PER_LEN);

		len += negative((rt_uint8_t *)&(p->item.sensor2_ch1_min), buf+len, SENSOR_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.sensor2_ch1_max), buf+len, SENSOR_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.sensor2_ch1_set), buf+len, SENSOR_SET_PER_LEN);

		len += negative((rt_uint8_t *)&(p->item.sensor2_ch2_min), buf+len, SENSOR_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.sensor2_ch2_max), buf+len, SENSOR_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.sensor2_ch2_set), buf+len, SENSOR_SET_PER_LEN);

		// 时间开始结束
		len += negative((rt_uint8_t *)&(p->item.time1_start), 	  buf+len, TIME_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time1_end), 	  buf+len, TIME_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time1_set), 	  buf+len, TIME_SET_PER_LEN);

		len += negative((rt_uint8_t *)&(p->item.time2_start), 	  buf+len, TIME_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time2_end), 	  buf+len, TIME_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time2_set), 	  buf+len, TIME_SET_PER_LEN);

		len += negative((rt_uint8_t *)&(p->item.time3_start), 	  buf+len, TIME_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time3_end), 	  buf+len, TIME_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time3_set), 	  buf+len, TIME_SET_PER_LEN);

		len += negative((rt_uint8_t *)&(p->item.time4_start), 	  buf+len, TIME_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time4_end), 	  buf+len, TIME_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.time4_set), 	  buf+len, TIME_SET_PER_LEN);

		len += negative((rt_uint8_t *)&(p->item.k1_relate), 	  buf+len, K_RELATE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.k2_relate), 	  buf+len, K_RELATE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.k3_relate), 	  buf+len, K_RELATE_PER_LEN);
		len += negative((rt_uint8_t *)&(p->item.k4_relate), 	  buf+len, K_RELATE_PER_LEN);

		p = p->next;
	}
	buf[1] = len / PER_SENSOR_FLASH_LEN;

#if ( FLASH_EN > 0 )
	Flash_Earse_Write(F_RS485, 0, buf, len);
#endif

	rt_free(buf);
	RT_DEBUG_LOG(DEBUG_RS485, ("refresh sensor to flash.\n"));
}


void read_sensor_from_flash(void)
{
	rt_uint8_t len = 0;
	rt_uint8_t *buf = RT_NULL;

	
	buf = rt_malloc(RS485_LEN);
	if(buf == RT_NULL)
	{
		rt_kprintf("read_sensor_flash() rt_malloc failed.\n");
		return;
	}	


	/* 读取数据 */
#if ( FLASH_EN > 0 )
	Flash_Read(F_RS485, 0, buf, RS485_LEN);
#endif
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

			// 传感器最大最小值
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor1_ch1_min), SENSOR_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor1_ch1_max), SENSOR_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor1_ch1_set), SENSOR_SET_PER_LEN);
	
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor1_ch2_min), SENSOR_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor1_ch2_max), SENSOR_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor1_ch2_set), SENSOR_SET_PER_LEN);
	
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor2_ch1_min), SENSOR_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor2_ch1_max), SENSOR_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor2_ch1_set), SENSOR_SET_PER_LEN);
	
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor2_ch2_min), SENSOR_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor2_ch2_max), SENSOR_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.sensor2_ch2_set), SENSOR_SET_PER_LEN);
	
			// 时间开始,结束,设置
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time1_start), 	TIME_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time1_end), 	TIME_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time1_set), 	TIME_SET_PER_LEN);
	
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time2_start), 	TIME_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time2_end), 	TIME_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time2_set), 	TIME_SET_PER_LEN);
	
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time3_start), 	TIME_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time3_end), 	TIME_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time3_set), 	TIME_SET_PER_LEN);
	
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time4_start), 	TIME_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time4_end), 	TIME_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.time4_set), 	TIME_SET_PER_LEN);

			len += negative(buf+len, (rt_uint8_t *)&(p->item.k1_relate), 	K_RELATE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.k2_relate), 	K_RELATE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.k3_relate), 	K_RELATE_PER_LEN);
			len += negative(buf+len, (rt_uint8_t *)&(p->item.k4_relate), 	K_RELATE_PER_LEN);
		}	
	}


	rt_free(buf);
}


#define BACKUP_BASE_ADD 	F_SELECT_FUN
void refresh_backup_to_flash(void)
{
	rt_uint8_t len = 0;
	rt_uint8_t *buf = RT_NULL;

	
	buf = rt_malloc(BACKUP_LEN);
	if(buf == RT_NULL)
	{
		rt_kprintf("refresh_backup_to_flash() rt_malloc failed.\n"); 
		return;
	}


	/*  */
	buf[0] = '$';	len += 1;
	get_time(buf + 1);	len += 6;
	len += positive((rt_uint8_t *)(BACKUP_BASE_ADD), buf+len, BACKUP_LEN - len);  


#if ( FLASH_EN > 0 )
	Flash_Earse_Write(F_BACKUP, 0, buf, len);
#endif


	rt_free(buf);
	RT_DEBUG_LOG(DEBUG_RS485, ("refresh backup to flash.\n"));
}


void read_backup_from_flash(void)
{
	rt_uint8_t len = 0;
	rt_uint8_t *buf = RT_NULL;

	
	buf = rt_malloc(BACKUP_LEN);
	if(buf == RT_NULL)
	{
		rt_kprintf("read_backup_from_flash() rt_malloc failed.\n");
		return;
	}	


	/* 读取数据 */
#if ( FLASH_EN > 0 )
	Flash_Read(F_BACKUP, 0, buf, BACKUP_LEN);
#endif
	if(buf[len] == '$')
	{
		len += 1 + 6;// $ + time  
		Flash_Earse_Write(BACKUP_BASE_ADD, 0, buf + len, BACKUP_LEN - len);
	}																			 


	rt_free(buf);
	RT_DEBUG_LOG(DEBUG_RS485, ("read backup from flash.\n"));
}


void get_backup_status(void)
{	
	rt_uint8_t len = 0;
	rt_uint8_t *buf = RT_NULL;

	
	buf = rt_malloc(BACKUP_LEN);
	if(buf == RT_NULL)
	{
		rt_kprintf("get_backup_status() rt_malloc failed.\n");
	}	


	/* 读取数据 */
#if ( FLASH_EN > 0 )
	Flash_Read(F_BACKUP, 0, buf, BACKUP_LEN);
#endif
	if(buf[len] == '$')
	{
		RT_DEBUG_LOG(DEBUG_RS485, ("backup time:%02X %02X %02X %02X %02X %02X\n", buf[len + 1], buf[len + 2], buf[len + 3], buf[len + 4], buf[len + 5], buf[len + 6]));			
	}
	else
	{
	 	RT_DEBUG_LOG(DEBUG_RS485, ("haven't backup.\n"));	
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
		RT_DEBUG_LOG(DEBUG_RS485, ("-----------------------%d-------------------\n",(++i)));
		RT_DEBUG_LOG(DEBUG_RS485, ("Id:%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n", p->item.id[0], p->item.id[1], p->item.id[2], p->item.id[3], p->item.id[4], p->item.id[5], p->item.id[6], p->item.id[7], p->item.id[8], p->item.id[9], p->item.id[10], p->item.id[11]));
		RT_DEBUG_LOG(DEBUG_RS485, ("-------------------------------------------\n"));
		RT_DEBUG_LOG(DEBUG_RS485, ("value set:%04X \n", p->item.value_set));
		RT_DEBUG_LOG(DEBUG_RS485, ("timer set:%04X \n", p->item.timer_set));
		RT_DEBUG_LOG(DEBUG_RS485, ("last rec time:%d \n", p->item.last_rec_time));
		RT_DEBUG_LOG(DEBUG_RS485, ("-------------------------------------------\n"));
		RT_DEBUG_LOG(DEBUG_RS485, ("s1_c1-> val:%d, min:%d, max:%d, set:%08X, 电流:%d, 最小:%d, 最大:%d \n", p->item.sensor1_ch1, p->item.sensor1_ch1_min, p->item.sensor1_ch1_max, p->item.sensor1_ch1_set, p->item.sensor1_ch1, 					p->item.sensor1_ch1_min, 					p->item.sensor1_ch1_max));
		RT_DEBUG_LOG(DEBUG_RS485, ("s1_c2-> val:%d, min:%d, max:%d, set:%08X, 溶氧:%d, 最小:%d, 最大:%d \n", p->item.sensor1_ch2, p->item.sensor1_ch2_min, p->item.sensor1_ch2_max, p->item.sensor1_ch2_set, formula_do_16(p->item.sensor1_ch2), 	formula_do_16(p->item.sensor1_ch2_min), 	formula_do_16(p->item.sensor1_ch2_max)));
		RT_DEBUG_LOG(DEBUG_RS485, ("s2_c1-> val:%d, min:%d, max:%d, set:%08X, 酸碱:%d, 最小:%d, 最大:%d \n", p->item.sensor2_ch1, p->item.sensor2_ch1_min, p->item.sensor2_ch1_max, p->item.sensor2_ch1_set, formula_ph_16(p->item.sensor2_ch1), 	formula_ph_16(p->item.sensor2_ch1_min), 	formula_ph_16(p->item.sensor2_ch1_max)));
		RT_DEBUG_LOG(DEBUG_RS485, ("s2_c2-> val:%d, min:%d, max:%d, set:%08X, 温度:%d, 最小:%d, 最大:%d \n", p->item.sensor2_ch2, p->item.sensor2_ch2_min, p->item.sensor2_ch2_max, p->item.sensor2_ch2_set, formula_temp_16(p->item.sensor2_ch2), 	formula_temp_16(p->item.sensor2_ch2_min), 	formula_temp_16(p->item.sensor2_ch2_max)));	
		RT_DEBUG_LOG(DEBUG_RS485, ("-------------------------------------------\n"));
		RT_DEBUG_LOG(DEBUG_RS485, ("t1-> start:%04X, end:%04X, set:%08X.\n", p->item.time1_start, p->item.time1_end, p->item.time1_set));
		RT_DEBUG_LOG(DEBUG_RS485, ("t2-> start:%04X, end:%04X, set:%08X.\n", p->item.time2_start, p->item.time2_end, p->item.time2_set));
		RT_DEBUG_LOG(DEBUG_RS485, ("t3-> start:%04X, end:%04X, set:%08X.\n", p->item.time3_start, p->item.time3_end, p->item.time3_set));
		RT_DEBUG_LOG(DEBUG_RS485, ("t4-> start:%04X, end:%04X, set:%08X.\n", p->item.time4_start, p->item.time4_end, p->item.time4_set));		
		RT_DEBUG_LOG(DEBUG_RS485, ("-------------------------------------------\n"));
		RT_DEBUG_LOG(DEBUG_RS485, ("K:%04X \n", p->item.k));
		RT_DEBUG_LOG(DEBUG_RS485, ("k1:%04X, k2:%04X, k3:%04X, k4:%04X.\n", p->item.k1_relate, p->item.k2_relate, p->item.k3_relate, p->item.k4_relate));
		RT_DEBUG_LOG(DEBUG_RS485, ("||||||||||||||||||||||||||||||||||||||||||||\n\n\n\n"));

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
			x01_request(p->item.id, (enum X01_DEV)dev, (enum X01_CONTROL)control);
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


void sensor_set(rt_uint8_t id, rt_uint8_t ch, rt_uint16_t min, rt_uint16_t max, rt_uint16_t set)
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
				p->item.sensor1_ch1_set = set;
			}
			else if(ch == 2)
			{
				p->item.sensor1_ch2_min = min;
				p->item.sensor1_ch2_max = max;
				p->item.sensor1_ch2_set = set;
			}
			else if(ch == 3)
			{
				p->item.sensor2_ch1_min = min;
				p->item.sensor2_ch1_max = max;
				p->item.sensor2_ch1_set = set;
			}
			else if(ch == 4)
			{
				p->item.sensor2_ch2_min = min;
				p->item.sensor2_ch2_max = max;
				p->item.sensor2_ch2_set = set;
			}	
			refresh_sensor_to_flash();
			return;
		}

		p = p->next;
	}
}
FINSH_FUNCTION_EXPORT(sensor_set, e.g: sensor_set(1,1,100,500,255))


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


void backup_factory(void)
{
	refresh_backup_to_flash();
}
FINSH_FUNCTION_EXPORT(backup_factory, e.g: backup_factory())


void factory(void)
{
	read_backup_from_flash();
	read_select_fun_from_flash();
	read_sensor_from_flash();

}
FINSH_FUNCTION_EXPORT(factory, e.g: factory())


void get_factory(void)
{
	get_backup_status();
}
FINSH_FUNCTION_EXPORT(get_factory, e.g: get_factory())
#endif

#endif
