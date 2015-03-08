#ifndef __SIM900_H
#define __SIM900_H

#ifndef NULL
    #define NULL 0
#endif

#include "stm32f10x.h"
#include "time.h"

#define SM_LENGTH_MAX 160
#define SM_OA_LENGTH_MAX 15
#define SM_DA_LENGTH_MAX SM_OA_LENGTH_MAX

#define SIM900_PRINT_RESPONSE

#define PWR_KEY_PORT GPIOB
#define PWR_KEY_PIN  GPIO_Pin_6


/* short message */
typedef struct {
    char oa[SM_OA_LENGTH_MAX];
    uint8_t data[SM_LENGTH_MAX];
	uint16_t length;
	char time[21];  // eg. 12/02/14,20:16:29+32 
} sim900_shortMsg;


/* cell info */
typedef struct {
	char lac[5];	  //lac: 0x0000 - 0xFFFF
	char cellid[5];   
	uint8_t rxl;
} cellInfo;

void sim900_pwrKey_pulse(void);

void sim900_hw_init(void);

void sim900_buf_switch(void);
void sim900_int_enable(void);
void sim900_int_disable(void);
int  sim900_checkBuf_isSM(uint8_t *index); /* arg returns sm index */
int  sim900_checkBuf_isRING(char *phoneNum);	/* arg returns phone number */

void sim900_AT(void);
void sim900_ATE0(void);
void sim900_ATE1(void);
void sim900_AT_CSQ(void);
void sim900_AT_CMGR(uint8_t index);
void sim900_AT_CMGF(uint8_t mode); /* mode: 0-1 */
void sim900_AT_CMGS(char da[], uint8_t mode, char text[]); /* da: phone number, mode: 0-1 */ 
void sim900_AT_CMGD(uint8_t index);
void sim900_AT_CENG_WR(uint8_t mode); /* mode: 0 - 3 */
void sim900_AT_CENG_RD(void);
void sim900_AT_CIFSR(void);
void sim900_AT_CIPSEND(void);
void sim900_AT_CIPCLOSE(void); 
void sim900_AT_CIPSHUT(void); 
void sim900_AT_CIPSTATUS(void);
void sim900_AT_CIPSTART(const char ip[], const char port[]);
void sim900_AT_CCLK_RD(void);
void sim900_AT_CCLK_WR(const char *time); 
void sim900_AT_CBC(void);

int  sim900_module_config(void);  /*  at+cnmi=2,1,0,0,0  at+ceng=3 and call sim900_tcp_init */
int  sim900_ate(uint8_t cmd);
int  sim900_SM_read(uint8_t index, uint8_t mode, sim900_shortMsg *smPtr); /* da: phone number, mode: 0-1 */ 
int  sim900_test_AT(void);
int  sim900_config_cmgf(uint8_t mode);
int  sim900_csq_read(int *res);
int  sim900_ceng_read(cellInfo buf[]);
int  sim900_tcp_client(const char ip[], const char port[]);
int  sim900_tcp_close(void);
int  sim900_tcp_shut(void);
int  sim900_tcp_write(const char buf[], char rec[]);  /* before call this function, ATE0 is prefered to be executed */ 
int  sim900_time_set(char tStr[]);
int  sim900_time_read(Time t);
int  sim900_battery_read(int battery[]);
int  sim900_waitfor_CLOSED(uint8_t timeover);
#endif

