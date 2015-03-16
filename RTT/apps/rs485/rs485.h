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



/* Э���ʽ */
// ��ͷ
#define HEAD        			0xAA
#define HEAD_LEN    			0x01

// ��β
#define END         			0xDD
#define END_LEN     			0x01

// �����
#define CMD_LEN     			0x01

// ID����
#define ID_LEN      			12

// ����
#define DATA1       			0x00
#define DATA2       			0x01
#define DATA3       			0x02
#define DATA4       			0x03
#define DATA_LEN    			4

// �ܳ���
#define ALL_LEN     			19



/* ��������	*/
#define TURN_CMD   				0x01
enum X01_DEV{ LED = 0x01, CH1 = 0x02, T2 = 0x03, T3 = 0x04, M_A = 0x05, M_B = 0x06, M_C = 0x07};
enum X01_CONTROL{ ON = 0x01, OFF = 0x00};

// �ж�����,ÿ�鳤��
#define TIME_JUDGE_NUM			0x10 * 2
#define TIME_PER_LEN			0x02

// ��ʱ����ֵ����ʱ����ֵ����
#define TIME_SET_VALUE_NUM		0x10
#define TIME_SET_PER_LEN		0x04



/* ͨ��Ӧ��	*/
#define GENERAL_CMD 			0x10



/* ������ */
// ������һ ��ѯ 
#define SENSOR_1    			0x02

// �������� ��ѯ 
#define SENSOR_2    			0x03

// �ж�����,ÿ�鳤��
#define SENSOR_JUDGE_NUM		0x04 * 2
#define SENSOR_PER_LEN			0x02

// ��ֵ����ֵ����ֵ����ֵ����
#define SENSOR_SET_VALUE_NUM	0x04
#define SENSOR_SET_PER_LEN		0x04



/* �ߵ͵�ƽ ��ѯ */
#define DETECT_K    			0x04
#define K1_SET      			0x01
#define K2_SET      			0x02
#define K3_SET      			0x04
#define K4_SET      			0x08

// ��ֵ����ֵ����ֵ����ֵ����
#define K_RELATE_VALUE_NUM		0x04 
#define K_RELATE_PER_LEN		0x02 



/* ����������� */
#define MAX_SENSOR  			10



/* ÿ�鴫�������� */
#define PER_SENSOR_FLASH_LEN (ID_LEN + (SENSOR_PER_LEN * SENSOR_JUDGE_NUM + SENSOR_SET_VALUE_NUM * SENSOR_SET_PER_LEN) + (TIME_PER_LEN * TIME_JUDGE_NUM + TIME_SET_VALUE_NUM * TIME_SET_PER_LEN) + (K_RELATE_VALUE_NUM * K_RELATE_PER_LEN))



/* �������䣬�����Ϣ��	*/
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
// ��� Sensor ����
// 1) typedef struct ELEMENT{}Element �����ȫ�ֱ���
// 2) PER_SENSOR_FLASH_LEN �޸ĳ���
// 3) void refresh_sensor_to_flash(void) ��ӱ���
// 4) void read_sensor_from_flash(void) ��ӱ���
// 5) RT_USING_FINSH �б�д���ú���
// 6) list_sensor() ���ӱ����鿴
typedef struct ELEMENT
{			
	rt_uint8_t  id[ID_LEN]; /* ID */


	/*	
	1λ�� LED(0�أ�1��)
	2λ�� CH1(0�أ�1��)
	3λ�� T2 (0�أ�1��)
	4λ�� T3 (0�أ�1��)
	5λ�� M_A(0�أ�1��)
	6λ�� M_B(0�أ�1��)
	7λ�� M_C(0�أ�1��)

	9λ�� K1(0�ɿ���1����)
	10λ��K2(0�ɿ���1����)
	11λ��K3(0�ɿ���1����)
	12λ��K4(0�ɿ���1����)
	*/
	rt_uint16_t	k;


	/*	
	1λ�� LED (0�أ�1��)
	2λ�� CH1 (0�أ�1��)
	3λ�� T2  (0�أ�1��)
	4λ�� T3  (0�أ�1��)
	5λ�� M_A (0�أ�1��)
	6λ�� M_B (0�أ�1��)
	7λ�� M_C (0�أ�1��)
	*/
	rt_uint16_t	value_set;	/* ��ֵ��λ */
	rt_uint16_t	timer_set;	/* ��ʱ��λ	*/


	// ������ʱ��	
	rt_uint32_t	last_rec_time;	


	// ������ֵ����С����󣬿��Ʊ�־
	rt_uint16_t	sensor1_ch1;// ����
	rt_uint16_t	sensor1_ch1_min;
	rt_uint16_t	sensor1_ch1_max;
	rt_uint32_t sensor1_ch1_set;

	rt_uint16_t	sensor1_ch2;// ������
	rt_uint16_t	sensor1_ch2_min;
	rt_uint16_t	sensor1_ch2_max;
	rt_uint32_t sensor1_ch2_set;

	rt_uint16_t	sensor2_ch1;// PH
	rt_uint16_t	sensor2_ch1_min;
	rt_uint16_t	sensor2_ch1_max;
	rt_uint32_t sensor2_ch1_set;

	rt_uint16_t	sensor2_ch2;// �¶�
	rt_uint16_t	sensor2_ch2_min;
	rt_uint16_t	sensor2_ch2_max;
	rt_uint32_t	sensor2_ch2_set;

	
	// ����ʱ�俪ʼ�����������Ʊ�־
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


	// ���ù������
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
typedef Element Item;/*��������������*/


typedef struct node *PNode;/*����ڵ�ָ��*/
typedef struct node/*�ڵ�Ķ���*/
{
	Item item;  /*������*/
	PNode next; /*����*/	
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
