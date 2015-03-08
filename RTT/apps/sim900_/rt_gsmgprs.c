#include "rt_gsmgprs.h"
#include "time.h"
#include "LOG.h"
#include "strLib.h"

static rt_err_t   rt_gsmgprs_init(rt_device_t dev);
static rt_err_t   rt_gsmgprs_open(rt_device_t dev, rt_uint16_t oflag);
static rt_err_t   rt_gsmgprs_close(rt_device_t dev);
static rt_size_t  rt_gsmgprs_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size);
static rt_size_t  rt_gsmgprs_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size); 
static rt_err_t   rt_gsmgprs_control(rt_device_t dev, rt_uint8_t cmd, void *arg);

static rt_ggm_private private;
static struct rt_device ggm_device;
#define RES_SIZE 50
static char *writeRes = 0;

/**
  * @brief  write 
  * @param  dev, pos, buffer, size
  * @retval err 
  */
static rt_size_t  rt_gsmgprs_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
    rt_ggm_private *ggm;
    rt_ggm_wr *arg = RT_NULL;
    int res;
    rt_size_t ret;

    rt_set_errno(RT_EOK);

	if(dev == RT_NULL || arg == RT_NULL){
	    rt_set_errno(RT_ERROR);
	    return 0;    
	}

	ggm = (rt_ggm_private *)(dev->user_data);
	if(ggm == RT_NULL){
	    rt_set_errno(RT_ERROR);
	    return 0;    
	}

	arg = &(ggm->wr_setting);
	switch(ggm->wr_mode){
	    case RT_GGM_GPRS:{
			ret = rt_strlen((char *)buffer);
		    if(ret == 0){
	            rt_set_errno(RT_ERROR);
	            return 0;
            }
		    res = sim900_tcp_write((char *)buffer, writeRes);
		    if(res != 0){
	            rt_set_errno(RT_ERROR);
	            return 0;
            }			            
		} break;
		default: 
		    rt_set_errno(RT_ERROR);
		    return 0;
	}
	return ret;
}
/**
  * @brief  read
  * @param  dev, pos, buffer, size
  * @retval err 
  */
static rt_size_t  rt_gsmgprs_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    rt_ggm_private *ggm;
    rt_ggm_rd *arg = RT_NULL;
    int res;
    rt_size_t ret;

    rt_set_errno(RT_EOK);

	if(dev == RT_NULL || arg == RT_NULL){
	    rt_set_errno(RT_ERROR);
	    return 0;    
	}

	ggm = (rt_ggm_private *)(dev->user_data);
	if(ggm == RT_NULL){
	    rt_set_errno(RT_ERROR);
	    return 0;    
	}

	arg = &(ggm->rd_setting);
	switch(ggm->rd_mode){
	    case RT_GGM_SM:{
		    res = sim900_SM_read(arg->sm.index, arg->sm.mode, 
			                    (rt_shortMsg *)buffer);
		    if(res != 0){
	            rt_set_errno(RT_ERROR);
	            return 0;
            }
			ret = ((rt_shortMsg *)buffer)->length;			            
		} break;
		case RT_GGM_CSQ:{
		    res = sim900_csq_read((int *)buffer);
		    if(res != 0){
	            rt_set_errno(RT_ERROR);
	            return 0;
            }
			ret = 1;		    
		} break;
		case RT_GGM_CENG:{
		    res = sim900_ceng_read(((rt_cellsInfo *)buffer)->array);
		    if(res != 0){
	            rt_set_errno(RT_ERROR);
	            return 0;
            }
			ret = 1;
		} break;
		case RT_GGM_TIME:{
		    res = sim900_time_read(buffer);
		    if(res != 0){
	            rt_set_errno(RT_ERROR);
	            return 0;
            }
			ret = 1;		    
		} break;
		case RT_GGM_BATTERY:{
		    res = sim900_battery_read(buffer);
		    if(res != 0){
	            rt_set_errno(RT_ERROR);
	            return 0;
            }
			ret = 1;		    
		} break;
		case RT_GGM_RES: {
		    strCpy((char *)buffer, writeRes);
			__CONSOLE("write RES %s %d\n", writeRes, strLen(writeRes));
			ret = strLen(writeRes);
		}
		default: 
		    rt_set_errno(RT_ERROR);
		    return 0;
	}
	return ret;
}
/**
  * @brief  control device
  * @param  dev, cmd, arg
  * @retval err 
  */
