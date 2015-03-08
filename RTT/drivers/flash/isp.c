#include <stm32f10x_flash.h>
#include <config.h>

#if ( UP_DATA_EN == 1 )
#ifdef STM32F103
/*******************************************/
//（0x08040000~0x08070000）升级程序，192K
//（0x08070000~0x08070800）升级判断标志
//（0x0807F000~0x08080000）ISP程序
/*******************************************/
#define ISP_MARK 0x08070000
#define ISP_CODE 0x08040000

#else 
/*******************************************/
//（0x08020000~0x08038000）升级程序，192K
//（0x08038000~0x08038800）升级判断标志
//（0x0803F000~0x08040000）ISP程序
/*******************************************/
#define ISP_MARK 0x08038000
#define ISP_CODE 0x08020000

#endif


#define EraseTimeout          ((unsigned long)0x000B0000)
#define CR_PER_Set            ((unsigned long)0x00000002)
#define CR_STRT_Set           ((unsigned long)0x00000040)
#define CR_PER_Reset          ((unsigned long)0x00001FFD)
#define ProgramTimeout        ((unsigned long)0x00002000)
#define CR_PG_Set             ((unsigned long)0x00000001)
#define CR_PG_Reset           ((unsigned long)0x00001FFE)
 
static FLASH_Status FLASH_GetBank1StatusNew(void)
{
  FLASH_Status flashstatus = FLASH_COMPLETE;
  
  if((FLASH->SR & FLASH_FLAG_BANK1_BSY) == FLASH_FLAG_BSY) 
  {
    flashstatus = FLASH_BUSY;
  }
  else 
  {  
    if((FLASH->SR & FLASH_FLAG_BANK1_PGERR) != 0)
    { 
      flashstatus = FLASH_ERROR_PG;
    }
    else 
    {
      if((FLASH->SR & FLASH_FLAG_BANK1_WRPRTERR) != 0 )
      {
        flashstatus = FLASH_ERROR_WRP;
      }
      else
      {
        flashstatus = FLASH_COMPLETE;
      }
    }
  }
  /* Return the Flash Status */
  return flashstatus;
}

static FLASH_Status FLASH_WaitForLastOperationNew(unsigned long Timeout)
{ 
  FLASH_Status status = FLASH_COMPLETE;
   
  /* Check for the Flash Status */
  status = FLASH_GetBank1StatusNew();
  /* Wait for a Flash operation to complete or a TIMEOUT to occur */
  while((status == FLASH_BUSY) && (Timeout != 0x00))
  {
    status = FLASH_GetBank1StatusNew();
    Timeout--;
  }
  if(Timeout == 0x00 )
  {
    status = FLASH_TIMEOUT;
  }
  /* Return the operation status */
  return status;
}

static void FLASH_ClearFlagNew(unsigned long FLASH_FLAG)
{
#ifdef STM32F10X_XL
  /* Check the parameters */
  assert_param(IS_FLASH_CLEAR_FLAG(FLASH_FLAG)) ;

  if((FLASH_FLAG & 0x80000000) != 0x0)
  {
    /* Clear the flags */
    FLASH->SR2 = FLASH_FLAG;
  }
  else
  {
    /* Clear the flags */
    FLASH->SR = FLASH_FLAG;
  }  

#else
  /* Check the parameters */
  assert_param(IS_FLASH_CLEAR_FLAG(FLASH_FLAG)) ;
  
  /* Clear the flags */
  FLASH->SR = FLASH_FLAG;
#endif /* STM32F10X_XL */
}

static FLASH_Status FLASH_ErasePageNew(unsigned long Page_Address)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* Check the parameters */
  assert_param(IS_FLASH_ADDRESS(Page_Address));

#ifdef STM32F10X_XL
  if(Page_Address < FLASH_BANK1_END_ADDRESS)  
  {
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastBank1Operation(EraseTimeout);
    if(status == FLASH_COMPLETE)
    { 
      /* if the previous operation is completed, proceed to erase the page */
      FLASH->CR|= CR_PER_Set;
      FLASH->AR = Page_Address; 
      FLASH->CR|= CR_STRT_Set;
    
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastBank1Operation(EraseTimeout);
      if(status != FLASH_TIMEOUT)
      {
        /* if the erase operation is completed, disable the PER Bit */
        FLASH->CR &= CR_PER_Reset;
      }
    }
  }
  else
  {
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastBank2Operation(EraseTimeout);
    if(status == FLASH_COMPLETE)
    { 
      /* if the previous operation is completed, proceed to erase the page */
      FLASH->CR2|= CR_PER_Set;
      FLASH->AR2 = Page_Address; 
      FLASH->CR2|= CR_STRT_Set;
    
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastBank2Operation(EraseTimeout);
      if(status != FLASH_TIMEOUT)
      {
        /* if the erase operation is completed, disable the PER Bit */
        FLASH->CR2 &= CR_PER_Reset;
      }
    }
  }
#else
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperationNew(EraseTimeout);
  
  if(status == FLASH_COMPLETE)
  { 
    /* if the previous operation is completed, proceed to erase the page */
    FLASH->CR|= CR_PER_Set;
    FLASH->AR = Page_Address; 
    FLASH->CR|= CR_STRT_Set;
    
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperationNew(EraseTimeout);
    if(status != FLASH_TIMEOUT)
    {
      /* if the erase operation is completed, disable the PER Bit */
      FLASH->CR &= CR_PER_Reset;
    }
  }
#endif /* STM32F10X_XL */

  /* Return the Erase Status */
  return status;
}

