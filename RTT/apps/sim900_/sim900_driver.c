#include "stm32f10x.h"
#include "sim900_driver.h"
#include "strLib.h"
#include <rtthread.h>
#include "LOG.h"


#define REC_BUF_SIZE 300
#define BUF_SIZE 100

#define DELAY_WEIGHT 25


static void sendCmd(char str[]);
static void sendData(uint8_t* buf, uint16_t num);
static void delay(uint16_t t);
static void rec_bufEnable(void);
static void rec_bufDisable(uint8_t echo);
static void buf_clear(uint8_t *p, const int size);
static int sim900_tcp_init(void);

static  uint8_t echo_cmd = 1;
static 	DMA_InitTypeDef DMA_InitStructure;

static uint8_t recBuf[REC_BUF_SIZE];
static uint8_t buf[2][BUF_SIZE];
static uint8_t cur_bufIndex = 0;
static uint8_t recBuf_isclosed = 1;


void sim900_int_enable(void)
{
    if(recBuf_isclosed)
        USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}


void sim900_int_disable(void)
{
    if(recBuf_isclosed)
        USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
}


/**
  * @brief  buf return sm index 
  * @param  None 
  * @retval 0 if is not SM
  */
int  sim900_checkBuf_isSM(uint8_t *index)
{
    char *p, *q;	
	char tmp[] = {0x0d, 0x00};

	p = strStr((char *)buf[cur_bufIndex], "+CMTI:");
	if(p == 0)
	    return 0;

	p = strStr(p, "SM");
	if(p == 0)
	    return 0;	
	
	p += 4;
	q = strStr(p, tmp);
    *q = '\0';
	
	*index = atol(p);
	
	return 1;	     
}


/**
  * @brief  buf return phone number 
  * @param  None 
  * @retval None
  */
int  sim900_checkBuf_isRING(char *phoneNum)
{
    char *p, *q;	

	p = strStr((char *)buf[cur_bufIndex], "RING");
	if(p == 0)
	    return 0;

	p = strStr(p, "\"");
	if(p == 0)
	    return 0;

    p++;
	
	q = strStr(p, "\"");
	*q = '\0';
	
	strCpy(phoneNum, p);		

	return 1;
}


/**
  * @brief  switch buffer 0 - 1 
  * @param  None 
  * @retval None
  */
void sim900_buf_switch(void)
{
    uint8_t next_bufIndex;

    if(recBuf_isclosed == 0)
	    return;

	next_bufIndex = cur_bufIndex == 0? 1: 0;
	
	buf_clear(buf[next_bufIndex], BUF_SIZE);
	 
    DMA_Cmd(DMA1_Channel6, DISABLE);
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)buf[next_bufIndex];
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel6, ENABLE);

	cur_bufIndex = next_bufIndex;
}


/**
  * @brief  write '\0' 
  * @param  None 
  * @retval None
  */
void buf_clear(uint8_t *p, const int size)
{
    int i;
	
	for(i = 0; i < size; i ++){
	    *(p+i) = '\0';
	}    
}


/**
  * @brief  Enable DMA 
  * @param  None 
  * @retval None
  */
static void rec_bufEnable(void)
{
    buf_clear(recBuf, REC_BUF_SIZE);

    DMA_Cmd(DMA1_Channel6, DISABLE);
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)recBuf;
	DMA_InitStructure.DMA_BufferSize = REC_BUF_SIZE;
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel6, ENABLE);	

	USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);	
	recBuf_isclosed = 0;       
}


/**
  * @brief  Disable DMA 
  * @param  None 
  * @retval None
  */
static void rec_bufDisable(uint8_t echo)
{
    DMA_Cmd(DMA1_Channel6, DISABLE);
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)buf[cur_bufIndex];
	DMA_InitStructure.DMA_BufferSize = BUF_SIZE;
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel6, ENABLE);	 

	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET){
    	//clear ORE
		USART_GetITStatus(USART2, USART_IT_ORE);
		USART_ReceiveData(USART2); 
	}
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	
	recBuf_isclosed = 1;
	 
#ifdef SIM900_PRINT_RESPONSE
    #ifdef RT_USING_CONSOLE
	    if(echo)
            __CONSOLE((char *)recBuf);
	#endif
#endif    
}


