#include "flash.h"
#include "rs485.h"
#include <string.h>

#if ( FLASH_EN > 0 )

/**************************************************************/
//（0x08030000~0x08040000）变量,盲区 64K
/**************************************************************/				  
void Flash_Earse_Write(enum Flash_Data type, rt_uint32_t ops, rt_uint8_t *b, rt_uint16_t len)
{
	rt_uint8_t *p = RT_NULL;

	
	if(type == F_SELECT_FUN)
	{
		
	}
	else if(type == F_RS485)
	{
		RT_ASSERT(ops+len <= RS485_LEN);
	}
	else if(type == F_BACKUP)
	{
		RT_ASSERT(ops+len <= BACKUP_LEN);
	}	
	else
	{
		RT_ASSERT(0);
	}


	/* 复制 */
	p = rt_malloc(2048);
	RT_ASSERT(p != RT_NULL);
	memcpy(p, (rt_uint8_t *)((type+ops)/0x800*0x800), 0x800);

	/* 修改 */
	memcpy(p+(type-(type/0x800*0x800)+ops%0x800), b, len);

	/* 粘贴 */
	_flash_erase( ((type+ops)/0x800*0x800), ((type+ops)/0x800*0x800)+0x800 );
	_flash_write((rt_uint16_t*)p, ((type+ops)/0x800*0x800), 0x400);
	rt_free(p);	
}


void Flash_Read(enum Flash_Data type, rt_uint32_t ops, rt_uint8_t *b, rt_uint16_t len)
{	
	if(type == F_SELECT_FUN)
	{
		RT_ASSERT(ops+len <= SELECT_FUN_LEN);
	}
	else if(type == F_RS485)
	{
		RT_ASSERT(ops+len <= RS485_LEN);
	}
	else if(type == F_BACKUP)
	{
		RT_ASSERT(ops+len <= BACKUP_LEN);
	}	
	else
	{
		RT_ASSERT(0);
	}


	memcpy(b, (rt_uint8_t *)(type + ops), len);	
}


void Flash_Erase_Para(void)
{
	_flash_erase(0x08030000, 0x08040000);	
}

#endif


