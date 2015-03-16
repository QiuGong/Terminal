#include "warn.h"
#include "rtc.h"


#if ( WARN_TIMER > 0 )

/**
 * ��ȡϵͳʱ�䣨ʱ�֣�
 *
 * @param 
 *
 * @return ǰ8λʱ����8λ�� 
 */
rt_uint16_t get_system_hh_mm(void)
{
	return get_hh_mm();
}


/**
 * ��ȡRS485����ʱ��
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
 * ��ʼ���б�����ʱ��
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
 * ��ȡ����ʹ��
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
