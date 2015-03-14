#include "util.h"


static double TEMP_K = 0.0625;
rt_uint16_t formula_temp_16(rt_uint16_t source)
{
	return source * TEMP_K ;
}

rt_uint16_t formula_temp_8(rt_uint8_t source)
{
	return source / TEMP_K;
}
 
  
/** 
 * 1	685	4.9
 * 2	681	5.2
 * 3	670	5.6
 * 4	625	7.0
 * 5	617	7.1
 * 6	610	7.4
 * (E1-E2)/(PH1-PH2)=K
*/
static rt_uint16_t 	E1 = 618;
static rt_uint16_t 	PH = 69;
static rt_uint16_t	PH_K = 35200;
rt_uint16_t formula_ph_16(rt_uint16_t source)
{	
	return (((E1 - source) / (PH_K/1000.0)) + (PH/10.0)) * 10;
}

rt_uint16_t formula_ph_8(rt_uint8_t source)
{
	return E1 + (PH_K/1000.0) * ((PH/10.0) - source);
}									  


#ifdef RT_USING_FINSH
#include <finsh.h>
void set_formula_ph(rt_uint16_t e1, rt_uint16_t ph, rt_uint16_t ph_k)
{
    E1 = e1;
    PH = ph;
	PH_K = ph_k;
}
FINSH_FUNCTION_EXPORT(set_formula_ph, e.g: set_formula_ph(625, 70, 30000))

void get_formula_ph(void)
{
	rt_kprintf("E1=%d, PH=%d, PH_K=%d\n", E1, PH, PH_K);
}
FINSH_FUNCTION_EXPORT(get_formula_ph, e.g: get_formula_ph())
#endif


/**
 * 1	423	9.92（空气中饱和值）
 * 2	254	6.11
 * 3	250	5.90
 * 4	240	5.61
 * 5	234	5.35
 * 6	224	5.19
 *
 * 计算公式：Y=KX
 */
static rt_uint16_t 	DO_K = 248;
rt_uint16_t formula_do_16(rt_uint16_t source)
{	
	return (source * (DO_K/1000.0)) + 0.5;	  
}

rt_uint16_t formula_do_8(rt_uint8_t source)
{
	return (source * 10) / (DO_K/1000.0);
}


#ifdef RT_USING_FINSH
#include <finsh.h>
void set_formula_do(rt_uint16_t do_k)
{
    DO_K = do_k;
}
FINSH_FUNCTION_EXPORT(set_formula_do, e.g: set_formula_do(233))

void get_formula_do(void)
{
	rt_kprintf("DO_K=%d\n", DO_K);
}
FINSH_FUNCTION_EXPORT(get_formula_do, e.g: get_formula_do())
#endif


rt_uint8_t formula_relate_16(rt_uint16_t source)
{
	return (rt_uint8_t)source;
}

rt_uint16_t formula_relate_8(rt_uint8_t source)
{
	return (rt_uint16_t)source;
}
