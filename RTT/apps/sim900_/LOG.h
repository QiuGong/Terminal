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
<item>[ϵͳ����] �޷�����SIM900A���ٴγ���</item>	     0
<item>[ϵͳ��Ϣ] SIM900A��ʼ�����</item>		         1
<item>[ϵͳ����] SIM900A��ʼ��ʧ��</item>		         2
<item>[ϵͳ��Ϣ] �յ��¶���Ϣ</item>			         3
<item>[ϵͳ��Ϣ] ����</item>						     4 
<item>[ϵͳ����] �޷������豸SIM900A</item>			     5
<item>[ϵͳ��Ϣ] �����ѹر�</item>				    	 6
<item>[ϵͳ��Ϣ] �յ�����ָ��</item>				     7
<item>[ϵͳ��Ϣ] �����ѽ���</item>				    	 8
<item>[ϵͳ��Ϣ] ��λ����</item>				    	 9
<item>[ϵͳ��Ϣ] ��λ�ر�</item>				    	10
<item>[ϵͳ��Ϣ] ����ϵͳʱ��</item>				    11
<item>[ϵͳ����] �޷���ȡ��վ��Ϣ</item>		    	12
<item>[ϵͳ��Ϣ] �ɹ����ӵ������</item>		    	13
<item>[ϵͳ����] �޷�����JSON�ַ���</item>		    	14
<item>[ϵͳ����] ����ʧ��</item>				    	15
<item>[ϵͳ����] ���ӷ����ʧ�ܣ���������</item>    	16
<item>[ϵͳ����] ���������������������</item>	    	17
<item>[ϵͳ����] �޷���������Ϣ��ǰ׺У��ʧ��</item>    18
<item>[ϵͳ����] �޷���������Ϣ����ʽ����</item>	    19

***********************************************************************************/

void LOG(int id, Time t, char arg[]);

#endif





