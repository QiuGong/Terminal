#include <rtthread.h>
#include <stm32f10x.h>
#include "ds1302.h"

#if ( DS1302_EN > 0 )

#define DS1302_CLK_RCC              RCC_APB2Periph_GPIOB
#define DS1302_CLK_PORT				GPIOB
#define CLK_PIN                   	GPIO_Pin_12

#define DS1302_RST_RCC              RCC_APB2Periph_GPIOB
#define DS1302_RST_PORT				GPIOB
#define RST_PIN                   	GPIO_Pin_14

#define DS1302_IO_RCC               RCC_APB2Periph_GPIOB
#define DS1302_IO_PORT			 	GPIOB
#define IO_PIN                    	GPIO_Pin_13


#define DS1302_CLK_H()				(DS1302_CLK_PORT->BSRR	= CLK_PIN)
#define DS1302_CLK_L()				(DS1302_CLK_PORT->BRR	= CLK_PIN)

#define DS1302_RST_H()				(DS1302_RST_PORT->BSRR	= RST_PIN)
#define DS1302_RST_L()				(DS1302_RST_PORT->BRR	= RST_PIN)

#define DS1302_OUT_H()				(DS1302_IO_PORT->BSRR	= IO_PIN)
#define DS1302_OUT_L()				(DS1302_IO_PORT->BRR	= IO_PIN)
											
#define DS1302_IN_X					(DS1302_IO_PORT->IDR	& IO_PIN)

#define Time_24_Hour				0x00	//24ʱ�ƿ���	
#define Time_Start					0x00	//��ʼ��ʱ

#define DS1302_SECOND				0x80	//DS1302���Ĵ������������
#define DS1302_MINUTE				0x82
#define DS1302_HOUR					0x84
#define DS1302_DAY					0x86
#define DS1302_MONTH				0x88
#define DS1302_WEEK					0x8A
#define DS1302_YEAR					0x8C
#define DS1302_WRITE				0x8E
#define DS1302_POWER				0x90


static void DS1302_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(DS1302_CLK_RCC | DS1302_RST_RCC | DS1302_IO_RCC, ENABLE);

	GPIO_ResetBits(DS1302_CLK_PORT, CLK_PIN);
	GPIO_InitStructure.GPIO_Pin 	= CLK_PIN;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;	
	GPIO_Init(DS1302_CLK_PORT, &GPIO_InitStructure);

	GPIO_ResetBits(DS1302_RST_PORT, RST_PIN);
	GPIO_InitStructure.GPIO_Pin 	= RST_PIN;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;	
	GPIO_Init(DS1302_RST_PORT, &GPIO_InitStructure);

	GPIO_ResetBits(DS1302_IO_PORT, IO_PIN);
	GPIO_InitStructure.GPIO_Pin 	= IO_PIN;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;	
	GPIO_Init(DS1302_IO_PORT, &GPIO_InitStructure);	
}


static void DelayNOP(u32 count)
{
	while(count--);
}


static void DS1302_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin 	= IO_PIN;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(DS1302_IO_PORT, &GPIO_InitStructure);
}


static void DS1302_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin 	= IO_PIN;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPU;
	GPIO_Init(DS1302_IO_PORT, &GPIO_InitStructure);
}


static void DS1302SendByte(u8 byte)
{
	u8	i;

	for(i=0x01; i; i<<=1)
	{
		if(byte&i)	DS1302_OUT_H();
		else	DS1302_OUT_L();
		DS1302_CLK_H();
		DelayNOP(50);
		DS1302_CLK_L();
	}
}


static u8 DS1302ReceiveByte(void)
{
	u8	i,byte=0;

	for(i=0x01; i; i<<=1)
	{
		if(DS1302_IN_X)	byte |= i;
		DS1302_CLK_H();
		DelayNOP(50);		
		DS1302_CLK_L();
	}
	return(byte);
}


static void Write1302(u8 addr,u8 data)
{
    DS1302_OUT();
	DS1302_RST_L();
	DS1302_CLK_L();
	DS1302_RST_H();
	DelayNOP(100);
	DS1302SendByte(addr);
	DS1302SendByte(data);
	DelayNOP(100);
	DS1302_RST_L();
}


