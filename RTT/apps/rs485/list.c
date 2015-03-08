#include <rs485.h>
#include <string.h>


#if ( RS485_EN > 0 )

/*
	List MakeEmpty(List L)(使用)
参数
	L 要生成的空链表名
返回值
	返回生成的空链表名
功能
	生成空链表，item清空
*/
List MakeEmpty(List L)
{
	L = (PNode)rt_malloc(sizeof(Node));
	if(L == RT_NULL)
	{
		rt_kprintf("MakeEmpty() rt_malloc failed.\n"); 
		return RT_NULL;
	}


	memset(&(L->item), 0x00, sizeof(Element));
	L->next = NULL;
	return L;
}


/*
	Position Find() (使用)
参数
	b id号
	L 要查找的链表
返回值
	在L中存在对应id号，则返回当前节点位置，否则返回NULL
功能
	
*/
Position Find(void *b, List L)
{
	Position P = L;
	while( P != NULL && (memcmp((const void *)P->item.id, (const void *)b, ID_LEN) != 0))
	{
		P = P->next;
	}
	return P;
}


/*
	Position FindPrevious(void *b, List L)（使用）
参数
	b id号
	L 要查找的链表
返回值
	在L中存在对应id号，则返回前一节点位置，否则返回NULL
功能

*/
Position FindPrevious(void *b, List L)
{
	Position P = L;
	while(P->next!=NULL && (memcmp((const void *)P->item.id, (const void *)b, ID_LEN) != 0))
		P = P->next;
	return P;
}


/*
	void Insert(Position P)
参数
	P 要插入的链表
返回值
	无
功能
	在链表P位置之后，生成空链表，item清空
*/
void Insert(Position P)
{
	Position temp;

	temp = rt_malloc(sizeof(Node));
	if(temp == RT_NULL)
	{
		rt_kprintf("MakeEmpty() rt_malloc failed.\n"); 
		return ;
	}

	memset(&(temp->item), 0x00, sizeof(Element));
	temp->next = P->next;
	P->next = temp;	
}


/* 
	void* Delete(void *b, List sensor) 
参数 
	b 要删除的ID 
	L 要删除节点所在的链表 
返回值 
	返回删除地址 
功能 
	在链表L中删除查找到的第一个数据项为X的节点，
	(void *)RT_UINT32_MAX：剩下一个时返回
	p != RT_NULL：删除链表头时返回
	p == RT_NULL：删除中间项时返回
	 
*/
void* Delete(void *b, List sensor)
{
	Position cur, pre;
	
	// 当前项
	cur = Find(b, sensor);

    // 链表头
	if(cur == sensor)				   
	{
		void *p = cur->next;
		rt_free(cur);

		// 只剩一个时
		if(p == NULL)
		{
		   return (void *)RT_UINT32_MAX;
		}
		else
		{
			return p;
		}
	}

	// 前一项
	pre = sensor;
	while( pre != NULL && pre->next != cur )
	{
		pre = pre->next;
	}
	
	// 删除   
    pre->next = cur->next;  
    rt_free(cur);
	return (void *)0;  	 
}


/*
	unsigned char Length(List L)
参数
	L 要查找的链表
返回值
	链表长度
功能
	
*/
unsigned char Length(List L)
{
	unsigned char len = 0;
	Position P = L;

	while( P != NULL)
	{
		P = P->next;
		len ++ ;
	}

	return len;
}
#endif
