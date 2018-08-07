#include <stdio.h>
 
/* 状态码 */
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
 
#define MAXSIZE 20 // 存储空间初始分配量
 
typedef int Status; // Statu是函数返回的数据类型，其值为状态代码
typedef int SElemType; // SElemType为数据类型，类型根据实际情况而定，这里假设为int
 
/* 定义两栈共享空间结构 */
typedef struct {
	SElemType data[MAXSIZE];
	int top1; // 栈1栈顶指针
	int top2; // 栈2栈顶指针
}SqDoubleStack;

Status InitStack(SqDoubleStack *S) {
	S->top1 = -1;
	S->top2 = MAXSIZE;
	return OK;
}

Status ClearStack(SqDoubleStack *S) {
	S->top1 = -1;
	S->top2 = MAXSIZE;
	return OK;
}

Status StackEmpty(SqDoubleStack S) {
	if(S.top1 == -1 && S.top2 == MAXSIZE) {
		return TRUE;
	}
	return FALSE;
}

int StackLength(SqDoubleStack S) {
	return (S.top1 + 1) + (MAXSIZE - S.top2);
}

Status GetTop(SqDoubleStack S, SElemType *e, int stackNumber) {
	if(stackNumber == 1) {
		if(S.top1 == -1) {
			return ERROR;
		}
		else {
			*e = S.data[S.top1];
		}
		return OK;
	}
	else {
		if(S.top2 == -1) {
			return ERROR;
		}
		else {
			*e = S.data[S.top2];
		}
		return OK;
	}
}

Status Push(SqDoubleStack *S, SElemType e, int stackNumber) {
	/* 栈已满，不能再入栈了 */
	if(S->top1 + 1 == S->top2) {
		return ERROR;
	}
	/* 栈1有元素进栈 */
	if(stackNumber == 1) {
		S->data[++S->top1] = e; // 若是栈1则先top1+1后给数组元素赋值。
	}
	/* 栈2有元素进栈 */
	else if(stackNumber == 2) {
		S->data[--S->top2] = e; // 若是栈2则先top2-1后给数组元素赋值。
	}
	return OK;
}

Status Pop(SqDoubleStack *S, SElemType *e, int stackNumber) {
	if (stackNumber == 1) {
		/* 说明栈1已经是空栈，溢出 */
		if(S->top1 == -1) {
			return ERROR;
		}
		*e = S->data[S->top1--]; // 将栈1的栈顶元素出栈
	}
	else if(stackNumber == 2) {
		/* 说明栈2已经是空栈，溢出 */
		if(S->top2 == MAXSIZE) {
			return ERROR;
		}
		*e = S->data[S->top2++]; // 将栈2的栈顶元素出栈
	}
	return OK;
}

Status StackTraverse(SqDoubleStack S) {
	int i;
	i = 0;
	while(i <= S.top1) {
		printf("%d ", S.data[i++]);
	}
	i = S.top2;
	while(i < MAXSIZE) {
		printf("%d ", S.data[i++]);
	}
	printf("\n");
	return OK;
}

int main(int argc, char** argv) {
	SqDoubleStack s;
	SElemType e;
	int j;
 
	if(InitStack(&s) == OK) {
		for(j = 1; j <= 5; j++) {
			Push(&s, j, 1);
 
		}
		for(j = MAXSIZE; j >= MAXSIZE - 2; j--) {
			Push(&s, j, 2);
		}
	}
 
	printf("栈中元素依次为：");
	StackTraverse(s);
	printf("栈长：%d\n", StackLength(s));
 
	Pop(&s, &e, 2);
	printf("弹出的栈顶元素e=%d\n", e);
	printf("栈是否为：%d(1:空 0:否)\n", StackEmpty(s));
 
	for(j = 6; j <= MAXSIZE - 2; j++) {
		Push(&s, j, 1);
	}
 
	printf("栈中元素依次为：");
	StackTraverse(s);
 
	ClearStack(&s);
	printf("清空栈后，栈是否为空：%d(1:空 0:否)\n", StackEmpty(s));
 
	Push(&s, 1, 1);
	Push(&s, 3, 1);
	Push(&s, 5, 2);
	Push(&s, 7, 2);
	Pop(&s, &e, 1);
	printf("栈中元素依次为：");
	StackTraverse(s);
	printf("栈长：%d\n", StackLength(s));
 
	GetTop(s, &e, 1);
	printf("栈1的栈顶元素：%d\n", e);
	GetTop(s, &e, 2);
	printf("栈2的栈顶元素：%d\n", e);
 
	return 0;
}