/**
  * @brief  Delay
  * @param  time 
  * @retval None
  */
static void delay(uint16_t t)
{
    rt_thread_delay(t * DELAY_WEIGHT);
}


/**
  * @brief  Send Command to Serial
  * @param  cmd str 
  * @retval None
  */
static void sendCmd(char str[])
{
    int i;
	for(i = 0; str[i] != '\0'; i++){
	    USART_SendData(USART2, str[i]);
	        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	    
	}
	USART_SendData(USART2, 0x0D);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);    
}


/**
  * @brief  Send data to Serial
  * @param  buf, num 
  * @retval None
  */
static void sendData(uint8_t* buf, uint16_t num)
{
    int i;
	for(i = 0; i<num; i++){
	    USART_SendData(USART2, buf[i]);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	    
	}    
}


/**						   |-----|
  * @brief  PWR KEY:  _____|	 |______
  * @param  None 
  * @retval None
  */
void sim900_pwrKey_pulse(void)
{
	GPIO_ResetBits(PWR_KEY_PORT , PWR_KEY_PIN);
    delay(10);
 	GPIO_SetBits(PWR_KEY_PORT , PWR_KEY_PIN);
    delay(15);
	GPIO_ResetBits(PWR_KEY_PORT , PWR_KEY_PIN);
    delay(20);
}


/**
  * @brief  Initialize USART, DMA and Interrupt
  * @param  None 
  * @retval None
  */
void sim900_hw_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enbale usart2 */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

    /*
    *  USART1_TX -> PA2 , USART1_RX ->	PA3
    */				
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);		   
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART2, &USART_InitStructure); 
	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                             
    DMA_DeInit(DMA1_Channel6);                                                                             
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(USART2->DR));               
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)recBuf;                
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                                   
    DMA_InitStructure.DMA_BufferSize = REC_BUF_SIZE;                                        
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;             
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                       
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;        
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;           
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                              
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                          
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                               
    DMA_Init(DMA1_Channel6, &DMA_InitStructure);    
	USART_DMACmd(USART2,USART_DMAReq_Rx, ENABLE);
	

	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);  

	/* PWR KEY */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; ; 
    GPIO_Init(GPIOB, &GPIO_InitStructure);	 
}


/**
  * @brief  Ecxcute cmd "AT"
  * @param  None 
  * @retval None
  */
void sim900_AT(void)
{
    char s[] = "AT";
    sendCmd(s);        
}


/**
  * @brief  Ecxcute cmd "ATE0" && "ATE1"
  * @param  None 
  * @retval None
  */
void sim900_ATE0(void)
{
    char s[] = "ATE0";
    sendCmd(s);    
}

void sim900_ATE1(void)
{
    char s[] = "ATE1";
    sendCmd(s);    
}


/**
  * @brief  Ecxcute cmd "AT+CSQ"
  * @param  None 
  * @retval None
  */
void sim900_AT_CSQ(void)
{
    char s[] = "AT+CSQ";
    sendCmd(s);
}


/**
  * @brief  Ecxcute cmd "AT+CREG"
  * @param  None 
  * @retval None
  */
void sim900_AT_CREG(void)
{
    char s[] = "AT+CREG?";
    sendCmd(s);
}


/**
  * @brief  Ecxcute cmd "AT+CMGR"
  * @param  index 
  * @retval None
  */
void sim900_AT_CMGR(uint8_t index)
{
    char s[20] = "AT+CMGR=";
	char strTmp[10];

    itoa(index, strTmp, 10);
	strCat(s, strTmp, 20);
    sendCmd(s); 
}


/**
  * @brief  Ecxcute cmd "AT+CMGD"
  * @param  index 
  * @retval None
  */
void sim900_AT_CMGD(uint8_t index)
{
    char s[20] = "AT+CMGD=";
	char strTmp[10];

    itoa(index, strTmp, 10);
	strCat(s, strTmp, 20);
    sendCmd(s);     
}


/**
  * @brief  Ecxcute cmd "AT+CMGF"
  * @param  mode: 0-1  
  * @retval None
  */
void sim900_AT_CMGF(uint8_t mode)
{
    char s[] = "AT+CMGF=X";	
	s[8] = '0' + mode;
    sendCmd(s);  
} 


