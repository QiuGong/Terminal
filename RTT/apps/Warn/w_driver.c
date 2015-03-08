#include <stm32f10x.h>
#include "warn.h"


#if ( WARN_EN > 0 )


#if ( WARN_LED > 0 )

#define WARN_LED_RCC                    	RCC_APB2Periph_GPIOA
#define WARN_LED_GPIO                   	GPIOA
#define WARN_LED_PIN                  	 	(GPIO_Pin_8)

static void warn_led_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(WARN_LED_RCC,ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_InitStructure.GPIO_Pin   = WARN_LED_PIN;
    GPIO_Init(WARN_LED_GPIO, &GPIO_InitStructure);
}


void warn_led_off(void)
{
	GPIO_SetBits(WARN_LED_GPIO, WARN_LED_PIN);
	RT_DEBUG_LOG(DEBUG_WARN, ("warn led off.\n"));
}


void warn_led_on(void)
{
	GPIO_ResetBits(WARN_LED_GPIO, WARN_LED_PIN);
	RT_DEBUG_LOG(DEBUG_WARN, ("warn led on.\n"));
}


rt_uint8_t warn_led_status(void)
{
	return GPIO_ReadInputDataBit(WARN_LED_GPIO, WARN_LED_PIN);
}


#ifdef RT_USING_FINSH
#include <finsh.h>
void warn_led(rt_uint32_t value)
{
    /* set led status */
    switch (value)
    {
    case 0:
        warn_led_off();
        break;
    case 1:
        warn_led_on();
        break;
    default:
        break;
    }
}

FINSH_FUNCTION_EXPORT(warn_led, e.g: warn_led(0) warn_led(1))
#endif	
#endif


#if ( WARN_BELL > 0 )

#define WARN_BELL_RCC                    	RCC_APB2Periph_GPIOC
#define WARN_BELL_GPIO                   	GPIOC
#define WARN_BELL_PIN                  	 	(GPIO_Pin_9)

static void warn_bell_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;


	/* pin */
    RCC_APB2PeriphClockCmd(WARN_BELL_RCC,ENABLE);
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Pin   = WARN_BELL_PIN;
    GPIO_Init(WARN_BELL_GPIO, &GPIO_InitStructure);


	/* TIM4 40ms */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseStructure.TIM_Period 		= 1;
	TIM_TimeBaseStructure.TIM_Prescaler 	= 36000 - 1;	// 1ms
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);	
	/* Clear TIM4 update pending flag */
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);	
	/* TIM IT enable */
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);	
	/* TIM4 enable counter */
	TIM_Cmd(TIM4, DISABLE);


	/* NVIC */
	NVIC_InitStructure.NVIC_IRQChannel 						= TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}


rt_int8_t static bell_status = 1;
void warn_bell_off(void)
{
	bell_status = 1;
	TIM_Cmd(TIM4, DISABLE);
	RT_DEBUG_LOG(DEBUG_WARN, ("warn bell off.\n"));
}


void warn_bell_on(void)
{
	bell_status = 0;
	TIM_Cmd(TIM4, ENABLE);
	RT_DEBUG_LOG(DEBUG_WARN, ("warn bell on.\n"));
}

rt_uint8_t warn_bell_status(void)
{
	return bell_status;
}

void warn_bell_isr(void)
{
	static rt_uint16_t bit = 0;	

	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{		
		if ((bit< 500) && (bit % 2 == 0))
		{
			GPIO_ResetBits(WARN_BELL_GPIO, WARN_BELL_PIN);
		}
		else if ((bit< 500) && (bit % 2 == 1))
		{
			GPIO_SetBits(WARN_BELL_GPIO, WARN_BELL_PIN);
		}
		else if(bit > 1000)
		{
			bit = 0;
		}

		bit ++;			
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}


#ifdef RT_USING_FINSH
#include <finsh.h>
void warn_bell(rt_uint32_t value)
{
    /* set led status */
    switch (value)
    {
    case 0:
        warn_bell_off();
        break;
    case 1:
        warn_bell_on();
        break;
    default:
        break;
    }
}

FINSH_FUNCTION_EXPORT(warn_bell, e.g: warn_bell(0) warn_bell(1))
#endif
#endif


#if ( WARN_POWER_DROP > 0 )

#define WARN_POWER_RCC                    	RCC_APB2Periph_GPIOC
#define WARN_POWER_GPIO                   	GPIOC
#define WARN_POWER_PIN                  	(GPIO_Pin_8)

static void warn_power_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(WARN_BELL_RCC,ENABLE);

    GPIO_InitStructure.GPIO_Pin   = WARN_POWER_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(WARN_POWER_GPIO, &GPIO_InitStructure);
}


rt_uint8_t power_status(void)
{
	return GPIO_ReadInputDataBit(WARN_POWER_GPIO, WARN_POWER_PIN);
}

#endif

void warn_device_init(void)
{
#if ( WARN_LED > 0 )
	warn_led_init();	
	warn_led_off();
#endif

#if ( WARN_BELL > 0 )
	warn_bell_init();
	warn_bell_off();
#endif

#if ( WARN_POWER_DROP > 0 )
	warn_power_init();
#endif
}

#endif
