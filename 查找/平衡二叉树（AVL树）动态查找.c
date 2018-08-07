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
 
/* AVL树的平衡因子 */
#define LH +1 // 左高
#define EH 0 // 等高
#define RH -1 // 右高
 
typedef int Status; // Status是函数的类型,其值是函数结果状态代码，如OK等
typedef int KeyType; // 关键字的数据类型，类型根据实际情况而定，这里假设为int
 
/* 二叉树的二叉链表结点结构定义 */
typedef  struct BiTNode {
	KeyType data; // 结点数据
	int bf; // 结点的平衡因子
	struct BiTNode *lchild, *rchild; // 左右孩子指针
} BiTNode, *BiTree;
 
/* 对以P为根的二叉排序树作左旋处理，处理之后P指向新的树根结点，
	即旋转处理之前的右子树的根结点0  */
void L_Rotate(BiTree *P) {
	BiTree R;
	R = (*P)->rchild; // R指向P的右子树根结点
	(*P)->rchild = R->lchild; // R的左子树挂接为P的右子树
	R->lchild = (*P);
	*P = R; // P指向新的根结点
}
 
/* 对以p为根的二叉排序树作右旋处理，处理之后p指向新的树根结点，
	即旋转处理之前的左子树的根结点 */
void R_Rotate(BiTree *P) {
	BiTree L;
	L = (*P)->lchild; // L指向P的左子树根结点
	(*P)->lchild = L->rchild; // L的右子树挂接为P的左子树
	L->rchild = (*P);
	*P = L; // P指向新的根结点
}
 
/* 对以指针T所指结点为根的二叉树作左平衡旋转处理，
	本算法结束时，指针T指向新的根结点 */
void LeftBalance(BiTree *T) {
	BiTree L, Lr;
	L = (*T)->lchild; // L指向T的左子树根结点
	/*  检查T的左子树的平衡度，并作相应平衡处理 */
	switch(L->bf) {
	case LH: // 新结点插入在T的左孩子的左子树上，要作单右旋处理
		(*T)->bf = EH;
		L->bf = EH;
		R_Rotate(T);
		break;
	case EH:
		(*T)->bf = LH;
		L->bf = RH;
		R_Rotate(T);
		break;
	case RH: // 新结点插入在T的左孩子的右子树上，要作双旋处理
		Lr = L->rchild; // Lr指向T的左孩子的右子树根
		/* 修改T及其左孩子的平衡因子 */
		switch(Lr->bf) {
		case LH: 
			(*T)->bf = RH;
			L->bf = EH;
			break;
		case EH: 
			(*T)->bf = EH;
			L->bf = EH;
			break;
		case RH: 
			(*T)->bf = EH;
			L->bf = LH;
			break;
		default:
			break;
		}
		Lr->bf = EH;
		L_Rotate(&(*T)->lchild); // 对T的左子树作左旋平衡处理
		R_Rotate(T); // 对T作右旋平衡处理
		break;
	default:
		break;
	}
}
 
/* 对以指针T所指结点为根的二叉树作右平衡旋转处理，
	本算法结束时，指针T指向新的根结点 */
void RightBalance(BiTree *T) {
	BiTree R, Rl;
	R = (*T)->rchild; // R指向T的右子树根结点
	/* 检查T的右子树的平衡度，并作相应平衡处理 */
	switch(R->bf) {
	case RH: // 新结点插入在T的右孩子的右子树上，要作单左旋处理
		(*T)->bf = EH;
		R->bf = EH;
		L_Rotate(T);
		break;
	case EH:
		(*T)->bf = RH;
		R->bf = LH;
		L_Rotate(T);
		break;
	case LH: // 新结点插入在T的右孩子的左子树上，要作双旋处理
		Rl = R->lchild; // Rl指向T的右孩子的左子树根
		/* 修改T及其右孩子的平衡因子 */
		switch(Rl->bf) {
		case RH:
			(*T)->bf = LH;
			R->bf = EH;
			break;
		case EH:
			(*T)->bf = EH;
			R->bf = EH;
			break;
		case LH:
			(*T)->bf = EH;
			R->bf = RH;
			break;
		default:
			break;
		}
		Rl->bf = EH;
		R_Rotate(&(*T)->rchild); // 对T的右子树作右旋平衡处理
		L_Rotate(T); // 对T作左旋平衡处理
		break;
	default:
		break;
	}
}
 
/* 寻找最小元素，供DeleteAVL()调用 */
KeyType MinElement(BiTNode *p) {
	if(p->lchild) {
		return MinElement(p->lchild);
	}
	else {
		return p->data;
	}
}
 
