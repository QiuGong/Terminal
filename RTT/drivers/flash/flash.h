#ifndef __FLASH_H
#define __FLASH_H

#include <rtthread.h>
#include "config.h"
#include "rs485.h"

/**************************************************************/
//每页2048个字节（2K），FLASH共（512K），256页
//（0x08000000~0x08080000）总地址

//（0x08000000~0x08030000）程序，192K
//（0x08030000~0x08030800）变量, 2k
//（0x08030800~0x08031000）备份, 2k

//（0x08040000~0x08070000）升级程序，192K
//（0x08070000~0x08070800）升级判断标志,2K（1页）

//（0x0807F000~0x08080000）ISP程序 4K(2页)
/**************************************************************/


/****（0x08030000~0x08040000）变量,盲区 64K ****/
#define 	SELECT_FUN_LEN		20 						// $(1bit) + fun(19bit) 
#define     RS485_LEN			1 + 1 + PER_SENSOR_FLASH_LEN * MAX_SENSOR // $(1bit) + 长度(1bit) + 每组传感器长度() * 最大传感器(10) 
#define		BACKUP_LEN			1 + 6 + SELECT_FUN_LEN + RS485_LEN	 // $(1bit) + time(6bit) + SELECT_FUN_LEN + RS485_LEN


enum Flash_Data{ F_SELECT_FUN 	= 0x08030000, 
				 F_RS485 		= F_SELECT_FUN + SELECT_FUN_LEN,
				 F_BACKUP 		= 0x08031000
				};


/*****************flash.c*******************/
void 	_flash_erase(rt_uint32_t START_ADDR, rt_uint32_t END_ADDR);
void 	_flash_write(rt_uint16_t *pdata, rt_uint32_t START_ADDR, rt_uint16_t Len);
rt_uint8_t _flash_write_check(rt_uint16_t *pdata, rt_uint32_t START_ADDR, rt_uint16_t Len);


/*****************data.c*******************/
void 	Flash_Earse_Write(enum Flash_Data type, rt_uint32_t ops, rt_uint8_t *b, rt_uint16_t len);
void 	Flash_Read(enum Flash_Data type, rt_uint32_t ops, rt_uint8_t *b, rt_uint16_t len);
void 	Flash_Erase_Para(void);


/*****************backup.c*******************/


#endif
