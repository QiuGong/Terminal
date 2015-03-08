#ifndef __RT_GSMGPRS_H
#define __RT_GSMGPRS_H

#include <rtthread.h>
#include "sim900_driver.h"

#define GGM_DEVICE_NAME "sim900a"

#define GGM_INTBUF_SIZE 50
#define GGM_RDBUF_SIZE 256


/* Short Message Structure */
typedef sim900_shortMsg rt_shortMsg;
/* AT+CSQ response structure */
typedef struct rt_uint8_t sim900_csq; 
/* AT+CMGL response structure */
/* ... */
/* AT+CENG response structure */
typedef struct{
    char time[9]; // hh:mm:ss
    cellInfo array[7];
}rt_cellsInfo;


/* GSM/GPRS Module control commands */
/* part 1: bit [3:0] */
#define RT_GGM_CMD_AT 0
#define RT_GGM_CMD_WRITE 1
#define RT_GGM_CMD_READ 2
#define RT_GGM_CMD_TCPSERVER 3
#define RT_GGM_CMD_CMGF 4
#define RT_GGM_CMD_ATE 5
#define RT_GGM_CMD_TCP_CLOSE 6
#define RT_GGM_CMD_TCP_CLIENT 7
#define RT_GGM_CMD_TCP_SHUT 8
#define RT_GGM_CMD_TIME_SET 10 
/* part2: bit [7:4] */
#define RT_GGM_GPRS 0x00
#define RT_GGM_SM 0x10
#define RT_GGM_CENG 0x20
#define RT_GGM_CSQ 0x30
#define RT_GGM_CMGL 0x40 
#define RT_GGM_TIME 0x50
#define RT_GGM_BATTERY 0x60
#define RT_GGM_RES 0x70


/* Control commands arg types */ 
/* write & read short message arguments */
typedef struct {
    char da[SM_DA_LENGTH_MAX];
    rt_uint8_t mode;             
} wrSM_arg; 
typedef struct {
    rt_uint8_t index;
    rt_uint8_t mode;             
} rdSM_arg;


/* write & read arguments */
typedef union {
    wrSM_arg sm;    
} rt_ggm_wr;
typedef union {
    rdSM_arg sm;
} rt_ggm_rd;


/* establish tcp client arguments */ 
typedef struct {
    char ip[16];
	char port[6];
} rt_ggm_host; 
				

/* ggm device data */
typedef struct {
    rt_uint8_t wr_mode;
	rt_uint8_t rd_mode; 
    rt_ggm_wr wr_setting;
    rt_ggm_rd rd_setting;
} rt_ggm_private;


rt_err_t rt_ggm_register(void);
void rt_ggm_int_enable(void);
void rt_ggm_int_disable(void);
void rt_ggm_int_bufswitch(void);
int  rt_ggm_isSM(rt_uint8_t *buf); /* return sm index */
int  rt_ggm_isRING(char *buf);	/* return phone number */ 
int  rt_ggm_waitForCLOSED(rt_uint8_t timeover);
#endif

