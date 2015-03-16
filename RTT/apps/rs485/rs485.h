#ifndef __RS485_H
#define __RS485_H

#include <config.h>
#include <rtthread.h>
#include <stm32f10x.h>


/*****************rs485.c*******************/
/* EN485(PA0) */
#define EN_RS485_RCC        	RCC_APB2Periph_GPIOA
#define EN_RS485_GPIO       	GPIOA
#define EN_RS485_PIN        	(GPIO_Pin_0)
#define EN_RS485_DISABLE 		GPIO_SetBits(EN_RS485_GPIO, EN_RS485_PIN);
#define EN_RS485_ENABLE 		GPIO_ResetBits(EN_RS485_GPIO, EN_RS485_PIN);



/* 协议格式 */
// 包头
#define HEAD        			0xAA
#define HEAD_LEN    			0x01

// 包尾
#define END         			0xDD
#define END_LEN     			0x01

// 命令长度
#define CMD_LEN     			0x01

// ID长度
#define ID_LEN      			12

// 数据
#define DATA1       			0x00
#define DATA2       			0x01
#define DATA3       			0x02
#define DATA4       			0x03
#define DATA_LEN    			4

// 总长度
#define ALL_LEN     			19



/* 开关命令	*/
#define TURN_CMD   				0x01
enum X01_DEV{ LED = 0x01, CH1 = 0x02, T2 = 0x03, T3 = 0x04, M_A = 0x05, M_B = 0x06, M_C = 0x07};
enum X01_CONTROL{ ON = 0x01, OFF = 0x00};

// 判断组数,每组长度
#define TIME_JUDGE_NUM			0x10 * 2
#define TIME_PER_LEN			0x02

// 定时控制值，定时控制值长度
#define TIME_SET_VALUE_NUM		0x10
#define TIME_SET_PER_LEN		0x04



/* 通用应答	*/
#define GENERAL_CMD 			0x10



/* 传感器 */
// 传感器一 查询 
#define SENSOR_1    			0x02

// 传感器二 查询 
#define SENSOR_2    			0x03

// 判断组数,每组长度
#define SENSOR_JUDGE_NUM		0x04 * 2
#define SENSOR_PER_LEN			0x02

// 超值控制值，超值控制值长度
#define SENSOR_SET_VALUE_NUM	0x04
#define SENSOR_SET_PER_LEN		0x04



/* 高低电平 查询 */
#define DETECT_K    			0x04
#define K1_SET      			0x01
#define K2_SET      			0x02
#define K3_SET      			0x04
#define K4_SET      			0x08

// 超值控制值，超值控制值长度
#define K_RELATE_VALUE_NUM		0x04 
#define K_RELATE_PER_LEN		0x02 



/* 传感器最大数 */
#define MAX_SENSOR  			10



/* 每组传感器长度 */
#define PER_SENSOR_FLASH_LEN (ID_LEN + (SENSOR_PER_LEN * SENSOR_JUDGE_NUM + SENSOR_SET_VALUE_NUM * SENSOR_SET_PER_LEN) + (TIME_PER_LEN * TIME_JUDGE_NUM + TIME_SET_VALUE_NUM * TIME_SET_PER_LEN) + (K_RELATE_VALUE_NUM * K_RELATE_PER_LEN))



/* 接收邮箱，最大消息数	*/
#define MAX_MQ  				3



void _rs485_rec_thread_entry(void* parameter);
void re485_send(rt_uint8_t *b, rt_uint8_t len);


/*****************rsRequest.c*******************/
void x01_request(rt_uint8_t *id, enum X01_DEV dev, enum X01_CONTROL control);
rt_uint32_t _get_rs485_time(void);
void rs485_send_thread_entry(void *parameter);


/*****************rsRespond.c*******************/
void x01_respond(rt_uint8_t *b);
void x02_respond(rt_uint8_t *b);
void x03_respond(rt_uint8_t *b);
void x04_respond(rt_uint8_t *b);
void rec_last_time(rt_uint8_t *b);