/**
  * @brief  Ecxcute cmd "AT+CMGS"
  * @param  da: phone number, content 
  * @retval None
  */
void sim900_AT_CMGS(char da[], uint8_t mode, char text[])
{
    int i;

    char s[30] = "AT+CMGS=\"";
	
	if(mode == 1){	/* TEXT */				   
		strCat(s, da, 30);
		strCat(s, "\"", 30);  
	    sendCmd(s); 
		
		delay(2);
		i = strLen(text);
		sendData((uint8_t *)text, i);
		
		s[0] = 0x1a;
		s[1] = '\0';
		sendCmd(s);
	} else {   /* PDU */	
	}    
}


/**
  * @brief  Ecxcute cmd "AT+CENG"
  * @param  mode: 0-3 
  * @retval None
  */
void sim900_AT_CENG_WR(uint8_t mode)
{
    char s[] = "AT+CENG=X";	
	s[8] = '0' + mode;
    sendCmd(s);        
}


/**
  * @brief  Ecxcute cmd "AT+CENG"
  * @param  None
  * @retval None
  */
void sim900_AT_CENG_RD(void)
{
    char s[] = "AT+CENG?";	
    sendCmd(s);    
}


/**
  * @brief  Ecxcute cmd "AT+CIFSR"
  * @param  None
  * @retval None
  */
void sim900_AT_CIFSR(void)
{
    char s[] = "AT+CIFSR";
	sendCmd(s);
}


/**
  * @brief  Ecxcute cmd "CIPSHUT"
  * @param  None
  * @retval None
  */
void sim900_AT_CIPSHUT(void)
{
    char s[] = "AT+CIPSHUT";
	sendCmd(s);    
}


/**
  * @brief  Ecxcute cmd "CIPCLOSE"
  * @param  None
  * @retval None
  */
void sim900_AT_CIPCLOSE(void)
{
    char s[] = "AT+CIPCLOSE=1";
	sendCmd(s);     
}


/**
  * @brief  Ecxcute cmd "CIPSEND"
  * @param  None
  * @retval None
  */   
void sim900_AT_CIPSEND(void)
{
    char s[] = "AT+CIPSEND";
	sendCmd(s);        
}


/**
  * @brief  Ecxcute cmd "CIPSTATUS"
  * @param  None
  * @retval None
  */ 
void sim900_AT_CIPSTATUS(void)
{
    char s[] = "AT+CIPSTATUS";

	sendCmd(s);
}


/**
  * @brief  Ecxcute cmd "CIPSTART"
  * @param  ip, port
  * @retval None
  */ 
void sim900_AT_CIPSTART(const char ip[], const char port[])
{
    char s[40] = "AT+CIPSTART=\"TCP\",\"";
	
	strCat(s, ip, sizeof(s));
	strCat(s, "\",\"", sizeof(s));
	strCat(s, port, sizeof(s));
	strCat(s, "\"", sizeof(s));
	
	sendCmd(s);    
}


/**
  * @brief  Read cmd "CCLK"
  * @param  None
  * @retval None
  */
void sim900_AT_CCLK_RD()
{
    char s[]="AT+CCLK?";
	sendCmd(s);
}


/**
  * @brief  Write cmd "CCLK"
  * @param  time    format: yy/mm/dd,hh:mm:ss+zz 
  * @retval None
  */
void sim900_AT_CCLK_WR(const char *time)
{
    char s[30]="AT+CCLK=\"";   
	strCat(s, time, 30);
	strCat(s, "\"", 30); 
	sendCmd(s);
} 


/**
  * @brief  Write cmd "CALA"
  * @param  alarm time
  * @retval None
  */
void sim900_AT_CALA_WR(const char *alarmTime)
{
    char s[30]="AT+CALA=\"";   
	strCat(s, alarmTime, 30);
	strCat(s, "\",1", 30); 
	sendCmd(s);    
}

void sim900_AT_CBC(void)
{
    char s[] = "AT+CBC";
	sendCmd(s);
}


/**
  * @brief  Read short message
  * @param  index, mode, smPtr
  * @retval -1 if error 
  */
