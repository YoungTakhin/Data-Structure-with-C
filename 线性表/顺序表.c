#include <stdio.h>
#include <stdlib.h> // malloc、free
 
/*状态码*/
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
 
#define MAXSIZE 20 // 存储空间初始分配量
 
typedef int Status; // Statu是函数返回的数据类型，其值为状态代码
typedef int ElemType; // ElemType为数据类型，类型根据实际情况而定，这里假设为int
 
/* 定义结点 */
typedef struct Node {
	ElemType data;
	struct Node *next;
}Node;
 
typedef struct Node *LinkList; // 定义链表

Status InitList(LinkList *L) {
	*L = (LinkList)malloc(sizeof(Node)); // 产生头结点,并使L指向此头结点
	if(!(*L)) {
		return ERROR; // 内储分配失败
	}
	(*L)->next = NULL; // 指针域为空
	return OK; // 内存分配成功
}

Status ClearList(LinkList *L) {
	LinkList p, q;
	p = (*L)->next; // p指向第一个结点
	/* 没到表尾 */
	while(p) {
		q = p->next;
		free(p);
		p = q;
	}
	(*L)->next = NULL; // 头结点指针域为空
	return OK;
}

Status ListEmpty(LinkList L) {
	if(L->next) {
		return FALSE;
	}
	else {
		return TRUE;
	}
}

int ListLength(LinkList L) {
	int i = 0; //计数器
	LinkList p = L->next; // p指向第一个结点
	while(p) {
		i++;
		p = p->next;
	}
	return i;
}

Status GetElem(LinkList L, int i, ElemType *e) {
	int j; //计数器
	LinkList p;	// 声明结点p
	p = L->next; // 让p指向链表L的第一个结点
	j = 1;
	/* p不为空或者计数器j还没有等于i时，循环继续 */
	while(p && j < i) {
		p = p->next; // 让p指向下一个结点
		++j;
	}
	if(!p || j > i) {
		return ERROR; // 第i个元素不存在
	}
	*e = p->data; // 取第i个元素的数据
	return OK;
}

int LocateElem(LinkList L, ElemType e) {
	int i = 0;
	LinkList p = L->next;
	while(p) {
		i++;
		/* 找到这样的数据元素 */
		if(p->data == e) {
			return i;
		}
		p = p->next;
	}
	return 0;
}

Status PriorElem(LinkList L, ElemType e, ElemType *pre_e) {
	int i;
	i = LocateElem(L, e);
	if(i == 1 || i == 0) {
		return ERROR;
	}
	else {
		GetElem(L, i - 1, pre_e);
		return OK;
	}
}

Status NextElem(LinkList L, ElemType e, ElemType *next_e) {
	int i;
	i = LocateElem(L, e);
	if(i == ListLength(L) || i == 0) {
		return ERROR;
	}
	else {
		GetElem(L, i + 1, next_e);
		return OK;
	}
}

Status ListInsert(LinkList *L, int i, ElemType e) {
	int j;
	LinkList p, s;
	p = *L;
	j = 1;
	/* 寻找第i个结点 */
	while(p && j < i) {
		p = p->next;
		++j;
	}
	if(!p || j > i) {
		return ERROR; // 第i个元素不存在
	}
	s = (LinkList)malloc(sizeof(Node)); // 生成新结点
	s->data = e;
	s->next = p->next; // 将p的后继结点赋值给s的后继
	p->next = s; // 将s赋值给p的后继
	return OK;
}

Status ListDelete(LinkList *L, int i, ElemType *e) {
	int j;
	LinkList p, q;
	p = *L;
	j = 1;
	/* 遍历寻找第i个元素 */
	while(p->next && j < i) {
		p = p->next;
		++j;
	}
	/* 第i个元素不存在 */
	if(!(p->next) || j > i) {
		return ERROR;
	}
	q = p->next;
	p->next = q->next; // 将q的后继赋值给p的后继
	*e = q->data; // 将q结点中的数据赋值给e
	free(q); // 让系统回收此结点，释放内存
	return OK;
}

Status TraverseList(LinkList L) {
	LinkList p = L->next;
	while(p) {
		printf("%d ", p->data);
		p = p->next;
	}
	printf("\n");
	return OK;
}

int main(int argc, char** argv) {
	LinkList L; //声明链表L
	ElemType e; //声明数据元素e
	Status i; //声明状态码
	int j, k; //声明临时变量
 
	i = InitList(&L);
	printf("初始化L后表长：%d\n", ListLength(L));
	for(j = 1, e = 1; j <= 5; j++, e++) {
		i = ListInsert(&L, j, e);
	}
	printf("在L的表头依次插入 1～5 后：");
	TraverseList(L);
	printf("L表长：%d \n", ListLength(L));
 
	i = ListEmpty(L);
	printf("L是否空：%d (1:是 0:否)\n", i);
 
	i = ClearList(&L);
	printf("清空 L 后：L表长：%d\n", ListLength(L));
 
	i = ListEmpty(L);
	printf("L是否为空：%d (1:是 0:否)\n", i);
 
	for(j = 1, e = 1; j <= 10; j++, e++) {
		ListInsert(&L, j, e);
	}
	printf("在L的表尾依次插入 1～10 后：");
	TraverseList(L);
	printf("L表长：%d \n", ListLength(L));
 
	ListInsert(&L, 1, 0);
	printf("在L的表头插入 0 后：");
	TraverseList(L);
	printf("L表长：%d \n", ListLength(L));
 
	GetElem(L, 5, &e);
	printf("第5个元素的值为 %d\n", e);
 
	j = 4;
	k = LocateElem(L, j);
	if(k) {
		printf("值为%d的元素位置在%d\n", j, k);
	}
	else {
		printf("没有值为%d的元素\n", j);
	}
 
	k = ListLength(L); // k为表长
	for(j = k + 1; j >= k; j--) {
		i = ListDelete(&L, j, &e); // 删除第j个位置的数据
		if(i == ERROR) {
			printf("删除第%d个数据失败 \n", j);
		}
		else {
			printf("删除第%d个的元素值为：%d\n", j, e);
		}
	}
 
	printf("依次输出 L 的元素： ");
	TraverseList(L);
 
	j = 5;
	ListDelete(&L, j, &e); // 删除第5个位置的数据
	printf("删除第%d个的元素值为：%d\n", j, e);
	printf("依次输出L的元素： ");
	TraverseList(L);
 
	j = 5;
	i = PriorElem(L, j, &e);
	if(i == 0) {
		printf("没有值为%d的元素 或 %d是第一个元素，没有前驱\n", j, j);
	}
	else {
		printf("%d的前驱是%d\n", j, e);
	}
 
	j = 999;
	i = PriorElem(L, j, &e);
	if(i == 0) {
		printf("没有值为%d的元素 或 %d是第一个元素，没有前驱\n", j, j);
	}
	else {
		printf("%d的前驱是 %d\n", j, e);
	}
 
	j = 5;
	i = NextElem(L, j, &e);
	if(i == 0) {
		printf("没有值为%d的元素 或 %d是最后一个元素，没有后继\n", j, j);
	}
	else {
		printf("%d的后继是%d\n", j, e);
	}
 
	j = 1000;
	i = NextElem(L, j, &e);
	if(i == 0) {
		printf("没有值为%d的元素 或 %d是最后一个元素，没有后继\n", j, j);
	}
	else {
		printf("%d的后继是%d\n", j, e);
	}
 
	ClearList(&L);
 
	return 0;
}

