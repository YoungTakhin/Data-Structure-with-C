#include <stdio.h>   
#include <stdlib.h> // malloc、free、srand、rand
#include <time.h> // time
 
/* 状态码 */
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
 
#define MAXSIZE 10 // 存储空间初始分配量
#define INTERVAL 3 // 随机数波动区间
#define LIMIT MAXSIZE * INTERVAL - 1 // 随机数范围

typedef int Status; // Status是函数的类型,其值是函数结果状态代码，如OK等
typedef int KeyType; // 关键字的数据类型，类型根据实际情况而定，这里假设为 int
 
/* 定义二叉树二叉链表结点结构 */
typedef struct BiTNode {
	int data; // 结点数据
	struct BiTNode *lchild, *rchild; // 左右孩子指针
} BiTNode, *BiTree;
 
/* 从二叉排序树中删除结点p，并重接它的左或右子树，供DeleteBST()调用 */
Status Delete(BiTree *p) {
	BiTree q, s;
	/* 右子树空则只需重接它的左子树（待删结点是叶子也走此分支) */
	if((*p)->rchild == NULL) {
		q = *p;
		*p = (*p)->lchild;
		free(q);
	}
	/* 只需重接它的右子树 */
	else if((*p)->lchild == NULL) {
		q = *p;
		*p = (*p)->rchild;
		free(q);
	}
	/* 左右子树均不空 */
	else {
		q = *p;
		s = (*p)->lchild;
		/* 转左，然后向右到尽头（找待删结点的前驱） */
		while(s->rchild) {
			q = s;
			s = s->rchild;
		}
		(*p)->data = s->data; // s指向被删结点的直接前驱（将被删结点前驱的值取代被删结点的值）
		if(q != *p) {
			q->rchild = s->lchild; // 重接q的右子树
		}
		else {
			q->lchild = s->lchild; // 重接q的左子树
		}
		free(s);
	}
	return TRUE;
}
 
/* 初始条件: 二叉树T存在 */
/* 操作结果: 前序递归遍历T */
Status PreOrderTraverse(BiTree T) {
	if(T == NULL) {
		return ERROR;
	}
	printf("%d ", T->data); // 显示结点数据
	PreOrderTraverse(T->lchild); // 再先序遍历左子树
	PreOrderTraverse(T->rchild); // 最后先序遍历右子树
	return OK;
}
 
/* 初始条件: 二叉树T存在 */
/* 操作结果: 中序递归遍历T */
Status InOrderTraverse(BiTree T) {
	if(T == NULL) {
		return ERROR;
	}
	InOrderTraverse(T->lchild); // 中序遍历左子树
	printf("%d ", T->data); // 显示结点数据
	InOrderTraverse(T->rchild); // 最后中序遍历右子树
	return OK;
}
 
/* 初始条件: 二叉树T存在 */
/* 操作结果: 后序递归遍历T */
Status PostOrderTraverse(BiTree T) {
	if(T == NULL) {
		return ERROR;
	}
	PostOrderTraverse(T->lchild); // 先后序遍历左子树
	PostOrderTraverse(T->rchild); // 再后序遍历右子树
	printf("%d ", T->data); // 显示结点数据，可以更改为其它对结点操作
	return OK;
}
 
/* 生成顺随机不重复数组，测试用 */
KeyType CreatArray(KeyType *a) {
	int i;
	KeyType tmp;
	int flag = 0;
	int t = 0;
	/* 随机数的个数不能超出 MAXSIZE 和 LIMIT 之间的范围，不然会导致死循 */
	while(1) {
		flag = 0;
		if (t == MAXSIZE) {
			break;
		}
		tmp = (rand() % (LIMIT - 0)) + 0;
		for(i = 0; i < t; i++) {
			if(a[i] == tmp) {
				flag = 1;
			}
		}
		if (flag != 1) {
			a[t] = tmp;
			t++;
		}
	}
	return *a;
}

