#ifndef __LOG_H
#define __LOG_H

#include <rtthread.h>
#include "time.h"

#ifdef RT_USING_CONSOLE
    #define __CONSOLE(...) rt_kprintf(__VA_ARGS__)
#else 
    #define __CONSOLE(...) ;
#endif

/***********************************************************************************
<item>[系统错误] 无法连接SIM900A，再次尝试</item>	     0
<item>[系统信息] SIM900A初始化完成</item>		         1
<item>[系统错误] SIM900A初始化失败</item>		         2
<item>[系统信息] 收到新短消息</item>			         3
<item>[系统信息] 来电</item>						     4 
<item>[系统错误] 无法发现设备SIM900A</item>			     5
<item>[系统信息] 连接已关闭</item>				    	 6
<item>[系统信息] 收到设置指令</item>				     7
<item>[系统信息] 连接已建立</item>				    	 8
<item>[系统信息] 定位开启</item>				    	 9
<item>[系统信息] 定位关闭</item>				    	10
<item>[系统信息] 更新系统时间</item>				    11
<item>[系统错误] 无法读取基站信息</item>		    	12
<item>[系统信息] 成功连接到服务端</item>		    	13
<item>[系统错误] 无法生成JSON字符串</item>		    	14
<item>[系统错误] 发送失败</item>				    	15
<item>[系统警告] 连接服务端失败，缓存数据</item>    	16
<item>[系统警告] 缓存溢出，丢弃本次数据</item>	    	17
<item>[系统警告] 无法解析短消息，前缀校验失败</item>    18
<item>[系统错误] 无法解析短消息，格式错误</item>	    19

***********************************************************************************/

void LOG(int id, Time t, char arg[]);

#endif





