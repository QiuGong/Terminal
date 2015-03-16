#include "flash.h"
#include "formula.h"
#include "rs485.h"


#if ( RS485_EN > 0 )
/**
 * 获取sensor列表
 *
 * @param 
 *
 * @return 
 */
List get_sensor_list(void)
{
	return _get_sensor_list(); 
}


/**
 * RS485接收线程
 *
 * @param 
 *
 * @return 
 */
void rs485_rec_thread_entry(void* parameter)
{
	_rs485_rec_thread_entry(parameter);
}


/**
 * 获得RS485时间
 *
 * @param 
 *
 * @return 
 */
rt_uint32_t get_rs485_time(void)
{
	return _get_rs485_time();
}


/**
 * 获得RS485, FLASH最大值
 *
 * @param 
 *
 * @return 
 */
rt_uint16_t get_max_len(void)
{
	return RS485_LEN;
}


/**
 * RS485, 写入FLASH
 *
 * @param 
 *
 * @return 
 */
void refresh_to_flash(rt_uint8_t *buf, rt_uint16_t len)
{
#if ( FLASH_EN > 0 )
	Flash_Earse_Write(F_RS485, 0, buf, len);
#endif
}


/**
 * RS485, 读取FLASH
 *
 * @param 
 *
 * @return 
 */
void read_from_flash(rt_uint8_t *buf, rt_uint16_t len)
{
#if ( FLASH_EN > 0 )
	Flash_Read(F_RS485, 0, buf, RS485_LEN);
#endif
}


/**
 * 打印sensor
 *
 * @param 
 *
 * @return 
 */
