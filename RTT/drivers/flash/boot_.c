#include <includes.h>
#include <config.h>


/**************************************************************/
//£¨0x0807F000~0x0807F800£©Ð´ Boot
#define  BOOT_START_ADDR	((INT32U)0x0807F000)
#define  BOOT_END_ADDR		((INT32U)0x0807F800)
/**************************************************************/
INT8U Write_FLASH_Boot(INT16U Num, INT16U *p)
{
	_flash_erase(BOOT_START_ADDR, BOOT_END_ADDR);
	_flash_write((INT16U*)p, BOOT_START_ADDR, Num);

	if(_flash_write_check((INT16U*)p, BOOT_START_ADDR, Num)) 
	{
		return TRUE;  
	}
	else
	{
		return FALSE;
	}
}

