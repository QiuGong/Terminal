#include "warn.h"
#include "rtc.h"


#if ( WARN_TIMER > 0 )

/**
 * ��ȡ�������б�
 *
 * @param 
 *
 * @return 
 */
List _get_sensor(void)
{
	return get_sensor_list();
}


/**
 * ���ƴ�����
 *
 * @param 
 *
 * @return 
 */
void _control_sensor(rt_uint8_t *id, enum X01_DEV dev, enum X01_CONTROL control)
{
	control_sensor(id, dev, control);
}


/**
 * ��ȡϵͳʱ�䣨ʱ�֣�
 *
 * @param 
 *
 * @return ǰ8λʱ����8λ�� 
 */
rt_uint16_t _get_system_hh_mm(void)
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
rt_uint32_t _get_rs485_running(void)
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
SelectFun _get_fun_enable(void)
{
	return get_select_fun();
}


/**
 * ��ȡ��ֵ����
 *
 * @param 
 *
 * @return 
 */
rt_uint8_t get_value_warn_flag(void)
{
	return 	_get_value_warn_flag();
}


/**
 * ��ȡ��ʱ����
 *
 * @param 
 *
 * @return 
 */
rt_uint8_t get_timeout_warn_flag(void)
{
	return 	_get_timeout_warn_flag();
}

#endif
