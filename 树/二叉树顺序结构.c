#include <stdio.h>
#include <math.h> // powl
 
/* 状态码 */
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
 
#define MAXSIZE 100 // 存储空间初始分配量
#define MAX_TREE_SIZE 100 // 二叉树的最大结点数
 
typedef int Status; // Status是函数的类型,其值是函数结果状态代码，如OK等
typedef int TElemType; // QElemType类型根据实际情况而定，这里假设为int
 
typedef TElemType SqBiTree[MAX_TREE_SIZE]; // 定义二叉树，0号单元存储根结点
 
/* 定义二叉树属性 */
typedef struct {
	int level, order; // 结点的层,本层序号(按满二叉树计算)
}Position;
 
TElemType Nil = 0; // 定义整型以0为空
 
/* 先序遍历PreOrderTraverse()调用 */
void PreTraverse(SqBiTree T, TElemType e) {
	printf("%d ", T[e]);
	/* 左子树不空 */
	if(T[2 * e + 1] != Nil) {
		PreTraverse(T, 2 * e + 1);
	}
	/* 右子树不空 */
	if(T[2 * e + 2] != Nil) {
		PreTraverse(T, 2 * e + 2);
	}
}
 
/* 中序遍历InOrderTraverse()调用 */
void InTraverse(SqBiTree T, TElemType e) {
	/* 左子树不空 */
	if(T[2 * e + 1] != Nil) {
		InTraverse(T, 2 * e + 1);
	}
	printf("%d ", T[e]);
	/* 右子树不空 */
	if(T[2 * e + 2] != Nil) {
		InTraverse(T, 2 * e + 2);
	}
}
 
/* 后序遍历PostOrderTraverse()调用 */
void PostTraverse(SqBiTree T, TElemType e) {
	/* 左子树不空 */
	if(T[2 * e + 1] != Nil) {
		PostTraverse(T, 2 * e + 1);
	}
	/* 右子树不空 */
	if(T[2 * e + 2] != Nil) {
		PostTraverse(T, 2 * e + 2);
	}
	printf("%d ", T[e]);
}

Status InitBiTree(SqBiTree T) {
	int i;
	for(i = 0; i < MAX_TREE_SIZE; i++) {
		T[i] = Nil; // 初值为空
	}
	return OK;
}

Status CreateBiTree(SqBiTree T) {
	int i = 0;
	printf("请按层序输入结点的值(int)，0表示空结点，输入999结束，结点数≤%d: ", MAX_TREE_SIZE);
	while(1) {
		scanf("%d", &T[i]);
		if(T[i] == 999) {
			T[i] = Nil;
			break;
		}
		i++;
	}
	for(i = 1; i < MAX_TREE_SIZE; i++) {
		/* 此非根结点(不空)无双亲 */
		if(T[(i + 1) / 2 - 1] == Nil && T[i] != Nil) {
			printf("出现无双亲的非根结点%d\n", T[i]);
			return ERROR;
		}
	}
	return OK;
}

Status ClearBiTree(SqBiTree T) {
	int i;
	for(i = 0; i < MAX_TREE_SIZE; i++) {
		T[i] = Nil; // 值置空
	}
	return OK;
}

Status BiTreeEmpty(SqBiTree T) {
	/* 根结点为空,则树空 */
	if(T[0] == Nil) {
		return TRUE;
	}
	return FALSE;
}

int BiTreeDepth(SqBiTree T) {
	int i, j = -1;
	/* 找到最后一个结点 */
	for(i = MAX_TREE_SIZE - 1; i >= 0; i--) {
		if(T[i] != Nil) {
			break;
		}
	}
	i++;
	do {
		j++;
	}
	/* 计算2的j次幂 */
	while(i >= powl(2, j));
	return j;
}

Status Root(SqBiTree T, TElemType *e) {
	/* T空 */
	if(BiTreeEmpty(T)) {
		return ERROR;
	}
	else {
		*e = T[0];
		return OK;
	}
}

TElemType Value(SqBiTree T, Position e) {
	return T[(int)powl(2, e.level - 1) + e.order - 2];
}

Status Assign(SqBiTree T, Position e, TElemType value) {
	int i = (int)powl(2, e.level - 1) + e.order - 2; // 将层、本层序号转为矩阵的序号
	/* 给叶子赋非空值但双亲为空 */
	if(value != Nil && T[(i + 1) / 2 - 1] == Nil) {
		return ERROR;
	}
	/* 给双亲赋空值但有叶子(不空) */
	else if(value == Nil && (T[i * 2 + 1] != Nil || T[i * 2 + 2] != Nil)) {
		return ERROR;
	}
	T[i] = value;
	return OK;
}

TElemType Parent(SqBiTree T, TElemType e) {
	int i;
	/* 空树 */
	if(T[0] == Nil) {
		return Nil;
	}
	for(i = 1; i <= MAX_TREE_SIZE - 1; i++) {
		/* 找到e */
		if(T[i] == e) {
			return T[(i + 1) / 2 - 1];
		}
	}
	return Nil; // 没找到e
}

TElemType LeftChild(SqBiTree T, TElemType e) {
	int i;
	/* 空树 */
	if(T[0] == Nil) {
		return Nil;
	}
	for(i = 0; i <= MAX_TREE_SIZE - 1; i++) {
		/* 找到e */
		if(T[i] == e) {
			return T[i * 2 + 1];
		}
	}
	return Nil; // 没找到e
}

