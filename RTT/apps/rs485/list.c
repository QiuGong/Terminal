#include <rs485.h>
#include <string.h>


#if ( RS485_EN > 0 )

/*
	List MakeEmpty(List L)(ʹ��)
����
	L Ҫ���ɵĿ�������
����ֵ
	�������ɵĿ�������
����
	���ɿ�����item���
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
	Position Find() (ʹ��)
����
	b id��
	L Ҫ���ҵ�����
����ֵ
	��L�д��ڶ�Ӧid�ţ��򷵻ص�ǰ�ڵ�λ�ã����򷵻�NULL
����
	
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
	Position FindPrevious(void *b, List L)��ʹ�ã�
����
	b id��
	L Ҫ���ҵ�����
����ֵ
	��L�д��ڶ�Ӧid�ţ��򷵻�ǰһ�ڵ�λ�ã����򷵻�NULL
����

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
����
	P Ҫ���������
����ֵ
	��
����
	������Pλ��֮�����ɿ�����item���
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
���� 
	b Ҫɾ����ID 
	L Ҫɾ���ڵ����ڵ����� 
����ֵ 
	����ɾ����ַ 
���� 
	������L��ɾ�����ҵ��ĵ�һ��������ΪX�Ľڵ㣬
	(void *)RT_UINT32_MAX��ʣ��һ��ʱ����
	p != RT_NULL��ɾ������ͷʱ����
	p == RT_NULL��ɾ���м���ʱ����
	 
*/
void* Delete(void *b, List sensor)
{
	Position cur, pre;
	
	// ��ǰ��
	cur = Find(b, sensor);

    // ����ͷ
	if(cur == sensor)				   
	{
		void *p = cur->next;
		rt_free(cur);

		// ֻʣһ��ʱ
		if(p == NULL)
		{
		   return (void *)RT_UINT32_MAX;
		}
		else
		{
			return p;
		}
	}

	// ǰһ��
	pre = sensor;
	while( pre != NULL && pre->next != cur )
	{
		pre = pre->next;
	}
	
	// ɾ��   
    pre->next = cur->next;  
    rt_free(cur);
	return (void *)0;  	 
}


/*
	unsigned char Length(List L)
����
	L Ҫ���ҵ�����
����ֵ
	������
����
	
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
