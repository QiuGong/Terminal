#include <rs485.h>

#if ( RS485_EN > 0 )


/* EN485(PA0) */
#define EN_RS485_RCC        	RCC_APB2Periph_GPIOA
#define EN_RS485_GPIO       	GPIOA
#define EN_RS485_PIN        	(GPIO_Pin_0)
#define EN_RS485_DISABLE 		GPIO_SetBits(EN_RS485_GPIO, EN_RS485_PIN);
#define EN_RS485_ENABLE 		GPIO_ResetBits(EN_RS485_GPIO, EN_RS485_PIN);


/* 用于接收消息的消息队列*/
static struct rt_messagequeue rx_mq;
static rt_uint8_t msg_pool[ALL_LEN * MAX_MQ];


/* 接收线程的接收缓冲区*/
static rt_uint8_t rs485_buf[ALL_LEN];
static rt_uint8_t rs485_len;


/* 数据到达回调函数*/
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{	
    rt_uint8_t ch;
	 
	rt_device_read(dev, 0, &ch, size);	
	if(rs485_buf[0] == HEAD)
    {         
		rs485_buf[rs485_len] = ch;                
        if(rs485_len == ALL_LEN - 1)
        {			
            if(rs485_buf[rs485_len] == END)
            {
				rt_mq_send(&rx_mq, &rs485_buf, ALL_LEN);
            }
           
            rs485_buf[0] = 0x00;
        }

        rs485_len ++;
    }
    else if(ch == HEAD)
    {			
		rs485_buf[0] = HEAD;
        rs485_len = 1;
    }

	return RT_EOK;
}


static void init_enable_rs485(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(EN_RS485_RCC, ENABLE);
	
	/* EN485(PA0) */
	GPIO_InitStructure.GPIO_Pin   = EN_RS485_PIN;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;    
    GPIO_Init(EN_RS485_GPIO, &GPIO_InitStructure);	
}


static void parse(rt_uint8_t *b);
void _rs485_rec_thread_entry(void* parameter)
{
	rt_device_t device;
	rt_err_t result = RT_EOK;


	/* 初始化 en_rs485 */
	init_enable_rs485();


	/* 创建消息队列，分配队列存储空间  */
    result = rt_mq_init(&rx_mq, "mq_485", &msg_pool[0], ALL_LEN, ALL_LEN * MAX_MQ, RT_IPC_FLAG_FIFO);  
    if (result != RT_EOK)   
    {   
        rt_kprintf("init mq_485 message queue failed.\n");   
        return;   
    } 


	/* 查找系统中的串口2设备 */
	device = rt_device_find("uart2");
	if (device != RT_NULL)
	{
		rt_device_set_rx_indicate(device, uart_input);
		rt_device_open(device, RT_DEVICE_OFLAG_RDWR);
	}


	while (RT_TRUE)
	{
		rt_uint8_t buf[ALL_LEN];
		result = rt_mq_recv(&rx_mq, &buf, ALL_LEN, RT_WAITING_FOREVER);

		if (result == RT_EOK)
		{			
//			  rt_kprintf("%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X ",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],buf[8],buf[9],
//			  																											  buf[10],buf[11],buf[12],buf[13],buf[14],buf[15],buf[16],buf[17],buf[18]);
			  parse(buf + HEAD_LEN);
		}
	}
}


void re485_send(rt_uint8_t *b, rt_uint8_t len)
{					
	rt_uint8_t i; 
	 
    EN_RS485_DISABLE;  
    for(i = 0; i <= len; i++)
    {
		USART_SendData(USART2, b[i]);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); 
    }   
    EN_RS485_ENABLE;				
}


static void parse(rt_uint8_t *b)
{ 
    if(add_sensor(b) == RT_TRUE)
	{
		refresh_sensor_to_flash();	
	}

	if(b[ID_LEN] == GENERAL_CMD)
    {
	 	x01_respond(b);
    }	
    else if(b[ID_LEN] == SENSOR_1)
    {            
    	x02_respond(b);    
    }
	else if(b[ID_LEN] == SENSOR_2)
    {            
        x03_respond(b);
    }
    else if(b[ID_LEN] == DETECT_K)
    {            
        x04_respond(b);		
    }
	
	rec_last_time(b);	
}

#endif
