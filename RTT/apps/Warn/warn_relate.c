#include "warn.h"

#if ( WARN_RELATE > 0 )


/* BELL,LED ��λ,��	*/
#define SET			0x03000300 


#define k1(x) 		(x & (0x0001 << (9 - 1)))
#define k2(x) 		(x & (0x0001 << (10 - 1)))
#define k3(x) 		(x & (0x0001 << (11 - 1)))
#define k4(x) 		(x & (0x0001 << (12 - 1)))


void warn_relate(Position p)
{	
	// K1 �Ƿ����ù���
	if(p->item.k1_relate != 0x0000)
	{
		// K1״̬ ����λ״̬ ����λ����ʾ���
		// K1״̬ ����λ״̬ ��Ϊ0�� ��ʾ��� 
		if((k1(p->item.k) && (p->item.k1_relate & p->item.k)) || 
		   ((k1(p->item.k) == OFF) && ((p->item.k1_relate & p->item.k) == OFF)))
		{
		
		}
		else
		{
			#if ( WARN_EN > 0 )	
				RT_DEBUG_LOG(DEBUG_WARN, ("k1 isn't relate.\n"));
				warn_judge_set(RT_NULL, SET, WHICH_WARN_RELATE);
			#endif	
		}  
	}


	// K2 �Ƿ����ù���
	if(p->item.k2_relate != 0x0000)
	{
	  	// K2״̬ ����λ״̬ ����λ����ʾ���
		// K2״̬ ����λ״̬ ��Ϊ0�� ��ʾ��� 
		if((k2(p->item.k) && (p->item.k2_relate & p->item.k)) || 
		   ((k2(p->item.k) == OFF) && ((p->item.k2_relate & p->item.k) == OFF)))
		{
		
		}
		else
		{
			#if ( WARN_EN > 0 )	
				RT_DEBUG_LOG(DEBUG_WARN, ("k2 isn't relate.\n"));
				warn_judge_set(RT_NULL, SET, WHICH_WARN_RELATE);
			#endif
		}
	}


	// K3 �Ƿ����ù���
	if(p->item.k3_relate != 0x0000)
	{
	  	// K3״̬ ����λ״̬ ����λ����ʾ���
		// K3״̬ ����λ״̬ ��Ϊ0�� ��ʾ��� 
		if((k3(p->item.k) && (p->item.k3_relate & p->item.k)) || 
		   ((k3(p->item.k) == OFF) && ((p->item.k3_relate & p->item.k) == OFF)))
		{
		
		}
		else
		{
			#if ( WARN_EN > 0 )	
				RT_DEBUG_LOG(DEBUG_WARN, ("k3 isn't relate.\n"));
				warn_judge_set(RT_NULL, SET, WHICH_WARN_RELATE);
			#endif
		}
	}


	// K4 �Ƿ����ù���
	if(p->item.k4_relate != 0x0000)
	{
	  	// K4״̬ ����λ״̬ ����λ����ʾ���
		// K4״̬ ����λ״̬ ��Ϊ0�� ��ʾ��� 
		if((k4(p->item.k) && (p->item.k4_relate & p->item.k)) || 
		   ((k4(p->item.k) == OFF) && ((p->item.k4_relate & p->item.k) == OFF)))
		{
		
		}
		else
		{
			#if ( WARN_EN > 0 )	
				RT_DEBUG_LOG(DEBUG_WARN, ("k4 isn't relate.\n"));
				warn_judge_set(RT_NULL, SET, WHICH_WARN_RELATE);
			#endif
		}
	}
}

#endif