/* 递归查找二叉排序树T中是否存在key */
/* 指针f指向T的双亲，其初始调用值为NULL
若查找成功，则指针p指向该数据元素结点，并返回TRUE;
否则指针p指向查找路径上访问的最后一个结点并返回FALSE */
Status SearchBST(BiTree T, int key, BiTree f, BiTree *p) {
	/* 查找不成功 */
	if(!T) {
		*p = f;
		return FALSE;
	}
	/* 查找成功 */
	else if(key == T->data) {
		*p = T;
		return TRUE; // 递归出口
	}
	else if(key < T->data) {
		return SearchBST(T->lchild, key, T, p); // 在左子树中继续查找 
	}
	else {
		return SearchBST(T->rchild, key, T, p); // 在右子树中继续查找
	}
}
 
/* 当二叉排序树T中不存在关键字等于key的数据元素时 */
/* 插入key并返回TRUE，否则返回FALSE */
Status InsertBST(BiTree *T, KeyType key) {
	BiTree p, s;
	/* 查找不成功 */
	if(!SearchBST(*T, key, NULL, &p)) {
		s = (BiTree)malloc(sizeof(BiTNode));
		s->data = key;
		s->lchild = s->rchild = NULL;
		if(!p) {
			*T = s; // 插入s为新的根结点
		}
		else if(key < p->data) {
			p->lchild = s; // 插入s为左孩子
		}
		else {
			p->rchild = s; // 插入s为右孩子
		}
		return TRUE;
	}
	else {
		return FALSE; // 树中已有关键字相同的结点，不再插入
	}
}

/* 若二叉排序树T中存在关键字等于key的数据元素时，则删除该数据元素结点, */
/* 并返回TRUE；否则返回FALSE。 */
Status DeleteBST(BiTree *T, KeyType key) {
	/* 不存在关键字等于key的数据元素 */
	if(!*T) {
		return FALSE;
	}
	else {
		/* 找到关键字等于key的数据元素 */
		if(key == (*T)->data) {
			return Delete(T);
		}
		else if(key < (*T)->data) {
			return DeleteBST(&(*T)->lchild, key);
		}
		else {
			return DeleteBST(&(*T)->rchild, key);
		}
	}
}

int main(int argc, char** argv) {
	BiTree T = NULL, f = NULL, p; // 声明BST结点
	KeyType a[MAXSIZE]; // 声明数组
	KeyType k; // 声明关键字k
	int i; // 声明临时变量
 
	srand((unsigned int)time(NULL)); //设置随机数种子
 
	*a = CreatArray(a);
	printf("随机生成的不重复数组：");
	for (i = 0; i < MAXSIZE; i++) {
		printf("%d ", a[i]);
	}
 
	for(i = 0; i<MAXSIZE; i++) {
		InsertBST(&T, a[i]); // 将随机生成的数组插入到BST树里
	}
	printf("\n\n插入后的BST前序遍历：");
	PreOrderTraverse(T);
	printf("\n插入后的BST中序遍历：");
	InOrderTraverse(T);
	printf("\n插入后的BST后序遍历：");
	PostOrderTraverse(T);
	printf("\n");
 
	printf("请输入要删除的数据:");
	scanf("%d", &k);
	DeleteBST(&T, k);
	printf("删除 %d 后的BST前序遍历：", k);
	PreOrderTraverse(T);
	printf("\n删除 %d 后的BST中序遍历：", k);
	InOrderTraverse(T);
	printf("\n删除 %d 后的BST后序遍历：", k);
	PostOrderTraverse(T);
	printf("\n");
 
	printf("请输入要插入的数据:");
	scanf("%d", &k);
	InsertBST(&T, k);
	printf("插入 %d 后的BST前序遍历：", k);
	PreOrderTraverse(T);
	printf("\n插入 %d 后的BST中序遍历：", k);
	InOrderTraverse(T);
	printf("\n插入 %d 后的BST后序遍历：", k);
	PostOrderTraverse(T);
	printf("\n");

	printf("\n请输入要查找的数据元素：");
	scanf("%d", &k);
	i = SearchBST(T, k, f, &p);
	if(i == TRUE) {
		printf(" %d 在树里\n", p->data);
	}
	else {
		printf("不在树里\n");
	}

	return 0;
}

