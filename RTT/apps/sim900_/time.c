#include "time.h"
#include "strLib.h"

const char DIV_SYM[] = "//,::\0";

int  time_parase(Time t, const char *str)
{
    char *p, *q;
	char copy[30];
    int i;

    if(t == 0 || str == 0)
	    return -1;
    
	strCpy(copy, str);

	p = copy; 
	for(i = 0; i < 6; i ++){	    		
		q = p + 2;
		if(*q != DIV_SYM[i])
		    return -1;
		*q = '\0';
		t[i] = atoi(p);
		p = q + 1;
	}
	
	return 0;	  	  	    
}


int  time_print(Time t, char *des) 
{
    char *p;
    int i;

    if(t == 0 || des == 0)
	    return -1;

	p = des;	
	for(i = 0; i < 6; i++){
		if(t[i] < 10){
		    *p++ = '0';
			*p++ = '0'+ t[i];
		} else {
		    itoa(t[i], p, 10);
			p += 2;
		}
		*p++ = DIV_SYM[i];
	}
	
	return 0;	    
}


int time_print_withoutDate(Time t, char *des)
{
    char *p;
    int i;

    if(t == 0 || des == 0)
	    return -1;

	p = des;	
	for(i = 3; i < 6; i++){
		if(t[i] < 10){
		    *p++ = '0';
			*p++ = '0'+ t[i];
		} else {
		    itoa(t[i], p, 10);
			p += 2;
		}
		*p++ = DIV_SYM[i];
	}
	
	return 0;
}


void time_inc_ss(Time t, unsigned short ss)
{
    unsigned char minute;
    unsigned char second;

	if(t == 0 || ss >= 3600)
	    return;

    minute = ss / 60;
	if(minute == 0){
	    second = ss;
	} else {
	    second = ss % 60;
	}
	
	t[TIME_ss] += second;
	t[TIME_mm] += minute;
	
	if(t[TIME_ss] >= 60){
	    t[TIME_mm] += t[TIME_ss] / 60; 
	    t[TIME_ss] = t[TIME_ss] % 60;
	}
	if(t[TIME_mm] >= 60){
	    t[TIME_hh] += t[TIME_mm] / 60; 
	    t[TIME_mm] = t[TIME_mm] % 60;
	}
	if(t[TIME_hh] >= 24){
	    t[TIME_dd] += t[TIME_hh] / 24; 
	    t[TIME_hh] = t[TIME_hh] % 24;
	}	 	   
}


int time_copy(Time des, Time src)
{
    int i;
    if(des == 0 || src == 0)
	    return -1;
	for(i = 0; i < 6; i ++)
	    des[i] = src[i];
	return 0;    
}


int time_compare(Time A, Time B)
{
    int i;
    if(A == 0 || B == 0)
	    return 0;
	
	for(i = 0; i <6; i ++){
	    if(A[i] > B[i])
		    return 1;
	    else if(A[i] < B[i])
		    return -1;
	}
	return 0;
}	 
