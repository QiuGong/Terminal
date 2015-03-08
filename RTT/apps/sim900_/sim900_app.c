#include <board.h>
#include <rtthread.h>
#include "rt_gsmgprs.h"
#include "cellInfo_handle.h"
#include "time.h"
#include "strLib.h"
#include "LOG.h"


ALIGN(RT_ALIGN_SIZE)
const char SERVER_PORT[] = "7100"; 
static Time sysTime;

static rt_uint8_t listener_stack[512];
static rt_uint8_t server_stack[1024];

static struct rt_thread listener_thread;
static struct rt_thread server_thread;

rt_device_t dev;
rt_sem_t newInfo_sem = RT_NULL;
rt_mq_t newTask_mq = RT_NULL;
rt_timer_t taskTimer = RT_NULL;


struct {
    rt_ggm_host host;
	rt_uint8_t connectionValid;	 // boolean
	rt_uint8_t trackValid;		 // boolean
	Time deadLine;
	rt_uint8_t duration;  		 // 1 -> 30minutes
	rt_uint16_t interval; 
} connection;


typedef struct {
    rt_uint8_t type;
	rt_uint8_t index;
} NewTaskNotification;


void refreshTime(void);
void echoEnable(int cmd);
int sendCellInfo(void);
int handleSM(uint8_t sm_index);
void TimerHandler(void *parameter);
void alarmEnable(rt_uint16_t t);
int checkStatus(int stat[]); // return 1 if status changed, and "stat" returns the status < %, vol, csq>;
int checkTimeValid(void);
void handleResCmd(int id, char *cmd);


static void listener_thread_entry(void* parameter)
{
    uint8_t sm_index;
	char phoneNumber[15];
	NewTaskNotification notify;

    while (1)
    {
	    rt_sem_take(newInfo_sem, RT_WAITING_FOREVER);
	    rt_thread_delay( 50 );
		refreshTime();

		/* search key words */
		if(rt_ggm_isSM(&sm_index)){
		    notify.type = 0;
			notify.index = sm_index;		 
		    __CONSOLE("<INFO> New message arrived! Index<%d>\n", sm_index);
			LOG(3, sysTime, 0);
			rt_mq_send(newTask_mq, &notify, sizeof(NewTaskNotification));
	
		} else if(rt_ggm_isRING(phoneNumber)){
		    __CONSOLE("<INFO>: Ring! ( %s )!\n", phoneNumber);
			LOG(4, sysTime, phoneNumber);
			/* hang up */
		}

		rt_ggm_int_bufswitch();
		rt_ggm_int_enable();
    }
}


static void server_thread_entry(void* parameter)
{			
	NewTaskNotification notify;
	rt_err_t ret;
	int res; 

	newTask_mq = rt_mq_create("newTask_mq", sizeof(NewTaskNotification), 10, RT_IPC_FLAG_FIFO);
	newInfo_sem = rt_sem_create("newInfo_sem", 0, RT_IPC_FLAG_FIFO);
	taskTimer = rt_timer_create("task_timer", TimerHandler, RT_NULL, RT_TICK_PER_SECOND, RT_TIMER_FLAG_ONE_SHOT);
									
																	
    /* Init listener thread  */
	ret = rt_thread_init(&listener_thread,
		"listener",
		listener_thread_entry, RT_NULL,
		(rt_uint8_t*)&listener_stack[0], sizeof(listener_stack), 21, 5);
	if (ret == RT_EOK)
	{
        rt_thread_startup(&listener_thread);
	}

    dev = rt_device_find("sim900a");
	if(dev == RT_NULL){
	    __CONSOLE("<ERROR> Cannot find device < sim900a >\n");
		LOG(5, 0, 0);
		while(1);
	}  	
    ret = rt_device_open(dev, 0);
	

	/* init connecting */
    connection.connectionValid = 0;
    connection.interval = 0; 
	connection.trackValid = 0;

	echoEnable(1);
    notify.type = 0;
	notify.index = 4;		 
	rt_mq_send(newTask_mq, &notify, sizeof(NewTaskNotification));  
	
	 		
	/* Main Loop */
    while(1){
	    rt_mq_recv(newTask_mq, &notify, sizeof(NewTaskNotification), RT_WAITING_FOREVER);
		switch(notify.type){

		    /* Short message */
		    case 0: { 
	            res = handleSM(notify.index); 
				if(res != 0)
				    continue;
				if(!connection.connectionValid)
				    continue;
				refreshTime();
				LOG(8, sysTime, 0);					
				sendCellInfo();
				alarmEnable(connection.interval);
			} break;

			/* Alarm */
			case 1: {
			    if(!connection.connectionValid)
				    continue;
				refreshTime();	
				if(-1 == time_compare(sysTime, connection.deadLine)){
				    sendCellInfo();
					alarmEnable(connection.interval);
				} else { 
					connection.connectionValid = 0;
					connection.trackValid = 0;
					__CONSOLE("<INFO> Connection is invalid!");
					LOG(6, sysTime, 0);			 
				}							
			}
		}        		  	
	}					
}


