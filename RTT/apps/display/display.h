#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <config.h>
#include <rtthread.h>
#include <stm32f10x.h>


/****************display.c*******************/
/* 协议格式 */
// 包头
#define U_HEAD          0x24
#define U_HEAD_LEN      0x01

// 包尾
#define U_END           0x26
#define U_END_LEN       0x01

// 命令长度
#define U_CMD_LEN       0x01

// 数据长度
#define U_DATA_LEN      8

// 总长度
#define U_ALL_LEN       11


/* 接收邮箱，最大消息数	*/
#define MAX_MQ  		3


/* 接收命令 */
//时间，警告状态
#define U_TIME_CMD              	0x01
#define U_NORMAL                	0x00
#define U_TEMP                  	0x01
#define U_PH                    	0x02
#define U_DO                    	0x03


//传感器数据查询
#define U_SENSOR_CMD                0x02


//查询传感器个数
#define U_QUERY_SENSOR_CMD          0x03

//查询传感器n值
#define U_QUERY_SENSOR_VALUE_CMD    0x04

//设置传感器n值
#define U_QUERY_SENSOR_SET_CMD      0x05


//查询定时器个数
#define U_QUERY_TIMER_CMD           0x06

//查询定时器n值
#define U_QUERY_TIMER_VALUE_CMD     0x07

//设置定时器n值
#define U_QUERY_TIMER_SET_CMD       0x08

void display_rec_thread_entry(void* parameter);
void display_send(rt_uint8_t *b, rt_uint8_t len);


/*****************rsRequest.c*******************/
void display_send_thread_entry(void *parameter);
void x02_dsp_request(rt_uint8_t which, rt_uint16_t temp, rt_uint16_t ph, rt_uint16_t Do);
void x03_dsp_request(rt_uint8_t cnt);
void x04_dsp_request(rt_uint8_t temp_min, rt_uint8_t temp_max, 
					 rt_uint8_t ph_min, rt_uint8_t ph_max, 
					 rt_uint8_t do_min, rt_uint8_t do_max, 
					 rt_uint8_t relate);
void x06_dsp_request(rt_uint8_t cnt);
void x07_dsp_request(rt_uint8_t which, rt_uint8_t  h1, rt_uint8_t m1, rt_uint8_t h2, rt_uint8_t m2);


/*****************dspRespond.c*******************/
void x01_dsp_respond(rt_uint8_t *b);
void x02_dsp_respond(rt_uint8_t *b);
void x03_dsp_respond(rt_uint8_t *b);
void x04_dsp_respond(rt_uint8_t *b);
void x05_dsp_respond(rt_uint8_t *b);
void x06_dsp_respond(rt_uint8_t *b);
void x07_dsp_respond(rt_uint8_t *b);
void x08_dsp_respond(rt_uint8_t *b);


/*****************convert.c*******************/
void get_sensor_value(void);
void get_query_sensor_cnt(void);
void get_query_sensor_value(rt_uint8_t which);
void set_query_sensor_value(rt_uint8_t *b);
void get_time_cnt(void);
void get_time_value(rt_uint8_t which);
void set_time_value(rt_uint8_t *d);

#endif
