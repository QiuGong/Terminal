#include <stm32f10x.h>
#include "rtthread.h"
#include "cellular.h"
#include "sim900.h"


#if ( SIM900_EN > 0 )
/****************************************
*   rt_cellular_read                    *
*   port to specified device read func  *
*   cellular read ip data               *
****************************************/
rt_size_t rt_cellular_read(void* buffer)
{
    rt_size_t read_len = 0;
    read_len = sim900_readDATA(buffer);
    return read_len;
}


/****************************************
*   rt_cellular_write                   *
*   port to specified device write func *
*   cellular send ip data               *
****************************************/
rt_size_t rt_cellular_write(const void* buffer, rt_size_t size)
{
    rt_size_t   write_len = 0;
    write_len = sim900_sendDATA(buffer, size);
    return write_len;
}


/****************************************
*   rt_cellular_open                    *
*   port to specified device open func  *
****************************************/
static rt_err_t rt_cellular_open(rt_device_t dev, rt_uint16_t oflag)
{
    rt_err_t    err;
    err = rt_sim900_open();
    return err;
}


/****************************************
*   rt_cellular_close                   *
*   port to specified device close func *
****************************************/
static rt_err_t rt_cellular_close(rt_device_t dev)
{
    rt_err_t    err;
    err = rt_sim900_close();
    return err;
}


/****************************************
*   rt_cellular_control                 *
* port to specified device control func *
****************************************/
rt_err_t rt_cellular_control(rt_uint8_t cmd, void *args)
{
    rt_err_t    err;
    err = rt_sim900_control(cmd, args);
    return err;
}


/****************************************
*   rt_cellular_init                    *
*   port to specified device init func  *
****************************************/
rt_err_t rt_cellular_init(void)
{
    rt_err_t    err;
    err = rt_sim900_init();
    return err;
}


/****************************************
*   rt_hw_cellular_init                 *
* port to specified device hw int func  *
****************************************/
void rt_hw_cellular_init(const char* device_name)
{    
	rt_device_t dev = RT_NULL;

    dev = rt_device_find(device_name);
    if (dev != RT_NULL)
    {
    	dev->open = rt_cellular_open; 
		dev->close = rt_cellular_close;    
    }
    else
    {
        rt_kprintf("sim900: can not find device:%s\n", device_name);
    }

	rt_hw_sim900_init(device_name);
}


/****************************************
*   rt_cellular_thread_startup          *
*   port to specified service thread    *
****************************************/
void rt_cellular_thread_startup(void)
{
    rt_sim900_thread_startup();
}

#endif