int  sim900_SM_read(uint8_t index, uint8_t mode, sim900_shortMsg *smPtr)
{
    char *p1, *p2;

    if(smPtr == 0){ 
	    return -1;
	}
    if(mode ==1){

        rec_bufEnable();
	    sim900_AT_CMGR(index);
    	delay(4);  	
    	rec_bufDisable(echo_cmd);

		if( 0 == strStr((char *)recBuf, "OK")){
		    return -1;
		}
		/* Get OA 	         p1 			 p2
		**         "REC READ","+861510831xxxx","Ilmen"
		*/				           					   
		p1 = strStr((char *)recBuf, "\",\"");
		if(p1 == 0){
		    return -1;
		}
		p1 += 3;
		p2 = strStr(p1, "\",\"");
		if(p2 == 0){
		    return -1;
		}
		*p2 = '\0';
		strCpy(smPtr->oa, p1);

		/* Get Time */
		p1 = p2 + 1;
		p1 = strStr(p1, "\",\"");
		p1 += 3;
		p2 = strStr(p1, "\"");
		*p2 = '\0';
		strCpy(smPtr->time, p1);
	    
		/* Get Text */
		p1 = p2 + 1;
		p2 = strStr(p1, "OK");
		if(p2 == 0){
		    return -1;
		}
		*p2 = '\0';
		strCpy((char *)smPtr->data, p1);
		
		smPtr->length = strLen((char *)smPtr->data);				  	   
	}
	return 0;	    
}


/**
  * @brief  send cmd AT to check if the module is OK
  * @param  None
  * @retval -1 if error 
  */
int  sim900_test_AT(void)
{
    rec_bufEnable();
    sim900_AT();
	delay(1); 
	rec_bufDisable(echo_cmd);
	
	if( 0 == strStr((char *)recBuf, "OK"))
	    return -1;	    
	else
	    return 0;
}


/**
  * @brief  read csq
  * @param  None
  * @retval -1 if error 
  */
int  sim900_csq_read(int *res)
{
    char *p;
    char s[] = "xx"; 

    rec_bufEnable();
    sim900_AT_CSQ();
	delay(2); 
	rec_bufDisable(echo_cmd);
	
	if( 0 == strStr((char *)recBuf, "OK"))
	    return -1;	    
    	
	p = strStr((char *)recBuf, "CSQ:");
    if(p == 0 || res == 0)
	    return -1;

	p += 5;
	s[0] = (*p++);
	s[1] = *p;

    *res = atoi(s);
	return 0;
}


/**
  * @brief  read ceng
  * @param  None
  * @retval -1 if error 
  */
int  sim900_ceng_read(cellInfo buf[])
{
    int cnt;
	char *p, *q;

    rec_bufEnable();
    sim900_AT_CENG_RD();
	delay(4); 
	rec_bufDisable(0);
	
	if( 0 == strStr((char *)recBuf, "OK"))
	    return -1;
    
    p = (char *)recBuf;

    p = strStr(p, "CENG:");
	p += 1;

	for(cnt = 0; cnt < 7; cnt ++){
	    p = strStr(p, "CENG:");
	    if( p == 0)
	        return -1;
		p += 14;
		q = p + 4;
		*q = '\0';
		strCpy(buf[cnt].lac, p);
		p = q + 1;
		q = p + 4;
		*q = '\0'; 
		strCpy(buf[cnt].cellid, p);
		p = q + 4;
		q = p + 2;
		*q = '\0';
		buf[cnt].rxl = atoi(p);
		p = q + 1;			    
	}

	return 0;
}


/**
  * @brief  config cmgf
  * @param  None
  * @retval -1 if error 
  */
int  sim900_config_cmgf(uint8_t mode)
{
    rec_bufEnable();
    sim900_AT_CMGF(mode);
	delay(1); 
	rec_bufDisable(echo_cmd);
	
	if( 0 == strStr((char *)recBuf, "OK"))
	    return -1;

	return 0;
}


/**
  * @brief  init tcp
  * @param  None
  * @retval -1 if error 
  */
