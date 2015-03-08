#ifndef __WARN_H
#define __WARN_H

#include <config.h>
#include <rtthread.h>
#include <rs485.h>


// ��Ӿ��沽��
// 1)  config.h �����WARN��֧ WARN_RELATE
// 2)  select_fun.h SelectFun��ӱ���
// 3)  select_fun.c �������
//	   refresh_select_fun_to_flash(), 
//	   read_select_fun_from_flash(),
//	   list_define(),
//	   list_select_fun(),
// 	   select_fun()
// 4�� �½�C�ļ�����д����
// 5�� warn.h ������ �꣬���Ӻ�������
// 6�� warn.c  warn_callBack()������ 
// 7�� warn.c  ���� static control_relate_set
// 8�� warn.c  judge_control_set()������
// 9�� warn.c  control_set()������  
/*****************warn.c*******************/
#define WHICH_WARN_VALUE		1
#define WHICH_WARN_TIMER		2
#define WHICH_WARN_RELATE		3
#define WHICH_WARN_TIME_OUT		4
#define WHICH_WARN_POWER_DROP	5

enum WARN_CONTROL{WARN_CONTROL_LED = 0x01, WARN_CONTROL_BELL = 0x02};

void init_time_warn(void);
void warn_judge_set(Position p, rt_uint32_t value, rt_uint8_t which);


/*****************w_driver.c*******************/
#define WARN_LED_ON                  	 	0
#define WARN_LED_OFF                  	 	1

#define WARN_BELL_ON                  	 	0
#define WARN_BELL_OFF                  	 	1

#define WARN_POWER_ON                  	 	0
#define WARN_POWER_OFF                  	1

void warn_device_init(void);
void warn_led_off(void);
void warn_led_on(void);
rt_uint8_t warn_led_status(void);
void warn_bell_off(void);
void warn_bell_on(void);
rt_uint8_t warn_bell_status(void);
rt_uint8_t power_status(void);


/*****************warn_judge_value.c*******************/
void warn_judge_value(Position p);
rt_uint8_t get_warn_status(void);
void set_warn_status(rt_uint8_t w);


/*****************warn_judge_timer.c*******************/
void warn_judge_timer(Position p);


/*****************warn_relate.c*******************/
void warn_relate(Position p);


/*****************warn_time_out.c*******************/
void warn_time_out(Position p);


/*****************warn_power_drop.c*******************/
void warn_power_drop(void);


#endif