/**
  * @brief  Timer hook
  * @param  None
  * @retval None
  */	    
void TimerHandler(void *parameter)
{
	NewTaskNotification notify;
    notify.type = 1;		 
	rt_mq_send(newTask_mq, &notify, sizeof(NewTaskNotification));
}


/**
  * @brief  Interrupt ISR
  * @param  None
  * @retval None
  */
void rt_gsmgprs_isr(void)
{
    /* wake up listener thread */
	rt_ggm_int_disable();

	rt_sem_release(newInfo_sem);
}


int rt_application_init1()
{
	rt_err_t result;

	/* init server thread */
	result = rt_thread_init(&server_thread,
		"server",
		server_thread_entry, RT_NULL,
		(rt_uint8_t*)&server_stack[0], sizeof(server_stack), 22, 5);
	if (result == RT_EOK)
	{
        rt_thread_startup(&server_thread);
	}
	return 0;
}
	
							   
/****************************************************************************
****************************** USER FUNCTION ********************************
****************************************************************************/
int checkTimeValid(void)
{ 
	if(sysTime[TIME_yy] < 12)
	    return -1;
	else
	    return 0;
}


int checkStatus(int stat[])
{
    int res;
    static int lastStatus[3] = {0, 0, 0};
	
    if(stat == 0)
	    return 0;

    rt_device_control(dev, RT_GGM_CMD_READ | RT_GGM_CSQ, 0);
    rt_device_read(dev, 0, &res, 0);
	stat[2] = res;
	
    rt_device_control(dev, RT_GGM_CMD_READ | RT_GGM_BATTERY, 0);
    rt_device_read(dev, 0, stat, 0);
	
	if(stat[0] != lastStatus[0] || stat[2] != lastStatus[2]){
	    lastStatus[0] = stat[0];
	    lastStatus[1] = stat[1];
		lastStatus[2] = stat[2];
		return 1;
	}	  	  
	return 0;  
}


void refreshTime(void)
{
	rt_device_control(dev, RT_GGM_CMD_READ | RT_GGM_TIME, 0);	  
    rt_device_read(dev, 0, sysTime, 0);      
}


void alarmEnable(rt_uint16_t t)
{
    rt_tick_t tick = t * RT_TICK_PER_SECOND;
    if(t < 10)
	    return;
    rt_timer_control(taskTimer, RT_TIMER_CTRL_SET_TIME, (void *)&tick);
	rt_timer_start(taskTimer);    
}


void echoEnable(int cmd)
{
	rt_uint8_t arg;
    if(cmd)
	    arg = 1;
	else   
	    arg = 0;
    rt_device_control(dev, RT_GGM_CMD_ATE, &arg);  
}


void handleResCmd(int id, char *cmd)
{
    __CONSOLE("<INFO> Handling CMD<%d,\"%s>\"", id, cmd);
	switch(id){
	    case 1: {
		    connection.interval = atoi(cmd);
		} break;
		case 2: {
		    connection.trackValid = atoi(cmd);
			if(connection.trackValid){
			    __CONSOLE("<INFO> Track is enabled!");
				LOG(9, sysTime, 0);
			} else {
			    __CONSOLE("<INFO> Track is disabled!"); 
				LOG(10, sysTime, 0);
			}
		} break;
		case 3: {
		    connection.connectionValid = atoi(cmd);
			if(connection.connectionValid == 0){
			    connection.trackValid = 0; 
				__CONSOLE("<INFO> Connection is invalid!");	
				LOG(6, sysTime, 0);  
			}
		} break;
		case 4: {
		     rt_device_control(dev, RT_GGM_CMD_TIME_SET, cmd);
			 __CONSOLE("<INFO> Update system time: %s", cmd);
			 LOG(11, sysTime, cmd);
		     // update deadline
			 time_copy(connection.deadLine, sysTime);
			connection.deadLine[TIME_mm] += 30 * (connection.duration & 0x1);
			if(connection.deadLine[TIME_mm] >= 60){
			    connection.deadLine[TIME_mm] -= 60;
				connection.deadLine[TIME_hh] += 1;
			}    
			connection.deadLine[TIME_hh] += connection.duration >> 1;
			if(connection.deadLine[TIME_hh] >= 24){
				connection.deadLine[TIME_hh] = 23;
			} 
		} break;
	}
}


