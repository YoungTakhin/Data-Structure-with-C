#include <stdio.h>
#include <stdlib.h> // malloc、free
 
/* 状态码 */
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
 
#define MAXSIZE 20 // 存储空间初始分配量
 
typedef int Status; // Statu是函数返回的数据类型，其值为状态代码
typedef int SElemType; // SElemType为数据类型，类型根据实际情况而定，这里假设为int
 
/* 定义结点 */
typedef struct StackNode {
	SElemType data; // 储存数据元素
	struct StackNode *next; // 指针
}StackNode, *LinkStackPtr;
 
/* 定义链栈 */
typedef struct {
	LinkStackPtr top; // 栈顶
	int count; // 计数器
}LinkStack;

Status InitStack(LinkStack *S) {
	S->top = (LinkStackPtr)malloc(sizeof(StackNode));
	if(!S->top) {
		return ERROR;
	}
	S->top = NULL;
	S->count = 0;
	return OK;
}

Status ClearStack(LinkStack *S) {
	LinkStackPtr p, q;
	p = S->top;
	while(p) {
		q = p;
		p = p->next;
		free(q);
	}
	S->count = 0;
	return OK;
}

Status StackEmpty(LinkStack S) {
	if(S.count == 0) {
		return TRUE;
	}
	return FALSE;
}

int StackLength(LinkStack S) {
	return S.count;
}

Status GetTop(LinkStack S, SElemType *e) {
	if(S.top == NULL) {
		return ERROR;
	}
	else {
		*e = S.top->data;
	}
	return OK;
}

Status Push(LinkStack *S, SElemType e) {
	LinkStackPtr s = (LinkStackPtr)malloc(sizeof(StackNode));
	s->data = e;
	s->next = S->top; // 把当前的栈顶元素赋值给新结点的直接后继，见图中①
	S->top = s; // 将新的结点s赋值给栈顶指针，见图中②
	S->count++;
	return OK;
}

Status Pop(LinkStack *S, SElemType *e) {
	LinkStackPtr p;
	if(StackEmpty(*S)) {
		return ERROR;
	}
	*e = S->top->data;
	p = S->top;	// 将栈顶结点赋值给p，见图中③
	S->top = S->top->next; // 使得栈顶指针下移一位，指向后一结点，见图中④ 
	free(p); // 释放结点p
	S->count--;
	return OK;
}

Status StackTraverse(LinkStack S) {
	LinkStackPtr p;
	p = S.top;
	while(p) {
		printf("%d ", p->data);
		p = p->next;
	}
	printf("\n");
	return OK;
}

int main(int argc, char** argv) {
	LinkStack s; // 声明链栈s
	SElemType e; // 声明数据元素e
	int j; // 声明临时变量
 
	if(InitStack(&s) == OK) {
		for(j = 1; j <= 10; j++) {
			Push(&s, j);
		}
	}
 
	printf("栈中元素依次为：");
	StackTraverse(s);
	printf("栈长：%d\n", StackLength(s));
 
	Pop(&s, &e);
	printf("弹出的栈顶元素 e=%d\n", e);
	printf("栈是否为空：%d(1:空 0:否)\n", StackEmpty(s));
	GetTop(s, &e);
	printf("栈顶元素:%d\n栈的长度为%d\n", e, StackLength(s));
 
	ClearStack(&s);
	printf("清空栈后，栈是否为空：%d(1:空 0:否)\n", StackEmpty(s));
 
	return 0;
}

