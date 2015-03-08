#ifndef __STRLIB_H
#define __STRLIB_H

#ifndef NULL
    #define NULL 0
#endif

#define isspace(c) ((c == ' ') ? 1: 0) 
#define isdigit(c) (c <= '9'&& c >= '0' )

int atoi(const char *nptr);
long atol(const char *nptr);
char *itoa(int num,char *str,int radix);
unsigned int strLen(const char *string);
char *strStr(char* string, char* strSearch);
char *strChr(const char *string, char c);
char *strCat(char *des, const char *src, unsigned int des_size);
char *strCpy(char *strDestination, const char *strSource);
int toLower( int ch);
unsigned short positive(unsigned char *sour, unsigned char *dest, unsigned short len);
unsigned short negative(unsigned char *sour, unsigned char *dest, unsigned short len);
unsigned char  bcd8_to_hex8(unsigned char bcd_data);
unsigned char  hex8_to_bcd8(unsigned char hex_data);
unsigned short hex16_to_bcd16(unsigned short hex_data);
void del_char(char* str,char ch);
#endif

