#include "ds1302.h"
#include "rtc.h"
#include "strLib.h"
#include <string.h>
#include <time.h>


#if ( DS1302_EN > 0 )

static struct rt_device rtc;
static rt_err_t rt_rtc_open(rt_device_t dev, rt_uint16_t oflag)
{
    if (dev->rx_indicate != RT_NULL)
    {
        /* Open Interrupt */
    }

    return RT_EOK;
}


static rt_size_t rt_rtc_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size)
{
    return 0;
}


static rt_err_t rt_rtc_control(rt_device_t dev, rt_uint8_t cmd, void *args)
{
    RT_ASSERT(dev != RT_NULL);

    switch (cmd)
    {
    case RT_DEVICE_CTRL_RTC_GET_TIME:
			ReadDS1302Clock(args);
        break;

    case RT_DEVICE_CTRL_RTC_SET_TIME:
			WriteDS1302Clock(args);
    	break;
    }

    return RT_EOK;
}


static rt_uint8_t time_buf[6];
static void per_get_time(void* parameter)
{
	rt_device_t device;
	
	memset(time_buf, 0x00, 6);
	device = rt_device_find("rtc");
    if (device != RT_NULL)
    {
        rt_rtc_control(device, RT_DEVICE_CTRL_RTC_GET_TIME, time_buf);
    }
	else
	{
		rt_kprintf("list date error\n");
	}
}


static struct rt_timer get_time_timer;
static void init_per_get_time(void)
{
	rt_timer_init(&get_time_timer, "timer1", /* 定时器名字是 timer1 */
				  per_get_time, 	   	/* 超时时回调的处理函数 */
				  RT_NULL, 			 	/* 超时函数的入口参数 */
				  100, 				 	/* 定时长度，以OS Tick为单位，即100个OS Tick */
				  RT_TIMER_FLAG_SOFT_TIMER | RT_TIMER_FLAG_PERIODIC); /* 周期性定时器 */
	
	rt_timer_start(&get_time_timer);
}


void rt_hw_rtc_init(void)
{
	InitClock();
	
	rtc.type	= RT_Device_Class_RTC;
    rtc.init 	= RT_NULL;
    rtc.open 	= rt_rtc_open;
    rtc.close	= RT_NULL;
    rtc.read 	= rt_rtc_read;
    rtc.write	= RT_NULL;
    rtc.control = rt_rtc_control;
    rtc.user_data = RT_NULL;
    rt_device_register(&rtc, "rtc", RT_DEVICE_FLAG_RDWR);

	init_per_get_time();
}


rt_uint16_t get_hh_mm(void)
{
	return 	(time_buf[3] << 8) | time_buf[4];
}


void get_time(rt_uint8_t *buf)
{
	memcpy(buf, time_buf, 6);
}


#ifdef RT_USING_FINSH
#include <finsh.h>
void set_rtc(rt_uint8_t year, rt_uint8_t mon, rt_uint8_t day, rt_uint8_t hour, rt_uint8_t min, rt_uint8_t sec)
{
   	rt_device_t device;
	rt_uint8_t buf[6];


	buf[0] = hex8_to_bcd8(year);buf[1] = hex8_to_bcd8(mon);buf[2] = hex8_to_bcd8(day);
	buf[3] = hex8_to_bcd8(hour);buf[4] = hex8_to_bcd8(min);buf[5] = hex8_to_bcd8(sec);

    device = rt_device_find("rtc");
    if (device != RT_NULL)
    {
        rt_rtc_control(device, RT_DEVICE_CTRL_RTC_SET_TIME, buf);
    }
	else
	{
		rt_kprintf("set date error\n");
	}
}
FINSH_FUNCTION_EXPORT(set_rtc, e.g: set_rtc(14,10,30,21,13,0))


void list_date(void)
{
	rt_device_t device;
	rt_uint8_t buf[6];
	
	device = rt_device_find("rtc");
    if (device != RT_NULL)
    {
        rt_rtc_control(device, RT_DEVICE_CTRL_RTC_GET_TIME, buf);
		rt_kprintf("time:%02X %02X %02X %02X %02X %02X\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
    }
	else
	{
		rt_kprintf("list date error\n");
	}   
}
FINSH_FUNCTION_EXPORT(list_date, e.g: list_date())
#endif

#endif