static FLASH_Status FLASH_ProgramHalfWordNew(unsigned long Address, unsigned short Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* Check the parameters */
  assert_param(IS_FLASH_ADDRESS(Address));

#ifdef STM32F10X_XL
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperationNew(ProgramTimeout);
  
  if(Address < FLASH_BANK1_END_ADDRESS)
  {
    if(status == FLASH_COMPLETE)
    {
      /* if the previous operation is completed, proceed to program the new data */
      FLASH->CR |= CR_PG_Set;
  
      *(__IO uint16_t*)Address = Data;
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastBank1Operation(ProgramTimeout);
      if(status != FLASH_TIMEOUT)
      {
        /* if the program operation is completed, disable the PG Bit */
        FLASH->CR &= CR_PG_Reset;
      }
    }
  }
  else
  {
    if(status == FLASH_COMPLETE)
    {
      /* if the previous operation is completed, proceed to program the new data */
      FLASH->CR2 |= CR_PG_Set;
  
      *(__IO uint16_t*)Address = Data;
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastBank2Operation(ProgramTimeout);
      if(status != FLASH_TIMEOUT)
      {
        /* if the program operation is completed, disable the PG Bit */
        FLASH->CR2 &= CR_PG_Reset;
      }
    }
  }
#else
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperationNew(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to program the new data */
    FLASH->CR |= CR_PG_Set;
  
    *(__IO uint16_t*)Address = Data;
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperationNew(ProgramTimeout);
    if(status != FLASH_TIMEOUT)
    {
      /* if the program operation is completed, disable the PG Bit */
      FLASH->CR &= CR_PG_Reset;
    }
  } 
#endif  /* STM32F10X_XL */
  
  /* Return the Program Status */
  return status;
}

static void Flash_Erase(unsigned long START_ADDR,unsigned long END_ADDR)  //擦
{
  	unsigned long EraseCounter = 0x00;   
  	unsigned long NbrOfPage = 0x00;				   //要擦除的页面数量
  	FLASH_Status FLASHStatus = FLASH_COMPLETE; //FLASH工作状态

   	/* Unlock the Flash Bank1 Program Erase controller */
  	FLASH->KEYR = ((unsigned long)0x45670123);
  	FLASH->KEYR = ((unsigned long)0xCDEF89AB);

  	/* Define the number of page to be erased */
  	NbrOfPage = (END_ADDR - START_ADDR) / ((unsigned short)0x800);

 	/* Clear All pending flags */
 	FLASH_ClearFlagNew(((unsigned long)0x00000020) | ((unsigned long)0x00000004) | ((unsigned long)0x00000010));	

    /* Erase the FLASH pages */
    for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
    {
      FLASHStatus = FLASH_ErasePageNew(START_ADDR + (((unsigned short)0x800) * EraseCounter));
    }

  	FLASH->CR |= ((unsigned long)0x00000080);
}

static void Flash_write(unsigned short *pdata,unsigned long START_ADDR,unsigned short Len) //写
{
	unsigned long Address = 0x00;  

  	FLASH_Status FLASHStatus = FLASH_COMPLETE; //FLASH工作状态

  	FLASH->KEYR = ((unsigned long)0x45670123);
  	FLASH->KEYR = ((unsigned long)0xCDEF89AB);

  	Address = START_ADDR;
	Len=Len+Len;

  	while((Address < START_ADDR+Len) && (FLASHStatus == FLASH_COMPLETE))
  	{
    	FLASHStatus = FLASH_ProgramHalfWordNew(Address, *pdata);
		pdata++;
    	Address = Address + 2;
  	}

  	FLASH->CR |= ((unsigned long)0x00000080);
}

void Up_Data(void)
{
	const unsigned char *signal="Z6UPDATEME";
	const unsigned char *Success="SUCCESS!!!";
	unsigned short Buff[512];
	unsigned char i=0;
	unsigned short Address = 0;
	unsigned char pageNumber = 0;
	unsigned short j=0;

	for(i=0;i<10;i++)
	{					   	 
		if (signal[i]!=(unsigned char)(*(volatile unsigned long*)(ISP_MARK+Address)) )
		{
			return;
		}
		Address=Address+2;
	}
	if(i==10)			   
	{
		pageNumber=(unsigned char)(*(volatile unsigned long*)(ISP_MARK+20));
	}

   	pageNumber=pageNumber/2+pageNumber%2;

	for(i=0;i<pageNumber;i++)
	{		
		for(j=0,Address=0;j<512;j++)
		{
			Buff[j]=(*(volatile unsigned long*)((unsigned long)ISP_CODE+(2*j)+(i*0x400)));					
		}

		if(i%2==0)	
		{
			Flash_Erase(((unsigned long)0x08000000)+(i/2)*0x800,((unsigned long)0x08000000)+(i/2)*0x800+0x800);
			Flash_write(Buff,((unsigned long)0x08000000)+(i/2)*0x800,512);
		}
		else
		{
			Flash_write(Buff,((unsigned long)0x08000000)+(i/2)*0x800+0x400,512);
		}
	}
	
	for(i=0;i<10;i++)
	{
		Buff[i]=Success[i];
	}

	Flash_Erase(((unsigned long)ISP_MARK),((unsigned long)ISP_MARK)+0x800);
	Flash_write(Buff,((unsigned long)ISP_MARK),10);
		 
	*(volatile unsigned long*)(unsigned long)0xE000ED0C  = ((unsigned long)0x05FA0004) ;//软件复位
}

#endif
