#include "flash.h"
#include "rs485.h"
#include <string.h>

#if ( FLASH_EN > 0 )

#define 	MAX_LEN		0x1000
				  
void Flash_Earse_Write(enum Flash_Data type, rt_uint32_t ops, rt_uint8_t *b, rt_uint16_t len)
{
	rt_uint8_t *p = RT_NULL;


	RT_ASSERT(len <= MAX_LEN);
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


	/* ¸´ÖÆ */
	p = rt_malloc(MAX_LEN);
	RT_ASSERT(p != RT_NULL);
	memcpy(p, (rt_uint8_t *)((type+ops) / MAX_LEN * MAX_LEN), MAX_LEN);

	/* ÐÞ¸Ä */
	memcpy(p+(type-(type/MAX_LEN * MAX_LEN)+ (ops%MAX_LEN)), b, len);

	/* Õ³Ìù */
	_flash_erase(((type+ops) / MAX_LEN * MAX_LEN), ((type+ops) / MAX_LEN * MAX_LEN) + MAX_LEN);
	_flash_write((rt_uint16_t*)p, ((type+ops) / MAX_LEN * MAX_LEN), MAX_LEN / 2);
	rt_free(p);	
}


void Flash_Read(enum Flash_Data type, rt_uint32_t ops, rt_uint8_t *b, rt_uint16_t len)
{	
	RT_ASSERT(len <= MAX_LEN);
	
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


