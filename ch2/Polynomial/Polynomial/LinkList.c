#include<stdio.h>
#include <malloc.h>
#include"PreDefined.h"
#include "LinkList.h"
//针对有序链表更新了LocateElement函数，增加了0rderInsert函数
/*
typedef struct LNode {//结点类型
ElemType data;
struct LNode *next;
}*Link, *Position;
typedef struct {
Link head, tail;
int len;
}LinkList;
*/
Status makeNode(Link *p,ElemType e)
{
	(*p) = (Link)malloc(sizeof(struct LNode));
	if (!*p)return OVERFLOW;
	(*p)->data = e;
	(*p)->next = NULL;
	return OK;
}
//指针指向的内容释放后，指针记得置为NULL
void freeNode(Link *p)
{
	free(*p);
	*p = NULL;
} 
Status initList(LinkList *L)
{
	Link p;
	Status i;
	ElemType e = { .0,0 };
	if (i= makeNode(&p, e))
	{
		L->head = p;
		L->tail = p;
		L->len = 0;
		return OK;
	}
	return i;
}
Status destroyList(LinkList *L)
{
	Link q,p=(*L).head;
	while (p!=NULL)
	{
		q = p;
		p = p->next;
		free(q);
	}
	(*L).len = 0;
	(*L).head = NULL;
	(*L).tail = NULL;
	return OK;
}
Status clearList(LinkList *L)
{
	Link q, p = (*L).head->next;
	while (p != NULL)
	{
		q = p;
		p = p->next;
		free(q);
	}
	(*L).head->next = NULL;
	(*L).tail = (*L).head;
	(*L).len = 0;
	return OK;
}
//h指向线性表的头结点，将s插入在头结点之后，第一个结点之前；
Status inFirst(LinkList *L, Link h, Link s)
{
	s->next= h->next;
	h ->next = s;
	if (h == (*L).tail) /* h指向尾结点 */
		(*L).tail = h->next; /* 修改尾指针 */
	(*L).len++;
	return OK;
}
//h指向线性表的头结点，，删除链表中第一个结点；
Status delFirst(LinkList *L,Link h, Link *q)
{
	////////
	if (!h->next)
	{
		*q = NULL;
		return ERROR;
	}
		
	*q = h->next;
	h->next = h->next->next;
	(*L).len--;
	return OK;
}
//将s所指的一串结点（没有头结点），接在链表L之后//修复了一条计数错误
Status append(LinkList *L, Link s)
{
	(*L).tail->next= s;
	Link p = s;
	(*L).len++;
	while (p->next!=NULL)
	{
		p = p->next;
		(*L).len++;
	}
	(*L).tail = p;
	return OK;
}
///test2
//删除尾节点以q返回，改变尾指针
//remove nnnnnnnnnnnnnnn
Status remove(LinkList *L, Link *q)
{
	if (L->tail==L->head) return ERROR;
	(*q) = (*L).tail;
	(*L).len--;
	Link p= (*L).head;
	for (int i = 1; i <= (*L).len; i++)
	{
		p = p->next;
	}
	p->next = NULL;
	(*L).tail = p;
	return OK;
}
//已知p指向L中的一个结点，将S插入p结点之前,并改变p的指向指向s
Status insBefore(LinkList *L, Link *p, Link *s)
{
	Link q = L->head;
	while (q!=NULL&&q->next != (*p))
	{
		q = q->next;
	}
	if (!q)return ERROR;
	q ->next = *s;
	(*s)->next = (*p);
	(*p) = *s;
	L->len++;
	return OK;
}
//已知p指向L中的一个结点，将S插入p结点之后,并改变p的指向指向s
Status insAfter(LinkList *L, Link *p, Link *s)
{
	if (L->tail == *p)L->tail = *s;
	(*s)->next = (*p)->next;
	(*p)->next = *s;
	(*p) = *s;
	L->len++;
	return OK;
}
//修改p结点的值
Status setCurrentElement(Link p, ElemType e)
{
	p->data = e;
	return OK;
}
//返回p结点的值
ElemType getCurrentElement(Link p)
{
	return p->data;
}
Status listEmpty(LinkList L)
{
	if (L.len == 0)return TRUE;
	return FALSE;
}
int listLength(LinkList L)
{
	return L.len;
}
Position getHead(LinkList L)
{
	return L.head;
}
Position getLast(LinkList L)
{
	return L.tail;
}
Position priorPos(LinkList L, Link p)
{
	Link q = L.head;
	while (q!=NULL&&q->next != p)
	{
		q = q->next;
	}
	if (q == NULL)return NULL;
	return q;
 }
Position nextPos(LinkList L, Link p)
{
	if (!p)return NULL;
	return p->next;
}
Status LocatePos(LinkList L, int i, Link *p)
{
	if (i <1 || i>L.len)return ERROR;
	Link ptr=L.head;
	int q;
	for (q = 1; q <=i; q++)
	{
		ptr = ptr->next;
	}
	(*p) = ptr;
	return OK;
}

Status listTraverse(LinkList L, void(*visit)(ElemType*))
{
	Link p = L.head->next;
	while (p != NULL)
	{
		visit(&(p->data));
		p = p->next;
	}
	return OK;
}
//LocateElement若存在返回TRUE 否则返回第一个>0的位置的直接前驱，并返回False；
Status locateElement(LinkList L, ElemType e, Position *q, Status(*compare)(ElemType, ElemType))
{
	Link p = L.head->next;
	while (p != NULL)
	{
		if ((*compare)(e, p->data) == 0)break;
		p = p->next;
	}
	if (p != NULL) {
		*q = p;
		return TRUE;
	}
	else
	{
		Link ptr = L.head;
		Link p = L.head->next;
		while (p != NULL)
		{
			if ((*compare)(p->data, e) > 0)break;
			ptr = p;
			p = p->next;
		}
		*q = ptr;
		return FALSE;
	}
}

Status orderInsert(LinkList *L, ElemType e, Status(*compare)(ElemType, ElemType))
{
	Link p,q;
	
	Status flag = locateElement(*L, e, &q, compare);
	//如果存在相同指数的项进行合并，不存在进行添加；
	if (flag)
	{
		q->data.coef += e.coef;
		if (q->data.coef == 0)
		{
			p = priorPos(*L, q);
			delFirst(L, p, &q);
		}
	}
	else 
	{
		makeNode(&p, e);
		insAfter(L, &q, &p);
	}
 }