static int sim900_tcp_init(void)
{	 
    rt_uint8_t cnt, delayTime;
    sim900_tcp_shut();

	cnt = 4;
	delayTime = 4;
CIPMUX:    
    rec_bufEnable();
    sendCmd("AT+CIPMUX=0");
	delay(delayTime); 
	rec_bufDisable(echo_cmd);
	
	if( 0 == strStr((char *)recBuf, "OK")){
	    if(cnt == 0){		    
		    return -1;
		}
	    delayTime ++;
		cnt --;
	    goto CIPMUX;
	}

	cnt = 4;
	delayTime = 4;
CGDCONT:		
    rec_bufEnable();
    sendCmd("AT+CGDCONT=1,\"IP\",\"CMNET\"");
	delay(delayTime); 
	rec_bufDisable(echo_cmd);
	
	if( 0 == strStr((char *)recBuf, "OK")){
	    if(cnt == 0){		    
		    return -1;
		}
	    delayTime ++;
		cnt --;
	    goto CGDCONT;
	}

	cnt = 4;
	delayTime = 4;
CGATT:		
    rec_bufEnable();
    sendCmd("AT+CGATT=1");
	delay(delayTime); 
	rec_bufDisable(echo_cmd);
	
	if( 0 == strStr((char *)recBuf, "OK")){
	    if(cnt == 0){		    
		    return -1;
		}
	    delayTime ++;
		cnt --;
	    goto CGATT;
	}

	cnt = 4;
	delayTime = 4;
CIPCSGP:		
    rec_bufEnable();
    sendCmd("AT+CIPCSGP=1,\"CMNET\"");
	delay(delayTime); 
	rec_bufDisable(echo_cmd);
	
	if( 0 == strStr((char *)recBuf, "OK")){
	    if(cnt == 0){		    
		    return -1;
		}
	    delayTime ++;
		cnt --;
	    goto CIPCSGP;
	}
		
	return 0;   
}


/**
  * @brief  start tcp connection as client
  * @param  None
  * @retval -1 if error 
  */
int  sim900_tcp_client(const char ip[], const char port[])
{
    int cnt;    

    rec_bufEnable();
    sim900_AT_CIPSTART(ip, port);
	delay(5); 

    cnt = 10;

	while(--cnt){
	    if( 0 != strStr((char *)recBuf, "CONNECT OK")){
		    break;    
		} else if(0 != strStr((char *)recBuf, "ALREADY CONNECT")){
		    sim900_tcp_close();
			return -1;
		}
		delay(4);	
	}
    
    rec_bufDisable(echo_cmd);

	if(cnt == 0)
	    return -1;
	 
	return 0;   
}


/**
  * @brief  close tcp connection 
  * @param  None
  * @retval -1 if error 
  */
int  sim900_tcp_close(void)
{
    rec_bufEnable();
    sim900_AT_CIPCLOSE();
	delay(3); 
	rec_bufDisable(echo_cmd);
	
	if( 0 == strStr((char *)recBuf, "OK"))
	    return -1;

    return 0;
}


/**
  * @brief  write data via tcp 
  * @param  None
  * @retval -1 if error 
  */
int  sim900_tcp_write(const char buf[], char rec[])
{	
    int len;
	int cnt = 0;
    char s[] = "x";
	char *p = RT_NULL;

    rec[0] = '\0';

	rec_bufEnable();
    sim900_AT_CIPSEND();
	delay(2); 	

	if( 0 == strStr((char *)recBuf, ">")){
	    rec_bufDisable(echo_cmd); 
	    return -1;
	}
	
	len = strLen(buf);
	if(len != 0){
	    sendData((uint8_t *)buf, len);
		s[0] = 0x1a;
		sendCmd(s);
	
	    delay(10);
    
	    cnt = 20;
		while(--cnt){
		    p = strStr((char *)recBuf, "SEND OK");
		    if( p != 0 ){
			    break;    
			}
			delay(5);	
		}	
	}	
	rec_bufDisable(echo_cmd);

	if(cnt == 0)
	    return -1;
	
	p += 7;
	strCpy(rec, p);
	rt_kprintf("client write res: %s %d\n", rec, strLen(rec)); /////////////////////////////////////////////
			
	return 0;				       
}


/**
  * @brief  Ecxcute cmd "ATE0" && "ATE1"
  * @param  None 
  * @retval None
  */
int  sim900_ate(uint8_t cmd)
{
    rec_bufEnable();
    if(cmd)
	    sim900_ATE1();
	else
	    sim900_ATE0();
	delay(1); 
	rec_bufDisable(echo_cmd);
	
	if( 0 == strStr((char *)recBuf, "OK"))
	    return -1;

    return 0;    
}


