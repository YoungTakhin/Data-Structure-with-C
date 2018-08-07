#include <stdio.h>
 
/* 状态码 */
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
 
#define MAXSIZE 20 // 存储空间初始分配量
 
typedef int Status; // Status是函数的类型,其值是函数结果状态代码，如OK等
typedef int QElemType; // ElemType类型根据实际情况而定，这里假设为int
 
/* 定义顺序循环队列 */
typedef struct {
	QElemType data[MAXSIZE]; // 储存数据元素
	int front; // 头指针
	int rear; // 尾指针，若队列不空，指向队列尾元素的下一个位置
}SqQueue;

Status InitQueue(SqQueue *Q) {
	Q->front = 0;
	Q->rear = 0;
	return  OK;
}

Status ClearQueue(SqQueue *Q) {
	Q->front = 0;
	Q->rear = 0;
	return OK;
}

Status QueueEmpty(SqQueue Q) {
	/* 队列空的标志 */
	if(Q.front == Q.rear) {
		return TRUE;
	}
	return FALSE;
}

int QueueLength(SqQueue Q) {
	return (Q.rear - Q.front + MAXSIZE) % MAXSIZE;
}

Status GetHead(SqQueue Q, QElemType *e) {
	/* 队列空 */
	if(Q.front == Q.rear) {
		return ERROR;
	}
	*e = Q.data[Q.front];
	return OK;
}

Status EnQueue(SqQueue *Q, QElemType e) {
	/* 队列满标志 */
	if((Q->rear + 1) % MAXSIZE == Q->front) {
		return ERROR;
	}
	Q->data[Q->rear] = e; // 将元素e赋值给队尾
	Q->rear = (Q->rear + 1) % MAXSIZE; // rear指针向后移一位置，若到最后则转到数组头部
	return OK;
}

Status DeQueue(SqQueue *Q, QElemType *e) {
	/* 队列空标志 */
	if(Q->front == Q->rear) {
		return ERROR;
	}
	*e = Q->data[Q->front];	// 将队头元素赋值给e
	Q->front = (Q->front + 1) % MAXSIZE; // front指针向后移一位置，若到最后则转到数组头部
	return OK;
}

Status QueueTraverse(SqQueue Q) {
	int i;
	i = Q.front;
	while(i != Q.rear) {
		printf("%d ", Q.data[i]);
		i = (i + 1) % MAXSIZE;
	}
	printf("\n");
	return OK;
}

int main(int argc, char** argv) {
	SqQueue Q; // 声明顺序循环队列Q
	QElemType e; // 声明数据元素e
 
	InitQueue(&Q);
	printf("初始化后队是否为空：%d（1:空，0:否）\n", QueueEmpty(Q));
 
	EnQueue(&Q, 1);
	EnQueue(&Q, 2);
	EnQueue(&Q, 3);
	printf("把1 2 3依次入队后数据元素：");
	QueueTraverse(Q);
	printf("入队后队是否为空：%d（1:空，0:否）\n", QueueEmpty(Q));
	printf("入队后队长：%d\n", QueueLength(Q));
 
	GetHead(Q, &e);
	printf("队头数据元素：%d\n", e);
 
	DeQueue(&Q, &e);
	printf("出队后数据元素：");
	QueueTraverse(Q);
	GetHead(Q, &e);
	printf("队头数据元素：%d\n", e);
 
	DeQueue(&Q, &e);
	printf("出队后数据元素：");
	QueueTraverse(Q);
	printf("出队后队长：%d\n", QueueLength(Q));
 
	ClearQueue(&Q);
	printf("清空后队长：%d\n", QueueLength(Q));
	printf("清空后队是否为空：%d（1:空，0:否）\n", QueueEmpty(Q));
 
	return 0;
}

