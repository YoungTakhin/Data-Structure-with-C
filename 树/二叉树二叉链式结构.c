#include <stdio.h>
#include <stdlib.h> // malloc、free
 
/* 状态码 */
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
 
#define MAXSIZE 100 // 存储空间初始分配量
 
typedef int Status; // Status是函数的类型，其值是函数结果状态代码，如OK等
typedef int TElemType; // TlemType为数据类型，类型根据实际情况而定，这里假设为int
 
TElemType Nil = 0; // 定义整型以0为空
			
/* 定义二叉树结点结构 */
typedef struct BiTNode {
	TElemType data; // 结点数据
	struct BiTNode *lchild, *rchild; // 左右孩子指针
}BiTNode, *BiTree;
 
#if 1 // 以下是引入的链队列的基本操作
 
typedef int Status; // Status是函数的类型,其值是函数结果状态代码，如 OK 等
typedef BiTree QElemType; // QElemType类型定义为BiTree
 
/* 定义结点 */
typedef struct QNode {
	QElemType data;
	struct QNode *next;
}QNode, *QueuePtr;
 
/* 定义链队列 */
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
 
Status QueueEmpty(LinkQueue Q) {
	if(Q.front == Q.rear) {
		return TRUE;
	}
	return FALSE;
}
 
Status EnQueue(LinkQueue *Q, QElemType e) {
	QueuePtr s = (QueuePtr)malloc(sizeof(QNode));
	/* 存储分配失败 */
	if(!s) {
		return ERROR;
	}
	s->data = e;
	s->next = NULL;
	Q->rear->next = s; // 把拥有元素 e 的新结点 s 赋值给原队尾结点的后继
	Q->rear = s; // 把当前的 s 设置为队尾结点，rear 指向 s
	return OK;
}
 
Status DeQueue(LinkQueue *Q, QElemType *e) {
	QueuePtr p;
	if (Q->front == Q->rear) {
		return ERROR;
	}
	p = Q->front->next; // 将欲删除的队头结点暂存给 p
	*e = p->data; // 将欲删除的队头结点的值赋值给 e
	Q->front->next = p->next; // 将原队头结点的后继 p->next 赋值给头结点后继
	/* 若队头就是队尾，则删除后将 rear 指向头结点*/
	if(Q->rear == p) {
		Q->rear = Q->front;
	}
	free(p);
	return OK;
}
 
#endif // 1
 
/* 返回二叉树T中指向数据元素值为s的结点指针，供其他函数调用 */
BiTree Point(BiTree T, TElemType s) {
	LinkQueue q;
	QElemType a;
	/* 非空树 */
	if(T) {
		InitQueue(&q); // 初始化队列
		EnQueue(&q, T); // 根指针入队
		/* 队不空 */
		while(!QueueEmpty(q)) {
			DeQueue(&q, &a); // 出队，数据元素赋给a
			if(a->data == s) {
				return a;
			}
			/* 有左孩子 */
			if(a->lchild) {
				EnQueue(&q, a->lchild); // 入队左孩子
			}
			/* 有右孩子 */
			if(a->rchild) {
				EnQueue(&q, a->rchild); // 入队右孩子
			}
		}
	}
	return NULL;
}
 
Status InitBiTree(BiTree *T) {
	*T = NULL;
	return OK;
}

void DestroyBiTree(BiTree *T) {
	if(*T) {
		/* 有左孩子 */
		if((*T)->lchild) {
			DestroyBiTree(&(*T)->lchild); // 销毁左孩子子树
		}
		/* 有右孩子 */
		if((*T)->rchild) {
			DestroyBiTree(&(*T)->rchild); // 销毁右孩子子树
		}
		free(*T); // 释放根结点
		*T = NULL; // 空指针赋0
	}
}

Status CreateBiTree(BiTree *T) {
	TElemType i;
	scanf("%d ", &i);
	if(i == 0 || i == 999) {
		*T = NULL;
		return OK;
	}
	else {
		*T = (BiTree)malloc(sizeof(BiTNode));
		if(!*T) {
			return ERROR;
		}
		(*T)->data = i; // 生成根结点
		CreateBiTree(&(*T)->lchild); // 构造左子树
		CreateBiTree(&(*T)->rchild); // 构造右子树
	}
	return OK;
}

