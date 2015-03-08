#include <rtthread.h>
#include <stdio.h>
#include "cellular.h"
#include "config.h"


#if ( SIM900_EN > 0 )
//example thread for app read ipdata
static void rt_thread_Cellular_IPdataRead_entry(void* parameter)
{
    rt_uint8_t   buffer[1460];

    while(1)
    {
        rt_memset(&buffer[0], 0, sizeof(&buffer[0]));
        if(rt_cellular_read(&buffer[0])>0)
		{
			rt_kprintf("ip data:%d\r\n", buffer);
		}
    }
}

//example thread for incoming call handleing
static void rt_thread_Cellular_IncomingCal_entry(void* parameter)
{
    while(1)
    {
        //sim900_Call_ComeIn()
		if( rt_cellular_control(WAITFOR_IN_CALL,RT_NULL) == RT_EOK)
        {   
			//pickup:PICKUP_CALL or reject:HANGUP_CALL
            if(rt_cellular_control(HANGUP_CALL, RT_NULL) == RT_EOK)
			{
                //notice: CommLineStatus = IN_CALL
				rt_cellular_control(FINISH_CALL, RT_NULL);
			}
        }
        else
		{
            rt_kprintf("in coming call error!\r\n");
		}
    }
}

//example thread to monitor tcp/udp connection closed by server
static void rt_thread_Cellular_IPclosed_entry(void* parameter)
{
    rt_uint8_t temp = 1;

    while(1)
    {
		//monitor if TCP UDP CONNECTION closed by server
        if (rt_cellular_control(WAITFOR_CONN_CLOSED, RT_NULL) == RT_EOK)
        {
            rt_kprintf("Server Closed TCP/UDP connection!\r\n");
            rt_cellular_control(WAITFOR_CONN_CLOSED, &temp);
            rt_cellular_control(QUERY_AUTO_CONN, &temp);
            if(temp == 1)
            {
                //DO CONNECTION
                if( rt_cellular_control(READYtoCONN, RT_NULL) == RT_EOK )
				{
                    rt_cellular_control(TCPUDP_CONN, RT_NULL);
				}
                else
				{
                    rt_thread_delay(5*RT_TICK_PER_SECOND);
				}
            }
        }
    }
}

/****************************************
*   rt_thread_cellular_app_entry         *
*   a thread for app of sim900          *
*   a serial operation to sim900        *
*   recieve event from other thread     *
*   to do the app ops                   *
*   ops for setup,dailcall,answercall   *
*   sms send and read, and gprs,tcpip   *
*   fault handle for these ops          *
****************************************/
void rt_thread_cellular_app_entry(void* parameter)
{
	rt_thread_t tid;
	rt_uint8_t  rssi; 

    //do gprs & tcp connection
	rt_cellular_init();
    if(rt_cellular_control(READYtoCONN, RT_NULL) != RT_EOK)
    {
        rt_kprintf("not ready for tcp/udp conn\r\n");
    }

	if(rt_cellular_control(TCPUDP_CONN, RT_NULL) != RT_EOK)
    {
        rt_kprintf("tcp/udp connect\r\n");
    }

    tid = rt_thread_create( "inipdata",
                            rt_thread_Cellular_IPdataRead_entry,
                            RT_NULL,
                            2048, 5, 50);
    rt_thread_startup(tid);


    tid = rt_thread_create( "incall",
                            rt_thread_Cellular_IncomingCal_entry,
                            RT_NULL,
                            512, 5, 50);
    rt_thread_startup(tid);


    tid = rt_thread_create( "inclose",
                            rt_thread_Cellular_IPclosed_entry,
                            RT_NULL,
                            512, 5, 50);
    rt_thread_startup(tid);


    while(1)
    {
        //example thread for out going call
//      if(rt_cellular_control(DO_CALL, "10086") == RT_EOK)
//		{
//            rt_cellular_control(FINISH_CALL, RT_NULL);
//		}
//        rt_thread_delay(10*RT_TICK_PER_SECOND);

        //example of read gsm signal quality
        if(rt_cellular_control(DO_SIG_CHECK, &rssi) == RT_EOK)
        {
            rt_kprintf("signal quality: %d\r\n", rssi);
        }
        rt_thread_delay(10*RT_TICK_PER_SECOND);
    }
}


static void rt_init_thread_entry(void *parameter)
{
    rt_thread_t tid;

    rt_hw_cellular_init("uart5");
    rt_cellular_thread_startup();
    
    tid= rt_thread_create(  "cell_app",
                            &rt_thread_cellular_app_entry,
                            RT_NULL,
                            1024, 5, 10);
    if(tid != RT_NULL)
        rt_thread_startup(tid);
}


int rt_gsm_init(void)
{
    rt_thread_startup(rt_thread_create( "init",
                                        rt_init_thread_entry,
                                        RT_NULL,
                                        1024, 4, 10));
    return 0;
}

#endif
