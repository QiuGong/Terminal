#include "flash.h"
#include "strLib.h"
#include "rtc.h"
#include "select_fun.h"


#if ( FLASH_EN > 0 )
#define BACKUP_BASE_ADD 	F_SELECT_FUN

static void refresh_backup_to_flash(void)
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
#if ( DS1302_EN > 0 )
	get_time(buf + 1);
#endif		
	len += 6;
	len += positive((rt_uint8_t *)(BACKUP_BASE_ADD), buf+len, BACKUP_LEN - len);  
	Flash_Earse_Write(F_BACKUP, 0, buf, len);


	rt_free(buf);
	RT_DEBUG_LOG(DEBUG_RS485, ("refresh backup to flash.\n"));
}


static void read_backup_from_flash(void)
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
	Flash_Read(F_BACKUP, 0, buf, BACKUP_LEN);
	if(buf[len] == '$')
	{
		len += 1 + 6;// $ + time  
		Flash_Earse_Write(BACKUP_BASE_ADD, 0, buf + len, BACKUP_LEN - len);
	}																			 


	rt_free(buf);
	RT_DEBUG_LOG(DEBUG_RS485, ("read backup from flash.\n"));
}


static void get_backup_status(void)
{	
	rt_uint8_t len = 0;
	rt_uint8_t *buf = RT_NULL;

	
	buf = rt_malloc(BACKUP_LEN);
	if(buf == RT_NULL)
	{
		rt_kprintf("get_backup_status() rt_malloc failed.\n");
	}	


	/* 读取数据 */
	Flash_Read(F_BACKUP, 0, buf, BACKUP_LEN);
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
static void backup_factory(void)
{
	refresh_backup_to_flash();
}
FINSH_FUNCTION_EXPORT(backup_factory, e.g: backup_factory())


static void factory(void)
{
	read_backup_from_flash();
	read_select_fun_from_flash();
	read_sensor_from_flash();
}
FINSH_FUNCTION_EXPORT(factory, e.g: factory())


static void get_factory(void)
{
	get_backup_status();
}
FINSH_FUNCTION_EXPORT(get_factory, e.g: get_factory())
#endif

#endif
