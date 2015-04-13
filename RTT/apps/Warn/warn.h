#ifndef __WARN_H
#define __WARN_H

#include <config.h>
#include <rtthread.h>
#include <rs485.h>
#include "select_fun.h"


// 添加警告步骤
// 1)  config.h 中添加WARN分支 WARN_RELATE
// 2)  select_fun.h SelectFun添加变量
// 3)  select_fun.c 添加以下
//	   refresh_select_fun_to_flash(), 
//	   read_select_fun_from_flash(),
//	   list_define(),
//	   list_select_fun(),
// 	   select_fun()
// 4） 新建C文件，编写函数
// 5） warn.h 中增加 宏，增加函数定义
// 6） warn.c  warn_callBack()中增加 
// 7） warn.c  增加 static control_relate_set
// 8） warn.c  judge_control_set()中增加
// 9） warn.c  control_set()中增加  

/*****************w_driver.c*******************/
#define WARN_LED_ON                  	 	0
#define WARN_LED_OFF                  	 	1

#define WARN_BELL_ON                  	 	0
#define WARN_BELL_OFF                  	 	1

#define WARN_POWER_ON                  	 	0
#define WARN_POWER_OFF                  	1

void warn_led_off(void);
void warn_led_on(void);
rt_uint8_t warn_led_status(void);
void warn_bell_off(void);
void warn_bell_on(void);
rt_uint8_t warn_bell_status(void);
rt_uint8_t power_status(void);
void warn_device_init(void);


/*****************warn.c*******************/
#define WHICH_WARN_VALUE		1
#define WHICH_WARN_TIMER		2
#define WHICH_WARN_RELATE		3
#define WHICH_WARN_TIME_OUT		4
#define WHICH_WARN_POWER_DROP	5

enum WARN_CONTROL{WARN_CONTROL_LED = 0x01, WARN_CONTROL_BELL = 0x02};

void _init_time_warn(void);
void warn_judge_set(Position p, rt_uint32_t value, rt_uint8_t which);


/*****************warn_rep.c*******************/
void warn_rep(Position p);


/*****************warn_judge_value.c*******************/
void warn_judge_value(Position p);
rt_uint8_t _get_value_warn_flag(void);
void _set_value_warn_flag(rt_uint8_t value);


/*****************warn_judge_timer.c*******************/
void warn_judge_timer(Position p);


/*****************warn_relate.c*******************/
void warn_relate(Position p);


/*****************warn_time_out.c*******************/
void warn_time_out(Position p);
rt_uint8_t _get_timeout_warn_flag(void);
void _set_timeout_warn_flag(rt_uint8_t value);


/*****************warn_power_drop.c*******************/
void warn_power_drop(void);


/*****************warn_interface.c*******************/
List _get_sensor(void);
void _control_sensor(rt_uint8_t *id, enum X01_DEV dev, enum X01_CONTROL control);
rt_uint16_t _get_system_hh_mm(void);
rt_uint32_t _get_rs485_running(void);
void warn_timer_start(void);
SelectFun _get_fun_enable(void);
rt_uint8_t get_value_warn_flag(void);
void set_value_warn_flag(rt_uint8_t value);
rt_uint8_t get_timeout_warn_flag(void);
void set_timeout_warn_flag(rt_uint8_t value);


#endif