/**
  * @brief  Ecxcute cmd "CIPSHUT"
  * @param  None 
  * @retval None
  */
int  sim900_tcp_shut(void)
{
    rec_bufEnable();
    sim900_AT_CIPSHUT();
	delay(2); 
	rec_bufDisable(echo_cmd);
	
	if( 0 == strStr((char *)recBuf, "OK"))
	    return -1;

    return 0;
}


/**
  * @brief  at+cnmi=2,1,0,0,0  at+ceng=3 and call sim900_tcp_init 
  * @param  None 
  * @retval None
  */
int  sim900_module_config(void)
{
    int res;
    int cnt;
	

	cnt = 10;    

	while(cnt --){
        sim900_test_AT();
    	sim900_test_AT();
    	res = sim900_test_AT();
		if(res == 0)
		    break;
		__CONSOLE("Cannot connect SIM900A, try again! <%d>\n", cnt);
		LOG(0, 0, 0);
		sim900_pwrKey_pulse();
	}
	if(cnt == 0 && res != 0)
	    return -1;


    rec_bufEnable();
    sendCmd("AT+CNMI=2,1,0,0,0");
	delay(3); 
	rec_bufDisable(echo_cmd);
	
	if( 0 == strStr((char *)recBuf, "OK"))
	    return -1;


    rec_bufEnable();
    sendCmd("AT+CREG=0");
	delay(3); 
	rec_bufDisable(echo_cmd);
	
	if( 0 == strStr((char *)recBuf, "OK"))
	    return -1;


    rec_bufEnable();
    sim900_AT_CENG_WR(3);
	delay(3); 
	rec_bufDisable(echo_cmd);
	
	if( 0 == strStr((char *)recBuf, "OK"))
	    return -1;
    

	res = sim900_tcp_init();
	return res;
}


/**
  * @brief  set time
  * @param  None 
  * @retval None
  */							   //  tStr:   yy/MM/dd,HH,mm,ss
int  sim900_time_set(char tStr[])
{
    if(tStr == 0)
	    return -1;


    rec_bufEnable();
    sim900_AT_CCLK_WR(tStr);
	delay(1); 
	rec_bufDisable(echo_cmd);
	
	if( 0 == strStr((char *)recBuf, "OK"))
	    return -1; 
		
	return 0;   
}


/**
  * @brief  get time from SIM900
  * @param  None 
  * @retval None
  */
int  sim900_time_read(Time t)   /*   +CCLK: "00/01/01,00:00:31+00"  */
{
    char *p, *q;

    rec_bufEnable();
    sim900_AT_CCLK_RD();
	delay(1); 
	rec_bufDisable(echo_cmd);
	
	if( 0 == strStr((char *)recBuf, "OK"))
	    return -1; 
		
    p = strStr((char *)recBuf, ": \"");
	if(p == 0)
	    return -1;
	q = strStr(p, "+");
	if(q == 0)
	    return -1;
	p += 3;
	*q = '\0';

	if(0 != time_parase(t, p))
	    return -1;
	
	return 0;		   
}


/**
  * @brief  get battery status		   // +CBC: 0,100,4324
  * @param  None 
  * @retval None
  */
int  sim900_battery_read(int battery[])
{
    char *p, *q;    

    rec_bufEnable();
    sim900_AT_CBC();
	delay(1); 
	rec_bufDisable(echo_cmd); 
	
	if( 0 == strStr((char *)recBuf, "OK"))
	    return -1;
		
	p = (char *)recBuf;
	p = strStr(p, "+CBC");
	p = strStr(p, ",");
	if(p == 0)
	    return -1;
	p += 1;
	q = strStr(p, ","); 
	*q = '\0';
	battery[0] = atoi(p);
	
	q += 1;
	p = q;
	while(isdigit(*q)) q++;
	*q = '\0';
    battery[1] = atoi(p);
	
	return 0;	 
}


/**
  * @brief  wait for CLOSED
  * @param  None 
  * @retval None
  */
int  sim900_waitfor_CLOSED(uint8_t timeover)
{
    char *p;

    rec_bufEnable();
	delay(timeover); 
	rec_bufDisable(echo_cmd);
	
	p = (char *)recBuf;
	p = strStr(p, "CLOSED");
	if(p == 0)
	    return -1;
	return 0;    
}

