#ifndef __FLASH_H
#define __FLASH_H

#include <rtthread.h>
#include "config.h"
#include "rs485.h"

/**************************************************************/
//ÿҳ2048���ֽڣ�2K����FLASH����512K����256ҳ
//��0x08000000~0x08080000���ܵ�ַ

//��0x08000000~0x08030000������192K
//��0x08030000~0x08030800������, 2k
//��0x08030800~0x08031000������, 2k

//��0x08040000~0x08070000����������192K
//��0x08070000~0x08070800�������жϱ�־,2K��1ҳ��

//��0x0807F000~0x08080000��ISP���� 4K(2ҳ)
/**************************************************************/


/****��0x08030000~0x08040000������,ä�� 64K ****/
#define 	SELECT_FUN_LEN		20 						// $(1bit) + fun(19bit) 
#define     RS485_LEN			1 + 1 + PER_SENSOR_FLASH_LEN * MAX_SENSOR // $(1bit) + ����(1bit) + ÿ�鴫��������() * ��󴫸���(10) 
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
