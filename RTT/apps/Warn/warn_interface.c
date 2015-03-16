#include "warn.h"
#include "rtc.h"


#if ( WARN_TIMER > 0 )

/**
 * 获取系统时间（时分）
 *
 * @param 
 *
 * @return 前8位时，后8位分 
 */
rt_uint16_t get_system_hh_mm(void)
{
	return get_hh_mm();
}


/**
 * 获取RS485运行时间
 *
 * @param 
 *
 * @return 
 */
rt_uint32_t get_rs485_running(void)
{
	return get_rs485_time();
}


/**
 * 开始运行报警定时器
 *
 * @param 
 *
 * @return 
 */
void warn_timer_start(void)
{
	_init_time_warn();
}


/**
 * 获取功能使能
 *
 * @param 
 *
 * @return 
 */
SelectFun get_fun_enable(void)
{
	return get_select_fun();
}

#endif
