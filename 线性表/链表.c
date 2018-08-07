#include <stdio.h>
 
/* 状态码 */
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
 
#define MAXSIZE 1000 /* 存储空间初始分配量 */
 
typedef int Status; // Status是函数的类型,其值是函数结果状态代码，如OK等
typedef int ElemType; // ElemType类型根据实际情况而定，这里假设为int
 
/* 定义静态链表 */
typedef struct {
	ElemType data; // 储存数据元素
	int cur;  // 游标(Cursor)，为0时表示无指向
} Component, StaticLinkList[MAXSIZE];
 
int ListLength(StaticLinkList L); //返回表长，供其他函数调用
Status ListDelete(StaticLinkList L, int i); //删除数据元素，供其他函数调用
 
/* 若备用空间链表非空，则返回分配的结点下标，否则返回0 */
int Malloc_SSL(StaticLinkList space) {
	/* 当前数组第一个元素的cur存的值就是要返回的第一个备用空闲的下标 */
	int i = space[0].cur;
	if(space[0].cur) {
		space[0].cur = space[i].cur; // 由于要拿出一个分量来使用了，所以就将其下一个分量做备用
	}
	return i;
}
 
/* 将下标为k的空闲结点回收到备用链表 */
void Free_SSL(StaticLinkList space, int k) {
	space[k].cur = space[0].cur; // 把第一个元素的cur值赋给要删除的分量cur
	space[0].cur = k; // 把要删除的分量下标赋值给第一个元素的cur
}

Status InitList(StaticLinkList space) {
	int i;
	for(i = 0; i < MAXSIZE - 1; i++) {
		space[i].cur = i + 1;
	}
	space[MAXSIZE - 1].cur = 0; // 目前静态链表为空，最后一个元素的cur为0
	return OK;
}

Status ClearList(StaticLinkList L) {
	int k;
	for(k = ListLength(L); k > 0; k--) {
		ListDelete(L, k);
	}
	return 1;
}

Status ListEmpty(StaticLinkList L) {
	int j = 0;
	int i = L[MAXSIZE - 1].cur;
	while(i) {
		i = L[i].cur;
		j++;
	}
	if(j == 0) {
		return TRUE;
	}
	return FALSE;
}

int ListLength(StaticLinkList L) {
	int j = 0;
	int i = L[MAXSIZE - 1].cur;
	while(i) {
		i = L[i].cur;
		j++;
	}
	return j;
}

Status GetElem(StaticLinkList L, int i, ElemType *e) {
	int j;
	int k = 1;
	for(j = L[MAXSIZE - 1].cur; j>0; k++) {
		if(k == i) {
			*e = L[j].data;
			return OK;
		}
		j = L[j].cur;
 
	}
	return ERROR;
}

int LocateElem(StaticLinkList L, ElemType e) {
	int j;
	int k = 1;
	for(j = L[MAXSIZE - 1].cur; j>0; k++) {
		if(e == L[j].data) {
			return k;
		}
		j = L[j].cur;
	}
	return ERROR;
}

Status PriorElem(StaticLinkList L, ElemType e, ElemType *pre_e) {
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

Status NextElem(StaticLinkList L, ElemType e, ElemType *next_e) {
	int i;
	i = LocateElem(L, e);
	if(i == ListLength(L) || i == 0) {
		return ERROR;
	}
	else {
		GetElem(L, i+1, next_e);
		return OK;
	}
}

Status ListInsert(StaticLinkList L, int i, ElemType e) {
	int j, k, l;
	k = MAXSIZE - 1; // 注意k首先是最后一个元素的下标
	if(i < 1 || i > ListLength(L) + 1)
		return ERROR;
	j = Malloc_SSL(L); // 获得空闲分量的下标
	if(j) {
		L[j].data = e; // 将数据赋值给此分量的data
		/* 找到第i个元素之前的位置 */
		for(l = 1; l <= i - 1; l++) {
			k = L[k].cur;
		}
		L[j].cur = L[k].cur; // 把第i个元素之前的cur赋值给新元素的cur/
		L[k].cur = j; // 把新元素的下标赋值给第i个元素之前元素的cur
		return OK;
	}
	return ERROR;
}

Status ListDelete(StaticLinkList L, int i) {
	int j, k;
	if(i < 1 || i > ListLength(L)) {
		return ERROR;
	}
	k = MAXSIZE - 1;
	for(j = 1; j <= i - 1; j++) {
		k = L[k].cur;
	}
	j = L[k].cur;
	L[k].cur = L[j].cur;
	Free_SSL(L, j);
	return OK;
}

Status TraverseList(StaticLinkList L) {
	int i = L[MAXSIZE - 1].cur;
	while(i) {
		printf("%d ", L[i].data);
		i = L[i].cur;
	}
	printf("\n");
	return OK;
}

int main(int argc, char** argv) {
	StaticLinkList L; // 声明静态链表L
	Status i; // 声明状态码i
	ElemType e; // 声明数据元素e
	int j, k; // 声明临时变量
 
	i = InitList(L);
	printf("初始化L后表长：%d\n", ListLength(L));
	for(j = 1, e = 1; j <= 5; j++, e++) {
		i = ListInsert(L, j, e);
	}
	printf("在L的表头依次插入 1～5 后：");
	TraverseList(L);
	printf("L表长：%d \n", ListLength(L));
 
	i = ListEmpty(L);
	printf("L是否空：%d (1:是 0:否)\n", i);
 
	i = ClearList(L);
	printf("清空 L 后：L表长：%d\n", ListLength(L));
 
	i = ListEmpty(L);
	printf("L是否为空：%d (1:是 0:否)\n", i);
 
	for(j = 1, e = 1; j <= 10; j++, e++) {
		ListInsert(L, j, e);
	}
	printf("在L的表尾依次插入 1～10 后：");
	TraverseList(L);
	printf("L表长：%d \n", ListLength(L));
 
	ListInsert(L, 1, 0);
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
		i = ListDelete(L, j); // 删除第j个位置的数据
		if(i == ERROR) {
			printf("删除第%d个数据失败\n", j);
		}
		else {
			printf("删除第%d个的元素\n", j);
		}
	}
 
	printf("依次输出 L 的元素： ");
	TraverseList(L);
 
	j = 5;
	ListDelete(L, j); // 删除第5个位置的数据
	printf("删除第%d个的元素\n", j);
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
 
	ClearList(L);
 
	return 0;
}

