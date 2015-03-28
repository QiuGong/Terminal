#include "select_fun.h"
#include "flash.h"
#include "strLib.h"


static SelectFun fun;

SelectFun get_select_fun(void)
{
	return fun;
}


static void refresh_select_fun_to_flash(void)
{
	rt_uint8_t len = 0;
	rt_uint8_t *buf = RT_NULL;


	buf = rt_malloc(SELECT_FUN_LEN);
	if(buf == RT_NULL)
	{
		rt_kprintf("refresh_select_fun_to_flash() rt_malloc failed.\n"); 
		return;
	}


	/* 填写数据 */
	buf[0] = '$';	len += 1;
	len += positive(&(fun.warn_value), buf+len, 1);
	len += positive(&(fun.warn_rep), buf+len, 1);
	len += positive(&(fun.warn_timer), buf+len, 1);
	len += positive(&(fun.warn_relate), buf+len, 1);
	len += positive(&(fun.warn_time_out), buf+len, 1);
	len += positive(&(fun.warn_power_drop), buf+len, 1);


#if ( FLASH_EN > 0 )
	Flash_Earse_Write(F_SELECT_FUN, 0, buf, len);
#endif


	rt_free(buf);
	RT_DEBUG_LOG(DEBUG_RS485, ("refresh select fun to flash.\n"));
}


void read_select_fun_from_flash(void)
{
	rt_uint8_t len = 0;
	rt_uint8_t *buf = RT_NULL;

	
	buf = rt_malloc(SELECT_FUN_LEN);
	if(buf == RT_NULL)
	{
		rt_kprintf("read_sensor_flash() rt_malloc failed.\n");
		return;
	}	


	/* 读取数据 */
#if ( FLASH_EN > 0 )
	Flash_Read(F_SELECT_FUN, 0, buf, SELECT_FUN_LEN);
#endif
	if(buf[len] == '$')
	{
		len += 1;

		len += positive(buf+len, &(fun.warn_value), 1);
		len += positive(buf+len, &(fun.warn_rep), 1);
		len += positive(buf+len, &(fun.warn_timer), 1);
		len += positive(buf+len, &(fun.warn_relate), 1);
		len += positive(buf+len, &(fun.warn_time_out), 1);
		len += positive(buf+len, &(fun.warn_power_drop), 1);
	}


	rt_free(buf);
}


#ifdef RT_USING_FINSH
#include <finsh.h>
static void list_define(void)
{
	RT_DEBUG_LOG(RS485_EN, 		("RS485_EN \n"));
	RT_DEBUG_LOG(DISPLAY_EN, 	("DISPLAY_EN \n"));
	RT_DEBUG_LOG(DS1302_EN, 	("DS1302_EN \n"));
	RT_DEBUG_LOG(FLASH_EN, 		("FLASH_EN \n"));
	RT_DEBUG_LOG(WARN_EN, 		("WARN_EN \n"));
	RT_DEBUG_LOG(WARN_LED, 		("WARN_LED \n"));
	RT_DEBUG_LOG(WARN_BELL, 	("WARN_BELL \n"));	
	RT_DEBUG_LOG(WARN_VALUE, 	("WARN_VALUE \n"));
	RT_DEBUG_LOG(WARN_REP, 		("WARN_REP \n"));
	RT_DEBUG_LOG(WARN_TIMER, 	("WARN_TIMER \n"));
	RT_DEBUG_LOG(WARN_RELATE, 	("WARN_RELATE \n"));   
	RT_DEBUG_LOG(WARN_TIME_OUT, ("WARN_TIME_OUT \n")); 
	RT_DEBUG_LOG(WARN_POWER_DROP, ("WARN_POWER_DROP \n"));  
}

FINSH_FUNCTION_EXPORT(list_define, e.g: list_define())


static void list_select_fun(void)
{
	RT_DEBUG_LOG(RS485_EN, ("warn value:"));
	if(fun.warn_value == 1)
	{
		RT_DEBUG_LOG(RS485_EN, ("on \n"));
	}
	else
	{
		RT_DEBUG_LOG(RS485_EN, ("off \n"));
	}

	RT_DEBUG_LOG(RS485_EN, ("warn rep:"));
	if(fun.warn_rep == 1)
	{
		RT_DEBUG_LOG(RS485_EN, ("on \n"));
	}
	else
	{
		RT_DEBUG_LOG(RS485_EN, ("off \n"));
	}

	RT_DEBUG_LOG(RS485_EN, ("warn timer:"));
	if(fun.warn_timer == 1)
	{
		RT_DEBUG_LOG(RS485_EN, ("on \n"));
	}
	else
	{
		RT_DEBUG_LOG(RS485_EN, ("off \n"));
	}

	RT_DEBUG_LOG(RS485_EN, ("warn relate:"));
	if(fun.warn_relate == 1)
	{
		RT_DEBUG_LOG(RS485_EN, ("on \n"));
	}
	else
	{
		RT_DEBUG_LOG(RS485_EN, ("off \n"));
	}

	RT_DEBUG_LOG(RS485_EN, ("warn time out:"));
	if(fun.warn_time_out == 1)
	{
		RT_DEBUG_LOG(RS485_EN, ("on \n"));
	}
	else
	{
		RT_DEBUG_LOG(RS485_EN, ("off \n"));
	}

	RT_DEBUG_LOG(RS485_EN, ("warn power drop:"));
	if(fun.warn_power_drop == 1)
	{
		RT_DEBUG_LOG(RS485_EN, ("on \n"));
	}
	else
	{
		RT_DEBUG_LOG(RS485_EN, ("off \n"));
	}
}

FINSH_FUNCTION_EXPORT(list_select_fun, e.g: list_select_fun())


static void select_fun(rt_uint8_t select, rt_uint8_t status)
{
	if(select == 1)
	{
		if(status == 0)
		{
			fun.warn_value = 0;
		}
		else if(status == 1)
		{
			fun.warn_value = 1;
		}
	}
	else if(select == 2)
	{
		if(status == 0)
		{
			fun.warn_rep = 0;
		}
		else if(status == 1)
		{
			fun.warn_rep = 1;
		}
	}
	else if(select == 3)
	{
		if(status == 0)
		{
			fun.warn_timer = 0;
		}
		else if(status == 1)
		{
			fun.warn_timer = 1;
		}
	}
	else if(select == 4)
	{
		if(status == 0)
		{
			fun.warn_relate = 0;
		}
		else if(status == 1)
		{
			fun.warn_relate = 1;
		}
	}
	else if(select == 5)
	{
		if(status == 0)
		{
			fun.warn_time_out = 0;
		}
		else if(status == 1)
		{
			fun.warn_time_out = 1;
		}
	}
	else if(select == 6)
	{
		if(status == 0)
		{
			fun.warn_power_drop = 0;
		}
		else if(status == 1)
		{
			fun.warn_power_drop = 1;
		}
	}


	refresh_select_fun_to_flash();
}

FINSH_FUNCTION_EXPORT(select_fun, e.g: select_fun(1,0))
#endif