int sendCellInfo(void)
{
    char *k, *p, *q;
    int stat[3];
	rt_cellsInfo cells;
    char tStr[10];
	int res;
	char *JSON_String;
	static rt_uint8_t hasBufferedData = 0;		  

 	rt_device_control(dev, RT_GGM_CMD_READ | RT_GGM_CENG , 0);
    res = rt_device_read(dev, 0, (void *)&cells, 0);

    if(res == 0){
        __CONSOLE("<ERROR> Failed to read the cells infomation!\n");
		LOG(12, sysTime, 0);
        return -1;	    
	}
	
    time_print_withoutDate(sysTime, tStr);
    strCpy(cells.time, tStr); 	

    if( RT_ERROR != rt_device_control(dev, RT_GGM_CMD_TCP_CLIENT, &connection.host)){
	    __CONSOLE("<INFO> Connect to server!\n");
		//LOG(13, sysTime, 0);     
		if(connection.trackValid){
			if(!hasBufferedData)
		        newCellInfoPackage(0);
		    else
			    hasBufferedData = 0;
	
		    addCellInfo(&cells);
	
	        if(checkStatus(stat))
			    addStatus(1, stat);
			else
			    addStatus(0, stat);
	
	        if(0 != checkTimeValid())
			    addTimeValid(0);
			else
			    addTimeValid(1); 
	
		   	JSON_String = printCellInfoPackage();
	
			if(JSON_String != 0) 
			    rt_free(JSON_String);
		    else {
			    __CONSOLE("<ERROR>JSSON_String is null!\n");
				LOG(14, sysTime, 0); 
				return -1;
			}  	   
	    } else { 
		    newCellInfoPackage(1);
			JSON_String = printCellInfoPackage();

			if(0 != checkTimeValid())
			    addTimeValid(0);
			else
			    addTimeValid(1);
		}

	    echoEnable(0);        	  
		rt_device_control(dev, RT_GGM_CMD_WRITE | RT_GGM_GPRS, 0);
	    res = rt_device_write(dev, 0, JSON_String, 0);
		echoEnable(1);
   
		k = (char *)rt_malloc(40);
		if(k != 0){
	        rt_device_control(dev, RT_GGM_CMD_READ | RT_GGM_RES, 0);
            rt_device_read(dev, 0, k, 0);
		    p = strStr(k, "<");
			if(p != 0){	  //receive response from android 
			    p += 1;
			    while(*p != '>'){
				    q = strStr(p, ",");
				    *q = '\0';
				    res = atoi(p);
					p = q + 2;
					q = strStr(p, "\"");
					*q = '\0';
					handleResCmd(res, p);
					p = q + 1;
					putchar(*p);
				}      
			}    
		    rt_free(k);
		}			
		
		if(res != 0) {
			return 0;
		} else {
		    __CONSOLE("<ERROR> Failed to send data!\n");
			LOG(15, sysTime, 0);
			return -1;
		}
	} else {
	    /* Cannot establish the socket, buffer the cellInfo */
		if(!connection.trackValid)
		    return 0;
		if(!hasBufferedData){
	        newCellInfoPackage(0);
			hasBufferedData = 1;
		}
		__CONSOLE("<WARNING> Cannot connect to server, try to buffer the cells info...\n");
		LOG(16, sysTime, 0);
		res = addCellInfo(&cells);	 
        if(res == 0){	    	
			return 0;
		} else {
	    	__CONSOLE("<ERROR> Failed to buffer cells info, discard!\n");
			LOG(17, sysTime, 0);
			return -1;		    
		}
	}		  
}


int handleSM(uint8_t sm_index)
{
    char *p, *q;
	rt_shortMsg msg;
	rt_ggm_rd arg;
	int res;

	arg.sm.index = sm_index;
	arg.sm.mode = 1;	
	rt_device_control(dev, RT_GGM_CMD_READ | RT_GGM_SM , &arg);
	rt_device_read(dev, 0, (void *)&msg, 0);

	__CONSOLE("%s\n", msg.data);
	p = strStr((char *)(msg.data), "IL,");
	if(p == 0){
	    __CONSOLE("<ERROR> Cannot parase the short message. No prefix \"IL\"!\n");
		LOG(18, sysTime, 0);
	    return -1;
	}
	p += 4;
	q = p;
	q = strStr(q, "\",\"");
	if(q == 0){
	    __CONSOLE("<ERROR> Cannot parase the short message. Format error!\n");
		LOG(19, sysTime, 0);
	    return -1;
	}
	*q = '\0';
	strCpy(connection.host.ip, p);
	strCpy(connection.host.port, SERVER_PORT);
	q += 3;
	p = q;
	while(isdigit(*q)) q++;
	*q = '\0';
	
	res = atoi(p);
	connection.duration =  res;
	
	refreshTime();
	time_copy(connection.deadLine, sysTime);
	connection.deadLine[TIME_mm] += 30 * (res & 0x1);
	if(connection.deadLine[TIME_mm] >= 60){
	    connection.deadLine[TIME_mm] -= 60;
		connection.deadLine[TIME_hh] += 1;
	}    
	connection.deadLine[TIME_hh] += res >> 1;
	if(connection.deadLine[TIME_hh] >= 24){
		connection.deadLine[TIME_hh] = 23;
	} 

	connection.connectionValid = 1; 

	q += 3;
	p = q;
	while(isdigit(*q)) q++;
	*q = '\0';
	res = atoi(p);

    ///connection.interval = res;	 
	 connection.interval = 20;
	/* delete this message */	
	return 0;   	    								    
}