/*****************sensor.c*******************/
// 添加 Sensor 步骤
// 1) typedef struct ELEMENT{}Element 中添加全局变量
// 2) PER_SENSOR_FLASH_LEN 修改长度
// 3) void refresh_sensor_to_flash(void) 添加变量
// 4) void read_sensor_from_flash(void) 添加变量
// 5) RT_USING_FINSH 中编写设置函数
// 6) list_sensor() 增加变量查看
typedef struct ELEMENT
{			
	rt_uint8_t  id[ID_LEN]; /* ID */


	/*	
	1位： LED(0关，1开)
	2位： CH1(0关，1开)
	3位： T2 (0关，1开)
	4位： T3 (0关，1开)
	5位： M_A(0关，1开)
	6位： M_B(0关，1开)
	7位： M_C(0关，1开)

	9位： K1(0松开，1按下)
	10位：K2(0松开，1按下)
	11位：K3(0松开，1按下)
	12位：K4(0松开，1按下)
	*/
	rt_uint16_t	k;


	/*	
	1位： LED (0关，1开)
	2位： CH1 (0关，1开)
	3位： T2  (0关，1开)
	4位： T3  (0关，1开)
	5位： M_A (0关，1开)
	6位： M_B (0关，1开)
	7位： M_C (0关，1开)
	*/
	rt_uint16_t	value_set;	/* 阈值置位 */
	rt_uint16_t	timer_set;	/* 定时置位	*/


	// 最后接收时间	
	rt_uint32_t	last_rec_time;	


	// 传感器值，最小，最大，控制标志
	rt_uint16_t	sensor1_ch1;// 电流
	rt_uint16_t	sensor1_ch1_min;
	rt_uint16_t	sensor1_ch1_max;
	rt_uint32_t sensor1_ch1_set;

	rt_uint16_t	sensor1_ch2;// 溶氧度
	rt_uint16_t	sensor1_ch2_min;
	rt_uint16_t	sensor1_ch2_max;
	rt_uint32_t sensor1_ch2_set;

	rt_uint16_t	sensor2_ch1;// PH
	rt_uint16_t	sensor2_ch1_min;
	rt_uint16_t	sensor2_ch1_max;
	rt_uint32_t sensor2_ch1_set;

	rt_uint16_t	sensor2_ch2;// 温度
	rt_uint16_t	sensor2_ch2_min;
	rt_uint16_t	sensor2_ch2_max;
	rt_uint32_t	sensor2_ch2_set;

	
	// 设置时间开始，结束，控制标志
	rt_uint16_t time1_start;
	rt_uint16_t time1_end;
	rt_uint32_t time1_set;
	
	rt_uint16_t time2_start;
	rt_uint16_t time2_end;
	rt_uint32_t time2_set;

	rt_uint16_t time3_start;
	rt_uint16_t time3_end;
	rt_uint32_t time3_set;	

	rt_uint16_t time4_start;
	rt_uint16_t time4_end;
	rt_uint32_t time4_set;

	rt_uint16_t time5_start;
	rt_uint16_t time5_end;
	rt_uint32_t time5_set;

	rt_uint16_t time6_start;
	rt_uint16_t time6_end;
	rt_uint32_t time6_set;

	rt_uint16_t time7_start;
	rt_uint16_t time7_end;
	rt_uint32_t time7_set;

	rt_uint16_t time8_start;
	rt_uint16_t time8_end;
	rt_uint32_t time8_set;

	rt_uint16_t time9_start;
	rt_uint16_t time9_end;
	rt_uint32_t time9_set;

	rt_uint16_t time10_start;
	rt_uint16_t time10_end;
	rt_uint32_t time10_set;

	rt_uint16_t time11_start;
	rt_uint16_t time11_end;
	rt_uint32_t time11_set;

	rt_uint16_t time12_start;
	rt_uint16_t time12_end;
	rt_uint32_t time12_set;

	rt_uint16_t time13_start;
	rt_uint16_t time13_end;
	rt_uint32_t time13_set;

	rt_uint16_t time14_start;
	rt_uint16_t time14_end;
	rt_uint32_t time14_set;

	rt_uint16_t time15_start;
	rt_uint16_t time15_end;
	rt_uint32_t time15_set;

	rt_uint16_t time16_start;
	rt_uint16_t time16_end;
	rt_uint32_t time16_set;


	// 设置关联检测
	rt_uint16_t k1_relate;
	rt_uint16_t k2_relate;
	rt_uint16_t k3_relate;
	rt_uint16_t k4_relate;				
}Element;


rt_uint8_t add_sensor(rt_uint8_t *id);
Element* find_sensor(rt_uint8_t *b);
void refresh_sensor_to_flash(void);
void read_sensor_from_flash(void);


/*****************list.c*******************/
typedef Element Item;/*定义数据项类型*/


typedef struct node *PNode;/*定义节点指针*/
typedef struct node/*节点的定义*/
{
	Item item;  /*数据域*/
	PNode next; /*链域*/	
}Node;


typedef  PNode Position;
typedef  PNode List;

List MakeEmpty(List L);
Position Find(void *b, List L);
Position FindPrevious(void *b, List L);
void Insert(Position P);
void* Delete(void *b, List sensor);
unsigned char Length(List L);


/*****************rsInterface.c*******************/
List get_sensor_list(void);
List _get_sensor_list(void);
void rs485_rec_thread_entry(void* parameter);
rt_uint32_t get_rs485_time(void);
rt_uint16_t get_max_len(void);
void refresh_to_flash(rt_uint8_t *buf, rt_uint16_t len);
void read_from_flash(rt_uint8_t *buf, rt_uint16_t len);
void print_sensor(Position p, rt_uint8_t i);


#endif