static rt_err_t   rt_gsmgprs_control(rt_device_t dev, rt_uint8_t cmd, void *arg)
{
    rt_ggm_private *ggm;
	rt_uint8_t cmd1, cmd2;
	int cnt;
	int res;

	if(dev == RT_NULL){
	    return RT_ERROR;    
	}

	ggm = (rt_ggm_private *)(dev->user_data);
	if(ggm == RT_NULL){
	    return RT_ERROR;    
	}    

    cmd1 = cmd & 0x0f;

	switch(cmd1){
	    case RT_GGM_CMD_AT: {
		    cnt = 5;
		    while( -1 == sim900_test_AT() && cnt > 0){
			    cnt --;
			}
			if(cnt == 0)
			    return RT_ERROR;   
		} break;

	    case RT_GGM_CMD_WRITE: {
		    cmd2 = (cmd & 0xf0) >> 4;
		    if(cmd2 == RT_GGM_SM){
                ggm->wr_setting.sm = (*((rt_ggm_wr *)arg)).sm;
				ggm->wr_mode = cmd2;
		    } else if(cmd2 == RT_GGM_GPRS) {
			    ggm->wr_mode = cmd2;
			} else {  /* undefined, error */
			    return RT_ERROR;
			}
		} break;

		case RT_GGM_CMD_READ: {
		    cmd2 = cmd & 0xf0;
		    if(cmd2 == RT_GGM_SM){
                ggm->rd_setting.sm = (*((rt_ggm_rd *)arg)).sm;
				ggm->rd_mode = cmd2;
			} else if(cmd2 == RT_GGM_CSQ || cmd2 == RT_GGM_CENG || cmd2 == RT_GGM_TIME || 
			          cmd2 == RT_GGM_BATTERY || cmd2 == RT_GGM_RES){
			    ggm->rd_mode = cmd2;
			} else {  /* undefined, error */
			    return RT_ERROR;
			}          
		} break;

	    case RT_GGM_CMD_TCPSERVER: {
		} break;

		case RT_GGM_CMD_CMGF: {
		    res = sim900_config_cmgf(*((rt_uint8_t *)arg));
			if(res != 0)
			    return RT_ERROR;
		}

		case RT_GGM_CMD_ATE: {
		    res = sim900_ate(*((rt_uint8_t *)arg));
			if(res != 0)
			    return RT_ERROR;		     
		} break;

		case RT_GGM_CMD_TCP_CLOSE: {
		    res = sim900_tcp_close();
			if(res != 0)
			    return RT_ERROR;
		} break;

		case RT_GGM_CMD_TCP_SHUT: {
		    res = sim900_tcp_shut();
			if(res != 0)
			    return RT_ERROR;
		} break;

		case RT_GGM_CMD_TCP_CLIENT: {
		    res = sim900_tcp_client(((rt_ggm_host *)arg)->ip, ((rt_ggm_host *)arg)->port);
			if(res != 0)
			    return RT_ERROR;            
		} break;

		case RT_GGM_CMD_TIME_SET: {
		    res = sim900_time_set(arg);
			if(res != 0)
			    return RT_ERROR;
		} break;

		default: return RT_ERROR;
	}
    return RT_EOK;
}
/**
  * @brief  init device
  * @param  dev
  * @retval err 
  */
static rt_err_t   rt_gsmgprs_init(rt_device_t dev)
{
    sim900_hw_init();
	return RT_EOK;
}
/**
  * @brief  open device
  * @param  dev, oflag
  * @retval err 
  */ 
static rt_err_t   rt_gsmgprs_open(rt_device_t dev, rt_uint16_t oflag)
{
    int res;

    USART_Cmd(USART2, ENABLE);

    sim900_pwrKey_pulse();

	res = sim900_module_config();

	if(res != 0){
	    __CONSOLE("SIM900A initialization failed!\n");
		LOG(2, 0, 0);
	    return RT_ERROR;
    }
	__CONSOLE("SIM900A initialization success!\n");
	LOG(1, 0, 0);

	writeRes = rt_malloc(RES_SIZE);
	if(writeRes == RT_NULL)
	    __CONSOLE("Malloc failed!\n");    
    return RT_EOK;
} 
/**
  * @brief  close device
  * @param  dev
  * @retval err 
  */ 
static rt_err_t   rt_gsmgprs_close(rt_device_t dev)
{
    rt_free(writeRes);
	__CONSOLE("Free memory %u!\n", writeRes);
    return RT_EOK;
}
/**
  * @brief  register device
  * @param  dev
  * @retval err 
  */
rt_err_t rt_gsmgprs_register(void)
{
    rt_device_t device;

	device = &ggm_device;

	device->type 		= RT_Device_Class_Unknown;
	device->rx_indicate = RT_NULL;
	device->tx_complete = RT_NULL;
	device->init 		= rt_gsmgprs_init;
	device->open		= rt_gsmgprs_open;
	device->close		= rt_gsmgprs_close;
	device->read 		= rt_gsmgprs_read;
	device->write 		= rt_gsmgprs_write;
	device->control 	= rt_gsmgprs_control;
	device->user_data	= &private;

	/* register a character device */
	return rt_device_register(&ggm_device, GGM_DEVICE_NAME, RT_DEVICE_FLAG_RDWR);
}

void rt_ggm_int_enable(void)
{
    sim900_int_enable();
}
void rt_ggm_int_disable(void)
{
    sim900_int_disable();
}
void rt_ggm_int_bufswitch(void)
{
    sim900_buf_switch();
}
int  rt_ggm_isSM(uint8_t *buf)
{
    return sim900_checkBuf_isSM(buf);
}
int  rt_ggm_isRING(char *buf)
{
    return sim900_checkBuf_isRING(buf);
}
int  rt_ggm_waitForCLOSED(rt_uint8_t timeover)
{
    return sim900_waitfor_CLOSED(timeover);
}

