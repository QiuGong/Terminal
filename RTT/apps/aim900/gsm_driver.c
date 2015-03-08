#include <stm32f10x.h>
#include "gsm_driver.h"


#if ( SIM900_EN > 0 )


#if ( LED_GSM_EN > 0 )

#define LIGHT_RCC                    	RCC_APB2Periph_GPIOC
#define LIGHT_GPIO                   	GPIOC
#define LIGHT_PIN                  	 	(GPIO_Pin_13)


void light_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(LIGHT_RCC,ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_InitStructure.GPIO_Pin   = LIGHT_PIN;
    GPIO_Init(LIGHT_GPIO, &GPIO_InitStructure);
}


void light_off(void)
{
	GPIO_SetBits(LIGHT_GPIO, LIGHT_PIN);
}

void light_on(void)
{
	GPIO_ResetBits(LIGHT_GPIO, LIGHT_PIN);
}



#ifdef RT_USING_FINSH
#include <finsh.h>
void led(rt_uint32_t value)
{
    /* set led status */
    switch (value)
    {
    case 0:
        light_off();
        break;
    case 1:
        light_on();
        break;
    default:
        break;
    }
}
FINSH_FUNCTION_EXPORT(led, e.g: led(0) led(1))
#endif

#endif


#define GSM_EN_RCC                    	RCC_APB2Periph_GPIOC
#define GSM_EN_GPIO                   	GPIOC
#define GSM_EN_PIN                  	(GPIO_Pin_3)

#define GSM_DTR_RCC                    	RCC_APB2Periph_GPIOB
#define GSM_DTR_GPIO                   	GPIOB
#define GSM_DTR_PIN                  	(GPIO_Pin_6)

#define GSM_ONOFF_RCC                   RCC_APB2Periph_GPIOB
#define GSM_ONOFF_GPIO                  GPIOB
#define GSM_ONOFF_PIN                  	(GPIO_Pin_7)

void gsm_driver_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(GSM_EN_RCC, ENABLE);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Pin   = GSM_EN_PIN;
    GPIO_Init(GSM_EN_GPIO, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(GSM_DTR_RCC, ENABLE);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Pin   = GSM_DTR_PIN;
    GPIO_Init(GSM_DTR_GPIO, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(GSM_ONOFF_RCC, ENABLE);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Pin   = GSM_ONOFF_PIN;
    GPIO_Init(GSM_ONOFF_GPIO, &GPIO_InitStructure);
}

static void gsm_disable(void)
{
	GPIO_SetBits(LIGHT_GPIO, LIGHT_PIN);
}

static void gsm_enable(void)
{
	GPIO_ResetBits(LIGHT_GPIO, LIGHT_PIN);
}

void gsm_sleep(void)
{
	GPIO_SetBits(LIGHT_GPIO, LIGHT_PIN);
}

static void gsm_wakeup(void)
{
	GPIO_ResetBits(LIGHT_GPIO, LIGHT_PIN);
}

static void gsm_onoff_loose(void)
{
	GPIO_SetBits(LIGHT_GPIO, LIGHT_PIN);
}

static void gsm_onoff_press(void)
{
	GPIO_ResetBits(LIGHT_GPIO, LIGHT_PIN);
}

static void gsm_power_off(void)
{
	gsm_disable();
	rt_thread_delay(50); 
	gsm_enable();
}

static void gsm_power_on(void)
{
	gsm_onoff_press(); 
	rt_thread_delay(150);
	gsm_onoff_loose();
}


void gsm_reset(void)
{
	gsm_wakeup();
	gsm_power_off();
	gsm_power_on();
}
#endif
