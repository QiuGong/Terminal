#include "warn.h"


#if ( WARN_POWER_DROP > 0 )

/* BELL,LED ÖÃÎ»,¿ª	*/
#define WARN_POWER_DROP_SET	0x03000300 

void warn_power_drop(void)
{	
	if(power_status() == WARN_POWER_OFF)
	{
		#if ( WARN_EN > 0 )	
			RT_DEBUG_LOG(DEBUG_WARN, ("power drop warn.\n"));
			warn_judge_set(RT_NULL, WARN_POWER_DROP_SET, WHICH_WARN_POWER_DROP);
		#endif	
	}
}

#endif
