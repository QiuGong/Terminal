#include "warn.h"

#if ( WARN_RELATE > 0 )


/* BELL,LED 置位,开	*/
#define SET			0x03000300 


#define k1(x) 		(x & (0x0001 << (9 - 1)))
#define k2(x) 		(x & (0x0001 << (10 - 1)))
#define k3(x) 		(x & (0x0001 << (11 - 1)))
#define k4(x) 		(x & (0x0001 << (12 - 1)))


void warn_relate(Position p)
{	
	// K1 是否设置关联
	if(p->item.k1_relate != 0x0000)
	{
		// K1状态 关联位状态 都置位，表示相等
		// K1状态 关联位状态 都为0， 表示相等 
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


	// K2 是否设置关联
	if(p->item.k2_relate != 0x0000)
	{
	  	// K2状态 关联位状态 都置位，表示相等
		// K2状态 关联位状态 都为0， 表示相等 
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


	// K3 是否设置关联
	if(p->item.k3_relate != 0x0000)
	{
	  	// K3状态 关联位状态 都置位，表示相等
		// K3状态 关联位状态 都为0， 表示相等 
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


	// K4 是否设置关联
	if(p->item.k4_relate != 0x0000)
	{
	  	// K4状态 关联位状态 都置位，表示相等
		// K4状态 关联位状态 都为0， 表示相等 
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
