// linktable.cpp
// Demonstration a link table usage:

// 单链表结点类型的定义
typedef struct node{
	int data;
	struct node *link;
}NODE, *LinkList;

/* 
 *	单链表查找运算
 *	L为链表的头指针,在表中查找第K个元素,返回该元素结点指针,否则,返回空指针NULL
 */
LinkList Find_List(LinkList L, int k)
{
	LinkList p; int i;
	i = 1; p = L->link;
	while (p && k<i)
	{
		p = p->link;
		i++;
	}
	return (p && i==k) ? p : NULL;
}

/*
 * 	单链表的插入运算 
 *  L为链表的头指针,
 *  K为在链表插入的位置
 *  elem为元素值
 */
int Insert_List(LinkList L, int k, int elem)
{
	LinkList p, q;
	if (k == 1) { 
		p = L;
	}
	else {
		// 查找表中第k-1个元素,原因是要在k-1个位置后插入新元素.
		p = Find_List(L, k-1);
	}
	
	if (p == NULL)
		return -1;
	
	q = (NODE*)malloc(sizeof(NODE));
	q->data = elem;
	q->link = p->link;
	p->link = q;
	return 0;
}

/*
 *	单链表的删除运算
 *  L为链表头指针
 *  K为元素位置
 *  成功返回0,失败返回-1.
 */
int Delete_List(LinkList L, int k)
{
	LinkList p, q;
	if (k==1) { 
		p = L;
	} else {
		p = Find_List(L, k-1);
	}

	if (p == NULL)
		return -1;

	q = p->link;
	p->link = q->link;
	free(q);
	return (0);
}

int main(int argc, char** argv)
{
	return (0);
}