void print_sensor(Position p, rt_uint8_t i)
{
	RT_DEBUG_LOG(DEBUG_RS485, ("-----------------------%d-------------------\n",(++i)));
	RT_DEBUG_LOG(DEBUG_RS485, ("Id:%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X\n", p->item.id[0], p->item.id[1], p->item.id[2], p->item.id[3], p->item.id[4], p->item.id[5], p->item.id[6], p->item.id[7], p->item.id[8], p->item.id[9], p->item.id[10], p->item.id[11]));
	RT_DEBUG_LOG(DEBUG_RS485, ("-------------------------------------------\n"));
	RT_DEBUG_LOG(DEBUG_RS485, ("value set:%04X \n", p->item.value_set));
	RT_DEBUG_LOG(DEBUG_RS485, ("timer set:%04X \n", p->item.timer_set));
	RT_DEBUG_LOG(DEBUG_RS485, ("last rec time:%d \n", p->item.last_rec_time));
	RT_DEBUG_LOG(DEBUG_RS485, ("-------------------------------------------\n"));
	RT_DEBUG_LOG(DEBUG_RS485, ("s1_c1-> val:%d, min:%d, max:%d, set:%08X, 电流:%d, 最小:%d, 最大:%d \n", p->item.sensor1_ch1, p->item.sensor1_ch1_min, p->item.sensor1_ch1_max, p->item.sensor1_ch1_set, p->item.sensor1_ch1, 					p->item.sensor1_ch1_min, 					p->item.sensor1_ch1_max));
	RT_DEBUG_LOG(DEBUG_RS485, ("s1_c2-> val:%d, min:%d, max:%d, set:%08X, 溶氧:%d, 最小:%d, 最大:%d \n", p->item.sensor1_ch2, p->item.sensor1_ch2_min, p->item.sensor1_ch2_max, p->item.sensor1_ch2_set, formula_do_16(p->item.sensor1_ch2), 	formula_do_16(p->item.sensor1_ch2_min), 	formula_do_16(p->item.sensor1_ch2_max)));
	RT_DEBUG_LOG(DEBUG_RS485, ("s2_c1-> val:%d, min:%d, max:%d, set:%08X, 酸碱:%d, 最小:%d, 最大:%d \n", p->item.sensor2_ch1, p->item.sensor2_ch1_min, p->item.sensor2_ch1_max, p->item.sensor2_ch1_set, formula_ph_16(p->item.sensor2_ch1), 	formula_ph_16(p->item.sensor2_ch1_min), 	formula_ph_16(p->item.sensor2_ch1_max)));
	RT_DEBUG_LOG(DEBUG_RS485, ("s2_c2-> val:%d, min:%d, max:%d, set:%08X, 温度:%d, 最小:%d, 最大:%d \n", p->item.sensor2_ch2, p->item.sensor2_ch2_min, p->item.sensor2_ch2_max, p->item.sensor2_ch2_set, formula_temp_16(p->item.sensor2_ch2), 	formula_temp_16(p->item.sensor2_ch2_min), 	formula_temp_16(p->item.sensor2_ch2_max)));	
	RT_DEBUG_LOG(DEBUG_RS485, ("-------------------------------------------\n"));
	RT_DEBUG_LOG(DEBUG_RS485, ("t1-> start:%04X,  end:%04X, set:%08X.\n", p->item.time1_start,  p->item.time1_end,  p->item.time1_set));
	RT_DEBUG_LOG(DEBUG_RS485, ("t2-> start:%04X,  end:%04X, set:%08X.\n", p->item.time2_start,  p->item.time2_end,  p->item.time2_set));
	RT_DEBUG_LOG(DEBUG_RS485, ("t3-> start:%04X,  end:%04X, set:%08X.\n", p->item.time3_start,  p->item.time3_end,  p->item.time3_set));
	RT_DEBUG_LOG(DEBUG_RS485, ("t4-> start:%04X,  end:%04X, set:%08X.\n", p->item.time4_start,  p->item.time4_end,  p->item.time4_set));		
	RT_DEBUG_LOG(DEBUG_RS485, ("t5-> start:%04X,  end:%04X, set:%08X.\n", p->item.time5_start,  p->item.time5_end,  p->item.time5_set));
	RT_DEBUG_LOG(DEBUG_RS485, ("t6-> start:%04X,  end:%04X, set:%08X.\n", p->item.time6_start,  p->item.time6_end,  p->item.time6_set));
	RT_DEBUG_LOG(DEBUG_RS485, ("t7-> start:%04X,  end:%04X, set:%08X.\n", p->item.time7_start,  p->item.time7_end,  p->item.time7_set));
	RT_DEBUG_LOG(DEBUG_RS485, ("t8-> start:%04X,  end:%04X, set:%08X.\n", p->item.time8_start,  p->item.time8_end,  p->item.time8_set));
	RT_DEBUG_LOG(DEBUG_RS485, ("t9-> start:%04X,  end:%04X, set:%08X.\n", p->item.time9_start,  p->item.time9_end,  p->item.time9_set));
	RT_DEBUG_LOG(DEBUG_RS485, ("t10-> start:%04X, end:%04X, set:%08X.\n", p->item.time10_start, p->item.time10_end, p->item.time10_set));
	RT_DEBUG_LOG(DEBUG_RS485, ("t11-> start:%04X, end:%04X, set:%08X.\n", p->item.time11_start, p->item.time11_end, p->item.time11_set));
	RT_DEBUG_LOG(DEBUG_RS485, ("t12-> start:%04X, end:%04X, set:%08X.\n", p->item.time12_start, p->item.time12_end, p->item.time12_set));		
	RT_DEBUG_LOG(DEBUG_RS485, ("t13-> start:%04X, end:%04X, set:%08X.\n", p->item.time13_start, p->item.time13_end, p->item.time13_set));
	RT_DEBUG_LOG(DEBUG_RS485, ("t14-> start:%04X, end:%04X, set:%08X.\n", p->item.time14_start, p->item.time14_end, p->item.time14_set));
	RT_DEBUG_LOG(DEBUG_RS485, ("t15-> start:%04X, end:%04X, set:%08X.\n", p->item.time15_start, p->item.time15_end, p->item.time15_set));
	RT_DEBUG_LOG(DEBUG_RS485, ("t16-> start:%04X, end:%04X, set:%08X.\n", p->item.time16_start, p->item.time16_end, p->item.time16_set));		
	RT_DEBUG_LOG(DEBUG_RS485, ("-------------------------------------------\n"));
	RT_DEBUG_LOG(DEBUG_RS485, ("K:%04X \n", p->item.k));
	RT_DEBUG_LOG(DEBUG_RS485, ("k1:%04X, k2:%04X, k3:%04X, k4:%04X.\n", p->item.k1_relate, p->item.k2_relate, p->item.k3_relate, p->item.k4_relate));
	RT_DEBUG_LOG(DEBUG_RS485, ("||||||||||||||||||||||||||||||||||||||||||||\n\n\n\n"));
}

#endif
