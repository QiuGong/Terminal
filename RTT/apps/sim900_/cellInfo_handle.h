#ifndef __CELLINFO_HANDLE_H
#define __CELLINFO_HANDLE_H

#include "rt_gsmgprs.h"
#include "time.h"

#define CELLNUMBER_IN_A_JSONARRAY 7 /* 1~7 */
#define CELLINFO_NUM_MAX 15
#define LOG_MAX_NUM 20

int newCellInfoPackage(int isVoid);
int addCellInfo(rt_cellsInfo *cells);	   /* after this fun, cells can be free */ 
int addTimeValid(int bool);
int addStatus(int bool, int stat[]); 
char *printCellInfoPackage(void);
int addLog(int id, Time t, char arg[]);
void deleteCellInfoPackage(void);
#endif

