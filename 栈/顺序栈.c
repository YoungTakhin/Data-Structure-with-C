#include <stdio.h>
 
/* 状态码 */
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
 
#define MAXSIZE 20 // 存储空间初始分配量
 
typedef int Status; // Statu是函数返回的数据类型，其值为状态代码
typedef int SElemType; // SElemType为数据类型，类型根据实际情况而定，这里假设为int

/* 定义顺序栈 */
typedef struct {
	SElemType data[MAXSIZE]; // 储存数据元素
	int top; // 用于栈顶指针
}SqStack;

Status InitStack(SqStack *S) {
	S->top = -1;
	return OK;
}

Status ClearStack(SqStack *S) {
	S->top = -1;
	return OK;
}

Status StackEmpty(SqStack S) {
	if(S.top == -1) {
		return TRUE;
	}
	return FALSE;
}

int StackLength(SqStack S) {
	return S.top + 1;
}

Status GetTop(SqStack S, SElemType *e) {
	if(S.top == -1) {
		return ERROR;
	}
	*e = S.data[S.top];
	return OK;
}

Status Push(SqStack *S, SElemType e) {
	/* 栈满 */
	if(S->top == MAXSIZE - 1) {
		return ERROR;
	}
	S->top++; // 栈顶指针增加1
	S->data[S->top] = e; // 将新插入元素赋值给栈顶空间
	return OK;
}

Status Pop(SqStack *S, SElemType *e) {
	if(S->top == -1) {
		return ERROR;
	}
	*e = S->data[S->top]; // 将要删除的栈顶元素赋值给e
	S->top--; // 栈顶指针减1
	return OK;
}

Status StackTraverse(SqStack S) {
	int i = 0;
	while(i <= S.top) {
		printf("%d ", S.data[i++]);
	}
	printf("\n");
	return OK;
}

int main(int argc, char** argv) {
	SqStack s; // 声明顺序栈s
	SElemType e; // 声明数据元素e
	int j; // 声明临时变量
 
	if(InitStack(&s) == OK) {
		for(j = 1; j <= 10; j++) {
			Push(&s, j);
		}
	}
	printf("栈中元素依次为： ");
	StackTraverse(s);
 
	Pop(&s, &e);
	printf("弹出的栈顶元素 e=%d\n", e);
	printf("栈是否为空：%d(1:空 0:否)\n", StackEmpty(s));
 
	GetTop(s, &e);
	printf("栈顶元素e=%d\n", e);
	printf("栈的长度为 %d\n", StackLength(s));
 
	ClearStack(&s);
	printf("清空栈后，栈是否为空：%d(1:空 0:否)\n", StackEmpty(s));
 
	return 0;
}