void ClearBiTree(BiTree *T) {
	if(*T) {
		/* 有左孩子 */
		if((*T)->lchild) {
			ClearBiTree(&(*T)->lchild); // 销毁左孩子子树
		}
		/* 有右孩子 */
		if((*T)->rchild) {
			ClearBiTree(&(*T)->rchild); // 销毁右孩子子树
		}
		free(*T); // 释放根结点
		*T = NULL; // 空指针赋0
	}
}

Status BiTreeEmpty(BiTree T) {
	if(T) {
		return FALSE;
	}
	return TRUE;
}

int BiTreeDepth(BiTree T) {
	int i, j;
	if(!T) {
		return 0;
	}
	if(T->lchild) {
		i = BiTreeDepth(T->lchild);
	}
	else {
		i = 0;
	}
	if(T->rchild) {
		j = BiTreeDepth(T->rchild);
	}
	else {
		j = 0;
	}
	return i > j ? i + 1 : j + 1;
}

TElemType Root(BiTree T) {
	if(BiTreeEmpty(T)) {
		return Nil;
	}
	return T->data;
}

Status Assign(BiTree T, TElemType s, TElemType value) {
	Point(T, s)->data = value;
	return OK;
}

TElemType Parent(BiTree T, TElemType e) {
	LinkQueue q;
	QElemType a;
	/* 非空树 */
	if(T) {
		InitQueue(&q); // 初始化队列
		EnQueue(&q, T); // 树根指针入队
		/* 队不空 */
		while(!QueueEmpty(q)) {
			DeQueue(&q, &a); // 出队，队列元素赋给a
			/* 找到e(是其左或右孩子) */
			if(a->lchild && a->lchild->data == e || a->rchild && \
				a->rchild->data == e) {
				return a->data; // 返回e的双亲的值
			}
			/* 没找到e，则入队其左右孩子指针(如果非空) */
			else {
				if(a->lchild) {
					EnQueue(&q, a->lchild);
				}
				if(a->rchild) {
					EnQueue(&q, a->rchild);
				}
			}
		}
	}
	return Nil; // 树空或没找到e
}

TElemType LeftChild(BiTree T, TElemType e) {
	BiTree a;
	/* 非空树 */
	if(T) {
		a = Point(T, e); // a是结点e的指针
		/* T中存在结点e且e存在左孩子 */
		if(a && a->lchild) {
			return a->lchild->data; // 返回e的左孩子的值
		}
	}
	return Nil; // 其他情况返回空
}

TElemType RightChild(BiTree T, TElemType e) {
	BiTree a;
	/* 非空树 */
	if(T) {
		a = Point(T, e); // a是结点e的指针
		/* T中存在结点e且e存在右孩子 */
		if(a && a->rchild) {
			return a->rchild->data; // 返回e的右孩子的值
		}
	}
	return Nil; // 其他情况返回空
}

TElemType LeftSibling(BiTree T, TElemType e)  {
	TElemType a;
	BiTree p;
	/* 非空树 */
	if(T) {
		a = Parent(T, e); // a为e的双亲
		/* 找到e的双亲 */
		if(a != Nil) {
			p = Point(T, a); // p为指向结点a的指针
			/* p存在左右孩子且右孩子是e */
			if(p->lchild && p->rchild && p->rchild->data == e) {
				return p->lchild->data; // 返回p的左孩子(e的左兄弟)
			}
		}
	}
	return Nil; /* 其他情况返回空 */
}

TElemType RightSibling(BiTree T, TElemType e) {
	TElemType a;
	BiTree p;
	/* 非空树 */
	if(T) {
		a = Parent(T, e); // a为e的双亲
		/* 找到e的双亲 */
		if(a != Nil) {
			p = Point(T, a); // p为指向结点a的指针
			/* p存在左右孩子且左孩子是e */
			if(p->lchild && p->rchild && p->lchild->data == e) {
				return p->rchild->data; // 返回p的右孩子(e的右兄弟)
			}
		}
	}
	return Nil; // 其他情况返回空
}