static u8 Read1302(u8 addr)
{
    u8 data=0;

    DS1302_OUT();
	DS1302_RST_L();
	DS1302_CLK_L();
	DS1302_RST_H();
	DelayNOP(100);
	DS1302SendByte(addr|0x01);
	DS1302_IN();
	data = DS1302ReceiveByte();
	DelayNOP(100);
	DS1302_RST_L();
	return(data);
}


/*
��ȡDS1302�е�RAM
addr:��ַ,��0��30,��31���ֽڵĿռ�
����Ϊ����ȡ������
*/
static u8 ReadDS1302Ram(u8 addr)
{
	u8	tmp,res;

	tmp = (addr<<1)|0xc0;
	res = Read1302(tmp);
	return(res);
}


/*
дDS1302�е�RAM
addr:��ַ,��0��30,��31���ֽڵĿռ�
data:Ҫд������
*/
static void WriteDS1302Ram(u8 addr,u8 data)
{
	u8	tmp;

	Write1302(DS1302_WRITE,0x00);		//�ر�д����
	tmp = (addr<<1)|0xc0;
	Write1302(tmp,data);
	Write1302(DS1302_WRITE,0x80);		//��д����
}


void ReadDSRam(u8 *p,u8 add,u8 cnt)
{
	u8 i;
	
	if(cnt>30) return;
	for(i=0;i<cnt;i++)
	{
		*p = ReadDS1302Ram(add+1+i);
		p++;
	}
}


void WriteDSRam(u8 *p,u8 add,u8 cnt)
{
	u8 i;
	
	if(cnt>30) return;
	for(i=0;i<cnt;i++)
	{
		WriteDS1302Ram(add+1+i,*p++);
	}
}

  
/*
��ʱ�亯��,˳��Ϊ:��������ʱ����
*/
void ReadDS1302Clock(u8 *p)
{
	DS1302_OUT();
	DS1302_RST_L();
	DS1302_CLK_L();
	DS1302_RST_H();
	DelayNOP(100);
	DS1302SendByte(0xbf);			//ͻ��ģʽ
	DS1302_IN();
	p[5] = DS1302ReceiveByte();		//��
	p[4] = DS1302ReceiveByte();		//��
	p[3] = DS1302ReceiveByte();		//ʱ
	p[2] = DS1302ReceiveByte();		//��
	p[1] = DS1302ReceiveByte();		//��
	DS1302ReceiveByte();			//��
	p[0] = DS1302ReceiveByte();		//��
	DS1302ReceiveByte();			//������־�ֽ�
	DelayNOP(100);
	DS1302_RST_L();
}


/*
дʱ�亯��,˳��Ϊ:��������ʱ����
*/
void WriteDS1302Clock(u8 *p)
{
	Write1302(DS1302_WRITE,0x00);		//�ر�д����
	DS1302_OUT();
	DS1302_RST_L();
	DS1302_CLK_L();
	DS1302_RST_H();
	DelayNOP(100);
	DS1302SendByte(0xbe);				//ͻ��ģʽ
	DS1302SendByte(p[5]);				//��
	DS1302SendByte(p[4]);				//��
	DS1302SendByte(p[3]);				//ʱ
	DS1302SendByte(p[2]);				//��
	DS1302SendByte(p[1]);				//��
	DS1302SendByte(0x01);				//�ܣ����ó���һ��û��ʹ��
	DS1302SendByte(p[0]);				//��
	DS1302SendByte(0x80);				//������־�ֽ�
	DelayNOP(100);
	DS1302_RST_L();
}


void InitClock(void)
{
	u8	tmp;

	DS1302_Configuration();
	tmp = ReadDS1302Ram(0);
	if(tmp^0xa5)
	{
		WriteDS1302Ram(0,0xa5);
		Write1302(DS1302_WRITE,0x00);		//�ر�д����
		Write1302(0x90,0x03);				//��ֹ������
		Write1302(DS1302_HOUR,0x00);		//���ó�24Сʱ��
		Write1302(DS1302_SECOND,0x00);		//ʹ��ʱ������
		Write1302(DS1302_WRITE,0x80);		//��д����
	}
}


#endif

