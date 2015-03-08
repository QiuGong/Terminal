#include <display.h>

#if ( DISPLAY_EN > 0 )

/* 用于接收消息的消息队列*/
static struct rt_messagequeue rx_mq;
static rt_uint8_t msg_pool[U_ALL_LEN * MAX_MQ];


/* 接收线程的接收缓冲区*/
static rt_uint8_t rx_buf[U_ALL_LEN];
static rt_uint8_t rx_len;


/* 数据到达回调函数*/
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{	
    rt_uint8_t ch;
	 
	rt_device_read(dev, 0, &ch, size);	
	if(rx_buf[0] == U_HEAD)
    {         
		rx_buf[rx_len] = ch;                
        if(rx_len == U_ALL_LEN - 1)
        {			
            if(rx_buf[rx_len] == U_END)
            {
				rt_mq_send(&rx_mq, &rx_buf, U_ALL_LEN);
            }
           
            rx_buf[0] = 0x00;
        }

        rx_len ++;
    }
    else if(ch == U_HEAD)
    {			
		rx_buf[0] = U_HEAD;
        rx_len = 1;
    }

	return RT_EOK;
}


static void parse(rt_uint8_t *b);
void display_rec_thread_entry(void* parameter)
{
	rt_device_t device;
	rt_err_t result = RT_EOK;


	/* 创建消息队列，分配队列存储空间  */
    result = rt_mq_init(&rx_mq, "mq_dsp", &msg_pool[0], U_ALL_LEN, U_ALL_LEN * MAX_MQ, RT_IPC_FLAG_FIFO);  
    if (result != RT_EOK)   
    {   
        rt_kprintf("init mq_dsp message queue failed.\n");   
        return;   
    } 


	/* 查找系统中的串口3设备 */
	device = rt_device_find("uart3");
	if (device != RT_NULL)
	{
		rt_device_set_rx_indicate(device, uart_input);
		rt_device_open(device, RT_DEVICE_OFLAG_RDWR);
	}


	while (RT_TRUE)
	{
		rt_uint8_t buf[U_ALL_LEN];
		result = rt_mq_recv(&rx_mq, &buf, U_ALL_LEN, RT_WAITING_FOREVER);

		if (result == RT_EOK)
		{			
			parse(buf + U_HEAD_LEN);
		}
	}
}


void display_send(rt_uint8_t *b, rt_uint8_t len)
{					
	rt_uint8_t i; 
	 
    for(i = 0; i < len; i++)
    {
		USART_SendData(USART3, b[i]);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET); 
    }				
}


static void parse(rt_uint8_t *b)
{ 
	if(b[0] == U_TIME_CMD)
	{
		x01_dsp_respond(b + U_CMD_LEN);
	}
	else if(b[0] == U_SENSOR_CMD)
	{
		x02_dsp_respond(b + U_CMD_LEN);
	}
	else if(b[0] == U_QUERY_SENSOR_CMD)
	{
		x03_dsp_respond(b + U_CMD_LEN);
	}
	else if(b[0] == U_QUERY_SENSOR_VALUE_CMD)
	{
		x04_dsp_respond(b + U_CMD_LEN);
	}
	else if(b[0] == U_QUERY_SENSOR_SET_CMD)
	{
		x05_dsp_respond(b + U_CMD_LEN);
	}
	else if(b[0] == U_QUERY_TIMER_CMD)
	{
		x06_dsp_respond(b + U_CMD_LEN);
	}
	else if(b[0] == U_QUERY_TIMER_VALUE_CMD)
	{
		x07_dsp_respond(b + U_CMD_LEN);
	}
	else if(b[0] == U_QUERY_TIMER_SET_CMD)
	{
		x08_dsp_respond(b + U_CMD_LEN);
	}
}
#endif
