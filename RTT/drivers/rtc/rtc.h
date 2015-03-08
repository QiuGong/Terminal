/*
 * File      : rtc.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 */

#ifndef __RTC_H__
#define __RTC_H__

void rt_hw_rtc_init(void);
rt_uint16_t get_hh_mm(void);
void get_time(rt_uint8_t *buf);
void set_rtc(rt_uint8_t year, rt_uint8_t mon, rt_uint8_t day, rt_uint8_t hour, rt_uint8_t min, rt_uint8_t sec);

#endif