/* 生成顺随机不重复数组，测试用 */
KeyType CreatArray(KeyType *a) {
	int i;
	KeyType tmp;
	int flag = 0;
	int t = 0;
	/* 随机数的个数不能超出 MAXSIZE 和 LIMIT 之间的范围，不然会导致死循 */
	while (1) {
		flag = 0;
		if (t == MAXSIZE) {
			break;
		}
		tmp = (rand() % (LIMIT - 0)) + 0;
		for (i = 0; i < t; i++) {
			if (a[i] == tmp) {
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
 
/* 初始条件: 二叉树 T 存在 */
/* 操作结果: 前序递归遍历 T */
Status PreOrderTraverse(BiTree T) {
	if (T == NULL) {
		return ERROR;
	}
	printf("%d ", T->data); // 显示结点数据
	PreOrderTraverse(T->lchild); // 再先序遍历左子树
	PreOrderTraverse(T->rchild); // 最后先序遍历右子树
	return OK;
}
 
/* 初始条件: 二叉树 T 存在 */
/* 操作结果: 中序递归遍历 T */
Status InOrderTraverse(BiTree T) {
	if (T == NULL) {
		return ERROR;
	}
	InOrderTraverse(T->lchild); // 中序遍历左子树
	printf("%d ", T->data); // 显示结点数据
	InOrderTraverse(T->rchild); // 最后中序遍历右子树
	return OK;
}
 
/* 初始条件: 二叉树 T 存在 */
/* 操作结果: 后序递归遍历 T */
Status PostOrderTraverse(BiTree T) {
	if (T == NULL) {
		return ERROR;
	}
	PostOrderTraverse(T->lchild); // 先后序遍历左子树
	PostOrderTraverse(T->rchild); // 再后序遍历右子树
	printf("%d ", T->data); // 显示结点数据，可以更改为其它对结点操作
	return OK;
}

/* 查找AVL树T中是否存在k */
Status SearchAVL(BiTree T, KeyType k) {
	BiTree p = T;
	while (p) {
		if (p->data == k) {
			return TRUE;
		}
		else if (p->data < k) {
			p = p->rchild;
		}
		else {
			p = p->lchild;
		}
	}
	return FALSE;
}

/* 若在平衡的二叉排序树T中不存在和key有相同关键字的结点，
	则插入一个数据元素值为key的新结点，并返回TRUE，否则返回FALSE。
	若因插入而使二叉排序树失去平衡，则作平衡旋转处理，变量taller反映T长高与否 */
Status InsertAVL(BiTree *T, KeyType key, Status *taller) {
	/* 插入新结点，树"长高"，置taller为TRUE */
	if(!*T) {
		*T = (BiTree)malloc(sizeof(BiTNode));
		(*T)->data = key;
		(*T)->lchild = NULL;
		(*T)->rchild = NULL;
		(*T)->bf = EH;
		*taller = TRUE;
		return TRUE;
	}
	else {
		/* 树中已存在和e有相同关键字的结点则不再插入 */
		if(key == (*T)->data) {
			*taller = FALSE;
			return FALSE;
		}
		/* 应继续在T的左子树中进行搜索 */
		if(key < (*T)->data) {
			/* 未插入 */
			if(!InsertAVL(&(*T)->lchild, key, taller)) {
				return FALSE;
			}
			/* 已插入到T的左子树中且左子树"长高" */
			if(*taller) {
				/* 检查T的平衡度 */
				switch((*T)->bf) {
				case LH: // 原本左子树比右子树高，需要作左平衡处理
					LeftBalance(T);
					*taller = FALSE;
					break;
				case EH: // 原本左、右子树等高，现因左子树增高而使树增高
					(*T)->bf = LH;
					*taller = TRUE;
					break;
				case RH: // 原本右子树比左子树高，现左、右子树等高
					(*T)->bf = EH;
					*taller = FALSE;
					break;
				default:
					break;
				}
			}
		}
		/* 应继续在T的右子树中进行搜索 */
		else {
			/* 未插入 */
			if(!InsertAVL(&(*T)->rchild, key, taller)) {
				return FALSE;
			}
			/* 已插入到T的右子树且右子树"长高" */
			if(*taller) {
				/* 检查T的平衡度 */
				switch((*T)->bf) {
				case LH: // 原本左子树比右子树高，现左、右子树等高
					(*T)->bf = EH;
					*taller = FALSE;
					break;
				case EH: // 原本左、右子树等高，现因右子树增高而使树增高
					(*T)->bf = RH;
					*taller = TRUE;
					break;
				case RH: // 原本右子树比左子树高，需要作右平衡处理
					RightBalance(T);
					*taller = FALSE;
					break;
				default:
					break;
				}
			}
		}
	}
	return TRUE;
}

/* 若在平衡的二叉排序树T中存在和key有相同关键字的结点，
	则删除数据元素值为key的结点，并返回TRUE，否则返回FALSE。
	若因删除而使二叉排序树失去平衡，则作平衡旋转处理，变量shorter反映T变矮与否 */
Status DeleteAVL(BiTree *T, KeyType key, Status *shorter) {
	/* 不存在该元素 */
	if(*T == NULL) {
		return FALSE; // 删除失败   
	}
	/* 待删结点小于当前结点 */
	if(key < (*T)->data) {
		DeleteAVL(&((*T)->lchild), key, shorter);
		/* 已删除结点，当左子树变短时，进行平衡判断 */
		if(*shorter) {
			switch ((*T)->bf) {
			case LH: // 原来左高，现在左子树-1，所以等高
				(*T)->bf = EH;
				*shorter = TRUE;
				break;
			case EH: // 原来等高，现在左子树-1，所以右高
				(*T)->bf = RH;
				*shorter = FALSE;
				break;
			case RH: // 原来右高，现在左子树-1，所以需要左旋，调整平衡
				LeftBalance(T);
				*shorter = TRUE;
				break;
			default:
				break;
			}
		}
	}
	/*待删结点大于当前结点*/
	else if(key > (*T)->data) {
		DeleteAVL(&((*T)->rchild), key, shorter);
		/* 已删除结点，当左子树变短时，进行平衡判断 */
		if(*shorter) {
			switch((*T)->bf) {
			case LH: // 原来左高，现在右子树-1，所以需要右旋，调整平衡
				LeftBalance(T);
				*shorter = TRUE;
				break;
			case EH: // 原来等高，现在右子树-1，所以左高
				(*T)->bf = LH;
				*shorter = FALSE;
				break;
			case RH: // 原来右高，现在右子树-1，所以等高
				(*T)->bf = EH;
				*shorter = TRUE;
				break;
			default:
				break;
			}
		}
	}
	/* 待删结点等于当前结点 */
	else {
		/* 如果待删结点有右子树 */
		if((*T)->rchild) {
			(*T)->data = MinElement((*T)->rchild);
			DeleteAVL(&((*T)->rchild), (*T)->data, shorter);
			/* 已删除结点，当右子树变短时，进行平衡判断 */
			if(shorter) {
				switch((*T)->bf) {
				case LH: // 原来左高，现在右子树-1，所以需要右旋，调整平衡
					LeftBalance(T);
					*shorter = TRUE;
					break;
				case EH: // 原来等高，现在右子树-1，所以左高
					(*T)->bf = LH;
					*shorter = FALSE;
					break;
				case RH: // 原来右高，现在右子树-1，所以等高
					(*T)->bf = EH;
					*shorter = TRUE;
					break;
				default:
					break;
				}
			}
		}
		/* 若当前结点仅有左子树，则该结点只可能在倒数第二层，无需判断是否失衡 */
		else if((*T)->lchild) {
			(*T)->data = (*T)->lchild->data;
			(*T)->lchild = NULL;
			free((*T)->lchild);
		}
		/* 该结点为叶子结点 */
		else {
			free(*T);
			*T = NULL;
			*shorter = TRUE;
		}
	}
	return TRUE;
}

int main(int argc, char** argv) {
	BiTree T = NULL; // 声明二叉树T
	KeyType a[MAXSIZE]; // 声明数组
	KeyType k; // 声明关键字k
	Status taller, shorter; // 声明树高变化变量
	int i; // 声明临时变量
 
	srand((unsigned int)time(NULL)); //设置随机数种子
	
	*a = CreatArray(a);
	printf("随机生成的不重复数组：");
	for (i = 0; i < MAXSIZE; i++) {
		printf("%d ", a[i]);
	}
 
	for (i = 0; i < MAXSIZE; i++) {
		InsertAVL(&T, a[i], &taller);
	}
	printf("\n\n插入后的 AVL树 前序遍历：");
	PreOrderTraverse(T);
	printf("\n插入后的 AVL树 中序遍历：");
	InOrderTraverse(T);
	printf("\n插入后的 AVL树 后序遍历：");
	PostOrderTraverse(T);
	printf("\n");
 
	printf("\n请输入要删除的结点：");
	scanf("%d", &k);
	DeleteAVL(&T, k, &shorter);
	printf("删除 %d 后的 AVL树 前序遍历：", k);
	PreOrderTraverse(T);
	printf("\n删除 %d 后的 AVL树 中序遍历：", k);
	InOrderTraverse(T);
	printf("\n删除 %d 后的 AVL树 后序遍历：", k);
	PostOrderTraverse(T);
	printf("\n");
 
	printf("\n请输入要插入的数据元素：");
	scanf("%d", &k);
	InsertAVL(&T, k, &taller);
	printf("插入 %d 后的 AVL树 前序遍历：", k);
	PreOrderTraverse(T);
	printf("\n插入 %d 后的 AVL树 中序遍历：", k);
	InOrderTraverse(T);
	printf("\n插入 %d 后的 AVL树 后序遍历：", k);
	PostOrderTraverse(T);
	printf("\n");

	printf("\n请输入要查找的数据元素：");
	scanf("%d", &k);
	i = SearchAVL(T, k);
	if(i == TRUE) {
		printf(" %d 在树里\n", k);
	}
	else {
		printf("不在树里\n");
	}

	return 0;
}

