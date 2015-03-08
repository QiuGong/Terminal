#include"strLib.h"


/* 把字符转换成小写字母,非字母字符不做出处理 */
int toLower( int ch) 
{
   if ( (unsigned int)(ch - 'A') < 26u )
      ch += 'a' - 'A';
   return ch;
}


/* 把字符串转换成长整型数 */
long atol(const char *nptr)
{
        int c;              /* current char */        
		long total;         /* current total */        
		int sign;           /* if '-', then negative, otherwise positive */        
		/* skip whitespace */       
		while ( isspace((int)(unsigned char)*nptr) )           
			 ++nptr;        
		c = (int)(unsigned char)*nptr++;        
		sign = c;           /* save sign indication */        
		if (c == '-' || c == '+')            
		    c = (int)(unsigned char)*nptr++;    /* skip sign */        
		total = 0;        
		while (isdigit(c)) {            
		    total = 10 * total + (c - '0');     /* accumulate digit */            
			c = (int)(unsigned char)*nptr++;    /* get next char */        
		}        
		if (sign == '-')            
		    return -total;        
		else            
		    return total;   /* return result, negated if necessary */
}


/* 将字符串转换成整型数 */
int atoi(const char *nptr){        
    return (int)atol(nptr);
}


/* 把src所指字符串添加到dest结尾处 */
char *strCat(char *des, const char *src, unsigned int des_size)
{
    int i, j, k;
    
    if( !(des && src))
        return NULL;
    for(i = 0; *(des+i) != '\0'; i ++);
    for(j = 0; *(src+j) != '\0'; j ++);
    if(i+j > des_size)
        return NULL;
    
    j += i;
    for(k = 0;i < j; i++, k++)
    *(des+i) = *(src+k);
    *(des+i) = '\0';
    return des; 
}


/* 查找字符串s中首次出现字符c的位置 */
char *strChr(const char *string, char c)
{
    int i;
    
    if(!string)
        return NULL;
    
    i = 0;
    while(1){
        if(*(string+i) == c){
            break;     
        } else if(*(string+i) == '\0'){
            i = 0;
            break;
        }
		i++;
    }
    
    if(i == 0)
        return NULL;
    else
        return (char *)(string+i);
}


/* 函数搜索一个字符串在另一个字符串中的第一次出现 */
char* strStr(char* string, char* strSearch)
{
    int i;    
    char *p, *q;
        
 	if (NULL == string || NULL == strSearch)   
 	    return NULL; 
	
	for(i = 0; *(string+i) != '\0'; i++){
	 	p = (char *)(string+i);
	 	q = strSearch;
	 	
	 	while ('\0' != *q && *q == *p){
		 	  ++p;
		 	  ++q;
		}	
		if ('\0' == *q)
	        return (char *)(string+i);		
	}
	return 0;
}


/* 计算字符串s的（unsigned int型）长度 */
unsigned int strLen(const char *string)
{
    unsigned int len;
    
    for(len = 0; *(string+len) != '\0'; len++); 
    return len;
}


/* 将任意类型的数字转换为字符串 */
char *itoa(int num,char *str,int radix) 
{
     char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 
     unsigned int unum;
     int i, j, k;
     i = 0;
      
     if(radix==10&&num<0){
         unum=(unsigned)(-num); 
         str[i++]='-'; 
     } else 
         unum=(unsigned)num;
     do{
         str[i++]=index[unum%(unsigned)radix]; 
         unum/=radix;               
     }while(unum); 
     str[i]='\0';       
     if(str[0]=='-'){
         k=1;
         i-=1;
         str = str + 1;
     } else 
         k=0;  
     for(j=0;j<=(i-1)/2.0;j++){    
         num=str[j]; 
         str[j]=str[i-j-1];
         str[i-j-1]=num; 
     }
     if(k == 1)
         str = str - 1;     
     return str;              
}


/* 把从src地址开始且含有NULL结束符的字符串复制到以dest开始的地址空间 */
char *strCpy(char *strDestination, const char *strSource)
{
    char *strD=strDestination;

 	if (NULL == strDestination || NULL == strSource)   
 	    return NULL;      

	while ((*strDestination++=*strSource++)!='\0');
	return strD;
}


//正向复制
unsigned short positive(unsigned char *sour, unsigned char *dest, unsigned short len)	  
{
	unsigned short i = 0;
	
	for(i=0; i<len; i++)
	{
		*dest++ = *sour++;
	}
	return len;
}


//反向复制
unsigned short negative(unsigned char *sour, unsigned char *dest, unsigned short len)	  
{
	unsigned short i=0;
	
	sour = sour + len - 1;
	for(i=0; i<len; i++)
	{
		*dest++ = *sour--;
	}
	return len;
}


// 0x79 -> 79  十六进制 转 十进制
unsigned char bcd8_to_hex8(unsigned char bcd_data)
{
    unsigned char temp;
	temp=((bcd_data>>4)*10 + (bcd_data&0x0f)); 	   
	return temp;
}


// 79 -> 0x79  十进制 转 十六进制
unsigned char hex8_to_bcd8(unsigned char hex_data)
{
    unsigned char temp;
	temp = ((hex_data/10)<<4 | (hex_data%10));  
	return temp;
}


// 6789 -> 0x6789  十进制 转 十六进制
unsigned short hex16_to_bcd16(unsigned short hex_data)
{ 
	unsigned char a1, a2, a3, a4;
	a1 = hex_data / 1000;
	a2 = (hex_data - (a1 * 1000))/100;
	a3 = (hex_data - (a1 * 1000) - (a2 * 100))/10;
	a4 = (hex_data - (a1 * 1000) - (a2 * 100) - (a3 * 10))/1;	
	return (a1<<12 | a2<<8 | a3<<4 | a4);
}

// 删除字符串中指定字符
void del_char(char* str,char ch)
{
	int i,j;
	for(i=0; str[i]!='\0'; i++)
	{
		if(str[i]==ch) 
		{
			for(j=i; str[j]!='\0'; j++) 
			{
				str[j]=str[j+1]; 
			}
		}
	}
}
