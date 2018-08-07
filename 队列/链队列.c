#include <stdio.h>
#include <stdlib.h> // malloc、free
 
/* 状态码 */
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
 
#define MAXSIZE 20 // 存储空间初始分配量
 
typedef int Status; // Status是函数的类型,其值是函数结果状态代码，如OK等
typedef int QElemType; // ElemType类型根据实际情况而定，这里假设为int
 
/* 定义结点 */
typedef struct QNode {
	QElemType data;
	struct QNode *next;
}QNode, *QueuePtr;
 
/* 定义链栈 */
typedef struct {
	QueuePtr front, rear; // 队头、队尾指针
}LinkQueue;

Status InitQueue(LinkQueue *Q) {
	Q->front = (QueuePtr)malloc(sizeof(QNode));
	Q->rear = Q->front;
	if(!Q->front) {
		return ERROR;
	}
	Q->front->next = NULL;
	return OK;
}

Status DestroyQueue(LinkQueue *Q) {
	while(Q->front) {
		Q->rear = Q->front->next;
		free(Q->front);
		Q->front = Q->rear;
	}
	return OK;
}

Status ClearQueue(LinkQueue *Q) {
	QueuePtr p, q;
	Q->rear = Q->front;
	p = Q->front->next;
	Q->front->next = NULL;
	while(p) {
		q = p;
		p = p->next;
		free(q);
	}
	return OK;
}

Status QueueEmpty(LinkQueue Q) {
	if(Q.front == Q.rear) {
		return TRUE;
	}
	return FALSE;
}

int QueueLength(LinkQueue Q) {
	int i = 0;
	QueuePtr p;
	p = Q.front;
	while(Q.rear != p) {
		i++;
		p = p->next;
	}
	return i;
}

Status GetHead(LinkQueue Q, QElemType *e) {
	QueuePtr p;
	if(Q.front == Q.rear) {
		return ERROR;
	}
	p = Q.front->next;
	*e = p->data;
	return OK;
}

Status EnQueue(LinkQueue *Q, QElemType e) {
	QueuePtr s = (QueuePtr)malloc(sizeof(QNode));
	/* 存储分配失败 */
	if(!s) {
		return ERROR;
	}
	s->data = e;
	s->next = NULL;
	Q->rear->next = s; // 把拥有元素e的新结点s赋值给原队尾结点的后继
	Q->rear = s; // 把当前的s设置为队尾结点，rear指向s
	return OK;
}

Status DeQueue(LinkQueue *Q, QElemType *e) {
	QueuePtr p;
	if(Q->front == Q->rear) {
		return ERROR;
	}
	p = Q->front->next; // 将欲删除的队头结点暂存给p
	*e = p->data; // 将欲删除的队头结点的值赋值给e
	Q->front->next = p->next; // 将原队头结点的后继p->next赋值给头结点后继
	/* 若队头就是队尾，则删除后将rear指向头结点*/
	if(Q->rear == p) {
		Q->rear = Q->front;
	}
	free(p);
	return OK;
}

Status QueueTraverse(LinkQueue Q) {
	QueuePtr p;
	p = Q.front->next;
	while(p) {
		printf("%d ", p->data);
		p = p->next;
	}
	printf("\n");
	return OK;
}

int main(int argc, char** argv) {
	LinkQueue Q; // 声明链队Q
	QElemType e; // 声明数据元素e
	Status i; // 声明状态码i
 
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
 
	i = DestroyQueue(&Q);
	if(i == OK) {
		printf("链队销毁成功\n");
	}
	else {
		printf("链队销毁失败\n");
	}
 
	return 0;
}