void PreOrderTraverse(BiTree T) {
	if(T == NULL) {
		return;
	}
	printf("%d ", T->data); // 显示结点数据，可以更改为其它对结点操作
	PreOrderTraverse(T->lchild); // 再先序遍历左子树
	PreOrderTraverse(T->rchild); // 最后先序遍历右子树
}

void InOrderTraverse(BiTree T) {
	if(T == NULL) {
		return;
	}
	InOrderTraverse(T->lchild); // 中序遍历左子树
	printf("%d ", T->data); // 显示结点数据，可以更改为其它对结点操作
	InOrderTraverse(T->rchild); // 最后中序遍历右子树
}

void PostOrderTraverse(BiTree T) {
	if(T == NULL) {
		return;
	}
	PostOrderTraverse(T->lchild); // 先后序遍历左子树
	PostOrderTraverse(T->rchild); // 再后序遍历右子树
	printf("%d ", T->data); // 显示结点数据，可以更改为其它对结点操作
}

void LevelOrderTraverse(BiTree T) {
	LinkQueue q;
	QElemType a;
	if(T) {
		InitQueue(&q); // 初始化队列q
		EnQueue(&q, T); // 根指针入队
		/* 队列不空 */
		while(!QueueEmpty(q)) {
			DeQueue(&q, &a); // 出队元素(指针)，赋值给a
			printf("%d ", a->data); // 访问a所指结点
			/* a有左孩子 */
			if(a->lchild != NULL) {
				EnQueue(&q, a->lchild); // 入队a的左孩子
			}
			/* a有右孩子 */
			if (a->rchild != NULL) {
				EnQueue(&q, a->rchild); // 入队a的右孩子
			}
		}
	}
}

int main(int argc, char** argv) {
	BiTree T; // 声明二叉树T
	TElemType e; // 声明数据元素e
 
	InitBiTree(&T);
	printf("请按先序输入结点的值(int)，0 表示空结点，输入 999 结束，结点数≤%d: ", MAXSIZE);
	CreateBiTree(&T);
	/* 测试样例数据:
	1 2 4 7 0 0 8 0 0 0 3 5 9 0 0 0 6 0 0 999
	1 2 4 8 0 0 9 0 0 5 0 0 3 6 0 0 7 0 0 999
	*/
 
	printf("二叉树 T 是否是空树: %d（0:否,1:空）\n", BiTreeEmpty(T));
	printf("二叉树 T 的树高: %d\n", BiTreeDepth(T));
	printf("二叉树 T 的树根: %d\n", Root(T));
 
	printf("\n层序遍历二叉树 T: ");
	LevelOrderTraverse(T);
	printf("\n先序遍历二叉树 T: ");
	PreOrderTraverse(T);
	printf("\n中序遍历二叉树 T: ");
	InOrderTraverse(T);
	printf("\n后序遍历二叉树 T: ");
	PostOrderTraverse(T);
 
	printf("\n\n给值为2的结点重新赋值为50:");
	Assign(T, 2, 50);
	printf("\n重新赋值后层序遍历二叉树 T: ");
	LevelOrderTraverse(T);
	printf("\n重新赋值后先序遍历二叉树 T: ");
	PreOrderTraverse(T);
	printf("\n重新赋值后中序遍历二叉树 T: ");
	InOrderTraverse(T);
	printf("\n重新赋值后后序遍历二叉树 T: ");
	PostOrderTraverse(T);
 
	e = 5;
	printf("\n值为%d 的结点的双亲是：%d（0 表示无）", e, Parent(T, e));
	printf("\n值为%d 的结点的左孩子是：%d（0 表示无）", e, LeftChild(T, e));
	printf("\n值为%d 的结点的右孩子是：%d（0 表示无）", e, RightChild(T, e));
	printf("\n值为%d 的结点的左兄弟是：%d（0 表示无）", e, LeftSibling(T, e));
	printf("\n值为%d 的结点的右兄弟是：%d（0 表示无）", e, RightSibling(T, e));
	
	ClearBiTree(&T);
	printf("\n清空二叉树 T 后，T 是否为空：%d（0:否，1:空）\n", BiTreeEmpty(T));
	DestroyBiTree(&T);
 
	return 0;
}

