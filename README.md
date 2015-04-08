# Terminal
RT-Thread Sim900

想加入的伙伴，原谅我英语不好。做了这个决定，把代码开源出来，是因为这一次我迫切的想完善好SIM900的应用。GPRS车载产品是我第一个完成的产品，但只试用不到1000台就夭折了，各种问题。这一次我想借助大家的力量来做好。


###工程介绍
本项目有2个文件夹，1个RT-Thread帮助文档。RTT 为我们自定义工程，而RT-Thread是RT系统。这是参考别人的，把系统和自己工程分开。下面会介绍RTT
，而RT-Thread就不多说了。

1. RTT（自定义工程）
2. RT-Thread（RT-Thread系统）
3. rtthread_manual.zh.pdf（RT-Thread 帮助文档）

###自定义工程
![](http://i.imgur.com/soUHXs2.png)

cm3：RT支持该芯片

kernel：RT系统

Finsh：shell命令

STM32_StdPeriph：STM32硬件初始化库

StartUp：中断，RT启动

**以上主要是支持RT系统，不是该项目重点。**

Apps：(config.h 项目配置文件，大家把不需要的功能关闭)

Driver：设备驱动

Warn：报警设置（主要是对采集设备数据处理，报警）

RS485：采集设备通信，数据解析

Display：显示屏通信，数据解析

**Sim900：Sim900模块（这个是重点，大家可以把其它宏关闭，删除文件都行）**

FLASH：Flash数据存储

Util：工具包

以上是对项目的简单说明，其他的大家可以不并多关心。咱们重点核心是来维护Sim900这个功能模块。