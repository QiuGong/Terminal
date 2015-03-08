#ifndef __TIME_H
#define __TIME_H

typedef unsigned char Time[6];

#define TIME_yy 0
#define TIME_MM 1
#define TIME_dd 2
#define TIME_hh 3
#define TIME_mm 4
#define TIME_ss 5

int  time_parase(Time t, const char *str);
int  time_print(Time t, char *des); // format: yy/MM/dd,hh:mm:ss
int time_print_withoutDate(Time t, char *des);   // format: hh:mm:ss
void time_inc_ss(Time t, unsigned short ss);	// ss is less than 3600
int time_copy(Time des, Time src);
int time_compare(Time A, Time B); // returns 1 if A is later than B, 0 means A is equal to B, and...

#endif


