#ifndef __SIM900_H
#define __SIM900_H

#include "stm32f10x.h"
#include "rtthread.h"
#include "config.h"

//#ifndef __IP_ADDR_H
//struct ip_addr
//{
//    rt_uint32_t addr;
//};
//typedef struct ip_addr ip_addr_t;

///** Set an IP address given by the four byte-parts.
//    Little-endian version that prevents the use of htonl. */
//#define IP4_ADDR(ipaddr, a,b,c,d) \
//        (ipaddr)->addr = ((rt_uint32_t)((d) & 0xff) << 24) | \
//                         ((rt_uint32_t)((c) & 0xff) << 16) | \
//                         ((rt_uint32_t)((b) & 0xff) << 8)  | \
//                          (rt_uint32_t)((a) & 0xff)
//#endif

//defineS
//sim900 working status mode
//non transparent uart process for <0x10
//transparent uart process for >=0x10
#define INITIAL             0x00	//init
#define STANDBY             0x01	//sim900 standby for anything, CommLineStatus is free
#define IN_CALL             0x02	//sim900 in call mode, CommLineStauts is CALL
#define IN_SMS              0x03	//sim900 in sms mode,
#define IN_GPRS             0x04	//sim900 in gprs mode,
#define IN_ESC              0x05	//sim900 in esc mode of tcp transparent, it is non transparent
#define IN_TCPUDP_NONTRANSP 0x09	//sim900 in non-transparent tcpip mode
#define IN_TCPUDP_TRANSP    0x10	//sim900 in transparent tcpip mode


//sim900 AT cmd response event definition
//MUST < 32
#define OK              (1<<0)
#define CONNECT         (1<<1)
#define RING            (1<<2)  //  incoming call
#define NO_CARRIER      (1<<3)
#define ERROR           (1<<4)
#define NO_DIALTONE     (1<<5)
#define BUSY            (1<<6)  //  call hanged up
#define NO_ANSWER       (1<<7)
#define READY           (1<<8)
#define STATE           (1<<9)
#define CLOSED          (1<<11)
#define CLOSE_OK        (1<<12)
#define CONNECT_OK      (1<<13)
#define CONNECT_FAIL    (1<<14)
#define SEND_OK         (1<<15)
#define SEND_FAIL       (1<<16)
#define SHUT_OK         (1<<17)
#define SERVER_OK       (1<<18)
#define ALREADY_CONNECT (1<<19)
#define PROMOT_MARK     (1<<20) //">"
#define IPD             (1<<21) //ip data in

#define DATA_ACCEPT     (1<<27)
#define DATA_SEND       (1<<28)
#define CME_ERROR       (1<<29)
#define SIG_OK          (1<<30)
#define CALLDONE        (1<<31)

#define recv_ats        1
#define recv_ipdata_len 2
#define recv_ipdata     3

#define SIM900_AT_RESP_MQ_POOL_SIZE	1024
#define SIM900_AT_RESP_MQ_MSG_SIZE	35
#define SIM900_GSM_CONN_TOUT        5
#define SIM900_DATA_TOUT            10
#define SIM900_TCP_CONN_TOUT        20



//sim900 virtual device define
typedef struct rt_sim900_device
{
    rt_device_t 			device;             //phy device


    rt_uint8_t  			CommLineStatus;     //
    rt_int8_t   			signalDB;           //signal quality in DB:-115~-52
    rt_uint8_t				rssi;               //rssi:0~99

    rt_uint8_t  			tcpudp_autoconn;    //auto connection setting

    rt_uint8_t* 			CenterPhoneNumber;  //outgoing phone number[16]
    rt_uint8_t* 			IncomingCallNumber; //incoming call number[16]
    rt_uint8_t* 			conn_type;          //"TCP"/"UDP"[4]

    rt_uint8_t* 			local_addr;         //local ip[16]
    rt_uint8_t* 			remote_addr;        //serevr ip[16]
    rt_uint8_t* 			gw_addr;            //gateway ip[16]
    rt_uint8_t* 			mask_addr;          //mask[16]

    rt_uint8_t* 			local_port;         //local port[6]
    rt_uint8_t* 			remote_port;        //server port[6]
    
    struct rt_semaphore 	rx_semaphore;       //sem for sim900 get a char
    struct rt_semaphore    	frame_sem;           //sem for ip data pack recieved
   
    struct rt_event  		ATResp_event;       //event of modem AT response

    struct rt_messagequeue	AT_resp_MQ;         //msg queue for at response str
    rt_uint8_t*				AT_resp_MQ_poll;    //mem pool of the AT_resp_MQ

}rt_sim900_device;


void rt_sim900_thread_startup(void);
rt_size_t sim900_readDATA(void* buffer);
rt_size_t sim900_sendDATA(const void* buffer, rt_size_t size);

rt_err_t rt_sim900_init(void);
rt_err_t rt_sim900_open(void);
rt_err_t rt_sim900_close(void);

rt_size_t rt_sim900_read(void* buffer);
rt_size_t rt_sim900_write(const void* buffer, rt_size_t size);

rt_err_t rt_sim900_control(rt_uint8_t cmd, void *args);
void rt_hw_sim900_init(const char* device_name);

#endif

