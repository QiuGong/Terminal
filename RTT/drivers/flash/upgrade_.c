#include <includes.h>
#include <config.h>


/**************************************************************/
//（0x08040000~0x08070000）升级程序，192K
#define  UPGRADE_START_ADDR		((INT32U)0x08040000) 
/**************************************************************/
INT8U Write_FLASH_Upgrade(INT8U Page, INT8U *p)
{
	_ASSERT_(p != SAFE_ADD);
	
	if(Page%4==0)
	{
		_flash_erase(UPGRADE_START_ADDR+(Page/4)*0x800, UPGRADE_START_ADDR+(Page/4)*0x800+0x800);
		_flash_write((INT16U*)p, UPGRADE_START_ADDR+(Page/4)*0x800, 256);
		if(_flash_write_check((INT16U*)p, UPGRADE_START_ADDR+(Page/4)*0x800, 256)) return TRUE;
	}
	else
	{
		_flash_write((INT16U*)p, UPGRADE_START_ADDR+(Page/4)*0x800+(Page%4)*0x200, 256);
		if(_flash_write_check((INT16U*)p, UPGRADE_START_ADDR+(Page/4)*0x800+(Page%4)*0x200, 256)) return TRUE;
	}

	return FALSE;
}


/**************************************************************/
//（0x08070000~0x08070800）升级判断标志
#define  UPGRADE_MARK_START_ADDR	((INT32U)0x08070000)
#define  UPGRADE_MARK_END_ADDR		((INT32U)0x08070800)
/**************************************************************/
void Write_FLASH_Upgrade_Mark(INT16U Num,INT16U *p)
{
	_flash_erase(UPGRADE_MARK_START_ADDR,UPGRADE_MARK_END_ADDR);
	_flash_write((INT16U*)p,UPGRADE_MARK_START_ADDR,Num);	
}

