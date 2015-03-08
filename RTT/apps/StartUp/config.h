#ifndef  __CONFIG_H__
#define  __CONFIG_H__

#include <rtthread.h>


// 采集主机
#define  RS485_EN			1
// 显示屏指令
#define  DISPLAY_EN			1 
// SIM900
#define  SIM900_EN			1   


//  RS1302 时钟
#if (RS485_EN > 0)
#define	 DS1302_EN			1
#else
#define	 DS1302_EN			0
#endif


//  FLASH 配置
#if (RS485_EN > 0)
#define	 FLASH_EN			1	
#else
#define	 FLASH_EN			0	
#endif


//  LED GSM 配置
#if (SIM900_EN > 0)
#define	 LED_GSM_EN			1	
#else
#define	 LED_GSM_EN			0	
#endif


//  报警设置
#if (RS485_EN > 0)
#define	 WARN_EN			1
#define  WARN_LED			1
#define  WARN_BELL			1
#define  WARN_VALUE			1
#define  WARN_TIMER			1
#define  WARN_RELATE		1
#define  WARN_TIME_OUT		1
#define  WARN_POWER_DROP	1
#else
#define	 WARN_EN			0
#define  WARN_LED			0
#define  WARN_BELL			0
#define  WARN_VALUE			0
#define  WARN_TIMER			0
#define  WARN_RELATE		0
#define  WARN_TIME_OUT		0
#define  WARN_POWER_DROP	0
#endif



/********* 调试开关 ************/
#define	 DEBUG_RS485		1	
#define	 DEBUG_WARN			1
#define  DEBUG_DISPLAY		1
#define  DEBUG_SIM900		1


#endif