TElemType RightChild(SqBiTree T, TElemType e) {
	int i;
	/* 空树 */
	if(T[0] == Nil) {
		return Nil;
	}
	for(i = 0; i <= MAX_TREE_SIZE - 1; i++) {
		/* 找到e */
		if(T[i] == e) {
			return T[i * 2 + 2];
		}
	}
	return Nil; // 没找到e
}

TElemType LeftSibling(SqBiTree T, TElemType e) {
	int i;
	/* 空树 */
	if(T[0] == Nil) {
		return Nil;
	}
	for(i = 1; i <= MAX_TREE_SIZE - 1; i++) {
		/* 找到e且其序号为偶数(是右孩子) */
		if(T[i] == e && i % 2 == 0) {
			return T[i - 1];
		}
	}
	return Nil; // 没找到e
}

TElemType RightSibling(SqBiTree T, TElemType e) {
	int i;
	/* 空树 */
	if(T[0] == Nil) {
		return Nil;
	}
	for(i = 1; i <= MAX_TREE_SIZE - 1; i++) {
		/* 找到e且其序号为奇数(是左孩子) */
		if(T[i] == e && i % 2) {
			return T[i + 1];
		}
	}
	return Nil; // 没找到e
}

Status PreOrderTraverse(SqBiTree T) {
	/* 树不空 */
	if(!BiTreeEmpty(T)) {
		PreTraverse(T, 0);
	}
	printf("\n");
	return OK;
}

Status InOrderTraverse(SqBiTree T) {
	/* 树不空 */
	if(!BiTreeEmpty(T)) {
		InTraverse(T, 0);
	}
	printf("\n");
	return OK;
}

Status PostOrderTraverse(SqBiTree T) {
	/* 树不空 */
	if(!BiTreeEmpty(T)) {
		PostTraverse(T, 0);
	}
	printf("\n");
	return OK;
}

Status LevelOrderTraverse(SqBiTree T)  {
	int i = MAX_TREE_SIZE - 1, j;
	while(T[i] == Nil) {
		i--; // 找到最后一个非空结点的序号
	}
	/* 从根结点起,按层序遍历二叉树 */
	for(j = 0; j <= i; j++) {
		if (T[j] != Nil){
			printf("%d ", T[j]); // 只遍历非空的结点
		}
	}
	printf("\n");
	return OK;
}

void PrintBiTree(SqBiTree T) {
	int j, k;
	Position p;
	TElemType e;
	printf("\n");
	for(j = 1; j <= BiTreeDepth(T); j++) {
		printf("第%d层:\t", j);
		for(k = 1; k <= powl(2, j - 1); k++) {
			p.level = j;
			p.order = k;
			e = Value(T, p);
			if(e != Nil) {
				printf("%d:%d ", k, e);
			}
		}
		printf("\n");
	}
}

int main(int argc, char** argv) {
	SqBiTree T; // 声明二叉树T
	TElemType e; // 声明数据元素e
	Position p; // 声明二叉树属性p
 
	InitBiTree(T);
	CreateBiTree(T);
	/* 测试样例数据:
		1 2 3 4 0 5 6 7 8 0 0 9 999
		1 2 3 4 5 6 7 8 9 999
	*/
 
	printf("二叉树T是否是空树: %d（0:否,1:空）\n", BiTreeEmpty(T));
	printf("二叉树T的树高: %d\n", BiTreeDepth(T));
	Root(T, &e);
	printf("二叉树T的树根: %d\n", e);
 
	printf("层序遍历二叉树T: ");
	LevelOrderTraverse(T);
	printf("先序遍历二叉树T: ");
	PreOrderTraverse(T);
	printf("中序遍历二叉树T: ");
	InOrderTraverse(T);
	printf("后序遍历二叉树T: ");
	PostOrderTraverse(T);
	printf("逐层逐结点画出二叉树T: ");
	PrintBiTree(T);
 
	printf("请输入结点位置(层数 第几个): ");
	scanf("%d %d", &p.level, &p.order);
	/* 测试样例数据:
		1 2
		4 2
	*/
	printf("该结点值: %d\n", Value(T, p));
 
	/* 为了测试方便，直接引用了上边的赋值 */
	Assign(T, p, 50);
	printf("重新赋值后层序遍历二叉树T: ");
	LevelOrderTraverse(T);
	printf("重新赋值后先序遍历二叉树T: ");
	PreOrderTraverse(T);
	printf("重新赋值后中序遍历二叉树T: ");
	InOrderTraverse(T);
	printf("重新赋值后后序遍历二叉树T: ");
	PostOrderTraverse(T);
	printf("重新赋值后逐层逐结点画出二叉树T: ");
	PrintBiTree(T);
 
	e = 5;
	printf("值为%d的结点的双亲是：%d（0表示无）\n", e, Parent(T, e));
	printf("值为%d的结点的左孩子是：%d（0表示无）\n", e, LeftChild(T, e));
	printf("值为%d的结点的右孩子是：%d（0表示无）\n", e, RightChild(T, e));
	printf("值为%d的结点的左兄弟是：%d（0表示无）\n", e, LeftSibling(T, e));
	printf("值为%d的结点的右兄弟是：%d（0表示无）\n", e, RightSibling(T, e));
 
	ClearBiTree(T);
	printf("清空二叉树T后，T是否为空：%d（0:否，1:空）\n", BiTreeEmpty(T));
 
	return 0;
}
 
