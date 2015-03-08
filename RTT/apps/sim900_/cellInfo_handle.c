#include "cellInfo_handle.h"
#include "cJSON.h"
#include "strLib.h"

static cJSON *root = 0;
static int rootIsValid = 0; 
static int count = 0;;
static int logCnt = 0;
static cJSON *logArray;


int newCellInfoPackage(int isVoid)
{    
    if(rootIsValid != 0)
	    cJSON_Delete(root);
	root = cJSON_CreateObject();
	rootIsValid = 1;
	count = 0;

    if(isVoid)
        cJSON_AddTrueToObject(root, "isVoid");	    
	else
	    cJSON_AddFalseToObject(root, "isVoid");
	return 0;    
}


int addCellInfo(rt_cellsInfo *cells)
{
    int i;
    char *s;
    char strTmp[] = "xx";     
    cJSON *child;

    if(count >= CELLINFO_NUM_MAX)
	    return -1;

	child = cJSON_CreateArray();

	s = (char *)rt_malloc(11);
    /* add timeOffset */
	cJSON_AddItemToArray(child, cJSON_CreateString(cells->time));
		
	for(i = 0; i < CELLNUMBER_IN_A_JSONARRAY; i ++){
	    /* format: 	  lac    cellid	  rxl
		              XXXX    XXXX	  XX  
		   total: 11 byte
	    */
		*s = '\0';
		strCat(s, cells->array[i].lac, 11);
		strCat(s, cells->array[i].cellid, 11);
        itoa(cells->array[i].rxl, strTmp, 10);
	    strCat(s, strTmp, 11);
		cJSON_AddItemToArray(child, cJSON_CreateString(s)); 
	}

	itoa(count, s, 10);
	cJSON_AddItemToObject(root, s, child);
	rt_free(s);

	count ++;
	return 0;
}


int addTimeValid(int bool)
{
    if(bool == 0){
	    cJSON_AddFalseToObject(root, "timeValid");
	} else {
	    cJSON_AddTrueToObject(root, "timeValid");
	}
	return 0;  
}


int addStatus(int bool, int stat[])
{
    char *s;
	cJSON *child;
	int i;
	
	child = cJSON_CreateArray();
    if(bool == 0){
	    cJSON_AddItemToArray(child, cJSON_CreateFalse());
		cJSON_AddItemToObject(root, "status", child);
		return 0;    
	} else {
	    if(stat == 0)
		    return -1;
    	s = (char *)rt_malloc(10);
     	if(s == 0)
	        return -1;
	    cJSON_AddItemToArray(child, cJSON_CreateTrue());	   
		for(i = 0; i < 3; i ++){
		    itoa(stat[i], s, 10);
			cJSON_AddItemToArray(child, cJSON_CreateString(s));
		}		
		cJSON_AddItemToObject(root, "status", child);
		rt_free(s);
		return 0;
	}
}


char *printCellInfoPackage(void)
{
    char *s;
    char *ret;

	s = (char *)rt_malloc(4);
	itoa(count, s, 10);
    cJSON_AddStringToObject(root, "size", s);

	itoa(logCnt, s, 10);
    cJSON_AddStringToObject(root, "logCnt", s);
	if(logCnt){
	    cJSON_AddItemToObject(root, "log", logArray);
		logCnt = 0;
	} 

	ret = cJSON_PrintUnformatted(root);

	rt_free(s);
	return ret;
}


void deleteCellInfoPackage(void)
{
    if(rootIsValid != 0)
	    cJSON_Delete(root);
	rootIsValid = 0;
}


int addLog(int id, Time t, char arg[])
{
    cJSON *logItem;
    char *s;

    if(logCnt == LOG_MAX_NUM)
	    return 0;

    if(logCnt == 0){
	    logArray = cJSON_CreateArray();
	}
	logItem = cJSON_CreateArray();

	s = (char *)rt_malloc(10);
	if(s == 0)
	   return -1;
    itoa(id, s, 10); 
	cJSON_AddItemToArray(logItem, cJSON_CreateString(s));
	if(t != 0){
	    time_print_withoutDate(t, s);
		cJSON_AddItemToArray(logItem, cJSON_CreateString(s));
	} else {
	    cJSON_AddItemToArray(logItem, cJSON_CreateString("N/A"));    
	}
	
	if(arg != 0)
	    cJSON_AddItemToArray(logItem, cJSON_CreateString(arg));
	else
	    cJSON_AddItemToArray(logItem, cJSON_CreateString("null"));	
    cJSON_AddItemToArray(logArray, logItem); 
	rt_free(s);
	logCnt ++;
	return 0;   
}

