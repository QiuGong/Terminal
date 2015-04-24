#include "flash.h"
#include "stm32f10x_flash.h"
#include <string.h>


#if ( FLASH_EN > 0 )


#define FLASH_PAGE_SIZE    ((rt_uint16_t)0x800)			//ҳ��С
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;


void _flash_erase(rt_uint32_t START_ADDR, rt_uint32_t END_ADDR)  //��
{
  	rt_uint32_t EraseCounter = 0x00;   
  	rt_uint32_t NbrOfPage 	= 0x00;			   //Ҫ������ҳ������
  	FLASH_Status FLASHStatus = FLASH_COMPLETE; //FLASH����״̬

   	/* Unlock the Flash Bank1 Program Erase controller */
  	FLASH_UnlockBank1();

  	/* Define the number of page to be erased */
  	NbrOfPage = (END_ADDR - START_ADDR) / FLASH_PAGE_SIZE;

 	/* Clear All pending flags */
 	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	

    /* Erase the FLASH pages */
    for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
    {
      FLASHStatus = FLASH_ErasePage(START_ADDR + (FLASH_PAGE_SIZE * EraseCounter));
    }

  	FLASH_LockBank1();
}


void _flash_write(rt_uint16_t *pdata, rt_uint32_t START_ADDR, rt_uint16_t Len) //д
{
	rt_uint32_t Address = 0x00;  

  	FLASH_Status FLASHStatus = FLASH_COMPLETE; //FLASH����״̬

  	FLASH_UnlockBank1();

  	Address = START_ADDR;
	Len	= Len+Len;

  	while((Address < START_ADDR+Len) && (FLASHStatus == FLASH_COMPLETE))
  	{
    	FLASHStatus = FLASH_ProgramHalfWord(Address, *pdata);
		pdata++;
    	Address = Address + 2;
  	}

  	FLASH_LockBank1();
}


rt_uint8_t _flash_write_check(rt_uint16_t *pdata, rt_uint32_t START_ADDR, rt_uint16_t Len)
{
	if(memcmp((rt_uint8_t *)pdata, (rt_uint8_t *)START_ADDR, Len)==0) 
		return RT_TRUE;
	else
		return RT_FALSE;		
}


#endif
