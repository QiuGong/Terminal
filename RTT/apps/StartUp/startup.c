/*
 * File      : startup.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://openlab.rt-thread.com/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2006-08-31     Bernard      first implementation
 */

#include <rthw.h>
#include <rtthread.h>

#include "stm32f10x.h"
#include "board.h"
#include "rtc.h"


extern int  rt_application_init(void);
#ifdef RT_USING_FINSH
extern void finsh_system_init(void);
extern void finsh_set_device(const char* device);
#endif

#ifdef __CC_ARM
extern int Image$$RW_IRAM1$$ZI$$Limit;
#elif __ICCARM__
#pragma section="HEAP"
#else
extern int __bss_end;
#endif


void assert_failed(u8* file, u32 line)
{
	rt_kprintf("\n\r Wrong parameter value detected on\r\n");
	rt_kprintf("       file  %s\r\n", file);
	rt_kprintf("       line  %d\r\n", line);

	while (1) ;
}


/**
 * This function will startup RT-Thread RTOS.
 */
void rtthread_startup(void)
{
	/* 初始化硬件平台相关：时钟设置、中断设置、系统滴答设置、串口设置 */
	rt_hw_board_init();

	/* init tick */
	rt_system_tick_init();

	/* 内核对象初始化 */
	rt_system_object_init();

	/* 系统定时器初始化 */
	rt_system_timer_init();

	/* 如果使用动态内存分配，则配置之 */
#ifdef RT_USING_HEAP
#if STM32_EXT_SRAM
	rt_system_heap_init((void*)STM32_EXT_SRAM_BEGIN, (void*)STM32_EXT_SRAM_END);
#else
	#ifdef __CC_ARM
		rt_system_heap_init((void*)&Image$$RW_IRAM1$$ZI$$Limit, (void*)STM32_SRAM_END);
	#elif __ICCARM__
	    rt_system_heap_init(__segment_end("HEAP"), (void*)STM32_SRAM_END);
	#else
		/* init memory system */
		rt_system_heap_init((void*)&__bss_end, (void*)STM32_SRAM_END);
	#endif
#endif
#endif

	/* 系统调度器初始化 */
	rt_system_scheduler_init();

#ifdef RT_USING_DFS
	/* init sdcard driver */
#if STM32_USE_SDIO
	rt_hw_sdcard_init();
#else
	rt_hw_msd_init();
#endif
#endif

	/*下面可加入用户所需的相关初始化 */

	/*上面可加入用户所需的相关初始化 */


    /* 实时时钟初始化 */
#if(0)
    rt_hw_rtc_init();
#endif

	/* 系统设备对象初始化 */
	rt_device_init_all();

	/* 用户应用初始化 */
	rt_application_init();

#ifdef RT_USING_FINSH
	/* init finsh */
	finsh_system_init();
	finsh_set_device("uart1");
#endif

    /* 初始化软件定时器 */
    rt_system_timer_thread_init();

	/* 初始化空闲线程 */
	rt_thread_idle_init();

	/* 开始线程调度 此后便进入各个线程的无限循环 */
	rt_system_scheduler_start();

	/* never reach here */
	return ;
}

int main(void)
{
	/* disable interrupt first */
	rt_hw_interrupt_disable();

	/* startup RT-Thread RTOS */
	rtthread_startup();

	return 0;
}

/*@}*/
