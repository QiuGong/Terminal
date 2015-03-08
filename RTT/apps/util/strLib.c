#include"strLib.h"


/* ���ַ�ת����Сд��ĸ,����ĸ�ַ����������� */
int toLower( int ch) 
{
   if ( (unsigned int)(ch - 'A') < 26u )
      ch += 'a' - 'A';
   return ch;
}


/* ���ַ���ת���ɳ������� */
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


/* ���ַ���ת���������� */
int atoi(const char *nptr){        
    return (int)atol(nptr);
}


/* ��src��ָ�ַ�����ӵ�dest��β�� */
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


/* �����ַ���s���״γ����ַ�c��λ�� */
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


/* ��������һ���ַ�������һ���ַ����еĵ�һ�γ��� */
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


/* �����ַ���s�ģ�unsigned int�ͣ����� */
unsigned int strLen(const char *string)
{
    unsigned int len;
    
    for(len = 0; *(string+len) != '\0'; len++); 
    return len;
}


/* ���������͵�����ת��Ϊ�ַ��� */
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


/* �Ѵ�src��ַ��ʼ�Һ���NULL���������ַ������Ƶ���dest��ʼ�ĵ�ַ�ռ� */
char *strCpy(char *strDestination, const char *strSource)
{
    char *strD=strDestination;

 	if (NULL == strDestination || NULL == strSource)   
 	    return NULL;      

	while ((*strDestination++=*strSource++)!='\0');
	return strD;
}


//������
unsigned short positive(unsigned char *sour, unsigned char *dest, unsigned short len)	  
{
	unsigned short i = 0;
	
	for(i=0; i<len; i++)
	{
		*dest++ = *sour++;
	}
	return len;
}


//������
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


// 0x79 -> 79  ʮ������ ת ʮ����
unsigned char bcd8_to_hex8(unsigned char bcd_data)
{
    unsigned char temp;
	temp=((bcd_data>>4)*10 + (bcd_data&0x0f)); 	   
	return temp;
}


// 79 -> 0x79  ʮ���� ת ʮ������
unsigned char hex8_to_bcd8(unsigned char hex_data)
{
    unsigned char temp;
	temp = ((hex_data/10)<<4 | (hex_data%10));  
	return temp;
}


// 6789 -> 0x6789  ʮ���� ת ʮ������
unsigned short hex16_to_bcd16(unsigned short hex_data)
{ 
	unsigned char a1, a2, a3, a4;
	a1 = hex_data / 1000;
	a2 = (hex_data - (a1 * 1000))/100;
	a3 = (hex_data - (a1 * 1000) - (a2 * 100))/10;
	a4 = (hex_data - (a1 * 1000) - (a2 * 100) - (a3 * 10))/1;	
	return (a1<<12 | a2<<8 | a3<<4 | a4);
}

// ɾ���ַ�����ָ���ַ�
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
