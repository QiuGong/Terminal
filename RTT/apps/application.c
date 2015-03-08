#include <board.h>
#include <rtthread.h>
#include "rs485.h"
#include "rtc.h"
#include "warn.h"
#include "gsm_driver.h"
#include "select_fun.h" 
#include "display.h"


int rt_gsm_init(void);

/* RS485 接收线程 */
#if ( RS485_EN > 0 )
	#define RS485_REC_THREAD_STACK_SIZE 512
	#define RS485_REC_HREAD_PRIORITY 9
	#define RS485_REC_THREAD_TIMESLICE 20
	static struct rt_thread rs485_rec_thread;
	ALIGN(4)
	static rt_uint8_t rt_thread_rs485_rec_stack[RS485_REC_THREAD_STACK_SIZE];
#endif


/* RS485 发送线程 */
#if ( RS485_EN > 0 )
	#define RS485_SEND_THREAD_STACK_SIZE 512
	#define RS485_SEND_HREAD_PRIORITY 9
	#define RS485_SEND_THREAD_TIMESLICE 20
	static struct rt_thread rs485_send_thread;
	ALIGN(4)
	static rt_uint8_t rt_thread_rs485_send_stack[RS485_SEND_THREAD_STACK_SIZE];
#endif


/* Display 接收线程 */
#if ( DISPLAY_EN > 0 )
	#define DISPLAY_REC_THREAD_STACK_SIZE 512
	#define DISPLAY_REC_HREAD_PRIORITY 10
	#define DISPLAY_REC_THREAD_TIMESLICE 20
	static struct rt_thread display_rec_thread;
	ALIGN(4)
	static rt_uint8_t rt_thread_display_rec_stack[DISPLAY_REC_THREAD_STACK_SIZE];
#endif


/* Display 发送线程 */
#if ( DISPLAY_EN > 0 )
	#define DISPLAY_SEND_THREAD_STACK_SIZE 512
	#define DISPLAY_SEND_HREAD_PRIORITY 10
	#define DISPLAY_SEND_THREAD_TIMESLICE 20
	static struct rt_thread display_send_thread;
	ALIGN(4)
	static rt_uint8_t rt_thread_display_send_stack[DISPLAY_SEND_THREAD_STACK_SIZE];
#endif


void rt_init_thread_entry(void* parameter)
{
	read_select_fun_from_flash();

#if ( DS1302_EN > 0 )	
	rt_hw_rtc_init();
#endif

#if ( WARN_EN > 0 )
	init_time_warn();
	warn_device_init();
#endif

#if ( SIM900_EN > 0 )
	rt_gsm_init();
#endif
}


int rt_application_init()
{
	rt_thread_t init_thread;
	
	
	init_thread = rt_thread_create("init", rt_init_thread_entry, RT_NULL, 2048, 8, 20);
	if (init_thread != RT_NULL)
	{
		rt_thread_startup(init_thread);
	}


#if ( RS485_EN > 0 )
	{
		rt_err_t result;
		
		result = rt_thread_init(&rs485_rec_thread, "485_rec",
								rs485_rec_thread_entry, RT_NULL, 
								&rt_thread_rs485_rec_stack[0], RS485_REC_THREAD_STACK_SIZE, 
								RS485_REC_HREAD_PRIORITY, RS485_REC_THREAD_TIMESLICE);
		if (result == RT_EOK)
			rt_thread_startup(&rs485_rec_thread);
	   	else
			return -1;
	
		
		result = rt_thread_init(&rs485_send_thread, "485_send",
								rs485_send_thread_entry, RT_NULL, 
								&rt_thread_rs485_send_stack[0], RS485_SEND_THREAD_STACK_SIZE, 
								RS485_SEND_HREAD_PRIORITY, RS485_SEND_THREAD_TIMESLICE);
		if (result == RT_EOK)
			rt_thread_startup(&rs485_send_thread);
	   	else
			return -1;
	}
#endif


#if ( DISPLAY_EN > 0 )
	{
		rt_err_t result;
		
		result = rt_thread_init(&display_rec_thread, "dsp_rec",
								display_rec_thread_entry, RT_NULL, 
								&rt_thread_display_rec_stack[0], DISPLAY_REC_THREAD_STACK_SIZE, 
								DISPLAY_REC_HREAD_PRIORITY, DISPLAY_REC_THREAD_TIMESLICE);
		if (result == RT_EOK)
			rt_thread_startup(&display_rec_thread);
	   	else
			return -1;
	
		
		result = rt_thread_init(&display_send_thread, "dsp_send",
								display_send_thread_entry, RT_NULL, 
								&rt_thread_display_send_stack[0], DISPLAY_SEND_THREAD_STACK_SIZE, 
								DISPLAY_SEND_HREAD_PRIORITY, DISPLAY_SEND_THREAD_TIMESLICE);
		if (result == RT_EOK)
			rt_thread_startup(&display_send_thread);
	   	else
			return -1;
	}
#endif

	return 0;
}

