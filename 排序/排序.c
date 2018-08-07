#include <stdio.h>    
#include <stdlib.h> // malloc、srand、rand
#include <time.h> // time
 
/* 状态码 */
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
 
#define MAXSIZE 20 // 用于要排序数组个数最大值，可根据需要修改
#define INTERVAL 4 // 随机数波动区间
#define LIMIT MAXSIZE * INTERVAL - 1 // 随机数范围
 
#define MAX_LENGTH_INSERT_SORT 7 // 用于快速排序时判断是否选用插入排序阙值

typedef int Status; // Status是函数的类型，其值是函数结果状态代码，如OK等
typedef int KeyType; // 关键字的数据类型，类型根据实际情况而定，这里假设为int
 
typedef struct {
	KeyType r[MAXSIZE + 1]; // 用于存储要排序数组，r[0]用作哨兵或暂存单元
	int length; // 用于记录顺序表的长度
}SqList;
 
void InsertSort(SqList *L); // 插入排序，OpQuickSort()调用
 
/* 交换L中数组r的下标为i和j的值，供其他函数调用 */
void swap(SqList *L, KeyType i, KeyType j) {
	KeyType temp = L->r[i];
	L->r[i] = L->r[j];
	L->r[j] = temp;
}
 
/* 堆排序HeapSort()调用 */
/* 已知L->r[s..m]中记录的关键字除L->r[s]之外均满足堆的定义， */
/* 本函数调整L->r[s]的关键字,使L->r[s..m]成为一个大顶堆 */
void HeapAdjust(SqList *L, KeyType s, KeyType m) {
	int temp, j;
	temp = L->r[s];
	/* 沿关键字较大的孩子结点向下筛选 */
	for(j = 2 * s; j <= m; j *= 2) {
		if(j < m && L->r[j] < L->r[j + 1]) {
			++j; // j为关键字中较大的记录的下标
		}
		if(temp >= L->r[j]) {
			break; // rc应插入在位置s上
		}
		L->r[s] = L->r[j];
		s = j;
	}
	L->r[s] = temp; // 插入
}
 
/* 递归归并排序内部接口，MSort()调用 */
/* 将有序的SR[i..m]和SR[m+1..n]归并为有序的TR[i..n] */
void Merge(KeyType SR[], KeyType TR[], KeyType i, KeyType m, KeyType n) {
	int j, k, l;
	/* 将SR中记录由小到大地并入TR */
	for(j = m + 1, k = i; i <= m && j <= n; k++) {
		if(SR[i] < SR[j]) {
			TR[k] = SR[i++];
		}
		else {
			TR[k] = SR[j++];
		}
	}
	if(i <= m) {
		for(l = 0; l <= m - i; l++) {
			TR[k + l] = SR[i + l]; // 将剩余的SR[i..m]复制到TR
		}
	}
	if(j <= n) {
		for(l = 0; l <= n - j; l++) {
			TR[k + l] = SR[j + l]; // 将剩余的SR[j..n]复制到TR
		}
	}
}
 
/* 递归归并排序ReMergeSort()调用 */
/* 将SR[s..t]归并排序为TR1[s..t] */
void MSort(KeyType SR[], KeyType TR1[], KeyType s, KeyType t) {
	KeyType m;
	KeyType TR2[MAXSIZE + 1];
	if(s == t) {
		TR1[s] = SR[s];
	}
	else {
		m = (s + t) / 2; // 将SR[s..t]平分为SR[s..m]和SR[m+1..t] 
		MSort(SR, TR2, s, m); // 递归地将SR[s..m]归并为有序的TR2[s..m]
		MSort(SR, TR2, m + 1, t); // 递归地将SR[m+1..t]归并为有序的TR2[m+1..t]
		Merge(TR2, TR1, s, m, t); // 将TR2[s..m]和TR2[m+1..t]归并到TR1[s..t]
	}
}
 
/* 非递归归并排序MergeSort()调用 */
/* 将SR[]中相邻长度为s的子序列两两归并到TR[] */
void MergePass(KeyType SR[], KeyType TR[], KeyType s, KeyType n) {
	int i = 1;
	int j;
	/* 两两归并 */
	while(i <= n - 2 * s + 1) {
		Merge(SR, TR, i, i + s - 1, i + 2 * s - 1);
		i = i + 2 * s;
	}
	/* 归并最后两个序列 */
	if(i < n - s + 1) {
		Merge(SR, TR, i, i + s - 1, n);
	}
	/* 若最后只剩下单个子序列 */
	else{
		for(j = i; j <= n; j++) {
			TR[j] = SR[j];
		}
	}
}
 
/* 经典快速排序内部接口，QSort()调用 */
/* 交换顺序表L中子表的记录，使枢轴记录到位，并返回其所在位置 */
/* 此时在它之前(后)的记录均不大(小)于它。 */
int Partition(SqList *L, int low, int high) {
	int pivotkey;
	pivotkey = L->r[low]; // 用子表的第一个记录作枢轴记录
	/* 从表的两端交替地向中间扫描 */
	while(low < high) {
		while(low < high && L->r[high] >= pivotkey) {
			high--;
		}
		swap(L, low, high); // 将比枢轴记录小的记录交换到低端
		while(low < high&&L->r[low] <= pivotkey) {
			low++;
		}
		swap(L, low, high); // 将比枢轴记录大的记录交换到高端
	}
	return low; // 返回枢轴所在位置
}
 
/* 经典快速排序QuickSort()调用 */
/* 对顺序表L中的子序列L->r[low..high]作快速排序 */
void QSort(SqList *L, int low, int high) {
	int pivot;
	if(low < high) {
		pivot = Partition(L, low, high); // 将L->r[low..high]一分为二，算出枢轴值pivot
		QSort(L, low, pivot - 1); // 对低子表递归排序
		QSort(L, pivot + 1, high); // 对高子表递归排序
	}
}
 
/* 优化快速排序优化内部接口，OpQSort()调用 */
int OpPartition(SqList *L, int low, int high) {
	int pivotkey;
	int m = low + (high - low) / 2; // 计算数组中间的元素的下标
	if(L->r[low] > L->r[high]) {
		swap(L, low, high); // 交换左端与右端数据，保证左端较小
	}
	if(L->r[m] > L->r[high]) {
		swap(L, high, m); // 交换中间与右端数据，保证中间较小
	}
	if(L->r[m] > L->r[low]) {
		swap(L, m, low); // 交换中间与左端数据，保证左端较小
	}
	pivotkey = L->r[low]; // 用子表的第一个记录作枢轴记录
	L->r[0] = pivotkey; // 将枢轴关键字备份到L->r[0]
	/* 从表的两端交替地向中间扫描 */
	while (low < high) {
		while(low < high && L->r[high] >= pivotkey) {
			high--;
		}
		L->r[low] = L->r[high];
		while(low < high && L->r[low] <= pivotkey) {
			low++;
		}
		L->r[high] = L->r[low];
	}
	L->r[low] = L->r[0];
	return low; // 返回枢轴所在位置
}
 
/* 优化快速排序OpQuickSort()调用 */
void OpQSort(SqList *L, int low, int high) {
	int pivot;
	if((high - low) > MAX_LENGTH_INSERT_SORT) {
		while(low < high) {
			pivot = OpPartition(L, low, high); // 将L->r[low..high]一分为二，算出枢轴值pivot
			OpQSort(L, low, pivot - 1); // 对低子表递归排序
										// QSort(L,pivot+1,high); // 对高子表递归排序
			low = pivot + 1; // 尾递归
		}
	}
	else {
		InsertSort(L);
	}
}
 
/* 打印函数，测试用 */
void Print(SqList L) {
	int i;
	for(i = 1; i <= L.length; i++) {
		printf("%d ", L.r[i]);
	}
	printf("\n");
}
 
/* 生成顺随机不重复数组，测试用 */
KeyType CreatArray(KeyType *a) {
	int i;
	/* 0 号元素不赋值 */
	for(i = 1; i <= MAXSIZE; i++) {
		a[i] = (rand() % (LIMIT - 0)) + 0;
	}
	return *a;
}

/* 对顺序表L作简单交换排序（简单冒泡排序）*/
void SwapSort(SqList *L) {
	int i, j;
	for(i = 1; i < L->length; i++) {
		for(j = i + 1; j <= L->length; j++) {
			if(L->r[i] > L->r[j]) {
				swap(L, i, j); // 交换L->r[i]与L->r[j]的值
			}
		}
	}
}

/* 对顺序表L作普通冒泡排序 */
void BubbleSort(SqList *L) {
	int i, j;
	for(i = 1; i < L->length; i++) {
		/* j是从后往前循环 */
		for(j = L->length - 1; j >= i; j--) {
			/* 若前者大于后者（注意这里与上一算法的差异）*/
			if(L->r[j] > L->r[j + 1]) {
				swap(L, j, j + 1); // 交换L->r[j]与L->r[j+1]的值
			}
		}
	}
}

/* 对顺序表L作标志位冒泡排序 */
void FlagBubbleSort(SqList *L) {
	int i, j;
	Status flag = TRUE; // flag用来作为标记
	/* 若flag为true说明有过数据交换，否则停止循环 */
	for(i = 1; i < L->length && flag; i++) {
		flag = FALSE; // 初始为False
		for(j = L->length - 1; j >= i; j--) {
			if(L->r[j] > L->r[j + 1]) {
				swap(L, j, j + 1); // 交换L->r[j]与L->r[j+1]的值
				flag = TRUE; // 如果有数据交换，则flag为true
			}
		}
	}
}

/* 对顺序表L作简单选择排序 */
void SelectSort(SqList *L) {
	int i, j, min;
	for(i = 1; i < L->length; i++) {
		min = i; // 将当前下标定义为最小值下标
		/* 循环之后的数据 */
		for(j = i + 1; j <= L->length; j++) {
			/* 如果有小于当前最小值的关键字 */
			if(L->r[min] > L->r[j]) {
				min = j; // 将此关键字的下标赋值给min
			}
		}
		/* 若min不等于i，说明找到最小值，交换 */
		if (i != min) {
			swap(L, i, min); // 交换L->r[i]与L->r[min]的值
		}
	}
}

/* 对顺序表L作直接插入排序 */
void InsertSort(SqList *L) {
	int i, j;
	for(i = 2; i <= L->length; i++) {
		/* 需将L->r[i]插入有序子表 */
		if(L->r[i] < L->r[i - 1]) {
			L->r[0] = L->r[i]; // 设置哨兵
			for(j = i - 1; L->r[j] > L->r[0]; j--) {
				L->r[j + 1] = L->r[j]; // 记录后移
			}
			L->r[j + 1] = L->r[0]; // 插入到正确位置
		}
	}
}

/* 对顺序表L作希尔排序 */
void ShellSort(SqList *L) {
	int i, j, k = 0;
	int increment = L->length;
	do {
		increment = increment / 3 + 1; // 增量序列
		for(i = increment + 1; i <= L->length; i++) {
			/* 需将L->r[i]插入有序增量子表 */
			if(L->r[i] < L->r[i - increment]) {
				L->r[0] = L->r[i]; // 暂存在L->r[0]
				for(j = i - increment; j > 0 && L->r[0] < L->r[j]; j -= increment) {
					L->r[j + increment] = L->r[j]; // 记录后移，查找插入位置
				}
				L->r[j + increment] = L->r[0]; // 插入
			}
		}
		//printf("	第%d趟排序结果:  ", ++k);
		//Print(*L);
	} while(increment > 1);
}

/* 对顺序表L进行堆排序 */
void HeapSort(SqList *L) {
	int i;
	/*  把L中的r构建成一个大根堆 */
	for(i = L->length / 2; i > 0; i--) {
		HeapAdjust(L, i, L->length);
	}
	for(i = L->length; i > 1; i--) {
		swap(L, 1, i); // 将堆顶记录和当前未经排序子序列的最后一个记录交换
		HeapAdjust(L, 1, i - 1); // 将L->r[1..i-1]重新调整为大根堆
	}
}

/* 对顺序表L作递归的归并排序 */
void ReMergeSort(SqList *L) {
	MSort(L->r, L->r, 1, L->length);
}

/* 对顺序表L作非递归的归并排序 */
void MergeSort(SqList *L) {
	KeyType* TR = (KeyType*)malloc(L->length * sizeof(KeyType)); // 申请额外空间
	int k = 1;
	while(k < L->length) {
		MergePass(L->r, TR, k, L->length);
		k = 2 * k; // 子序列长度加倍
		MergePass(TR, L->r, k, L->length);
		k = 2 * k; // 子序列长度加倍
	}
}

/* 对顺序表L作经典快速排序 */
void QuickSort(SqList *L) {
	QSort(L, 1, L->length);
}

/* 对顺序表L作优化快速排序 */
void OpQuickSort(SqList *L) {
	OpQSort(L, 1, L->length);
}

int main(int argc, char** argv) {
	int d[MAXSIZE + 1]; // 声明数组
	SqList l0, l1, l2, l3, l4, l5, l6, l7, l8, l9, l10; // 声明顺序表
	int i; // 声明临时变量
 
	srand((unsigned int)time(NULL)); //设置随机数种子
 
	*d = CreatArray(d); // 随机数组生成器
	for (i = 1; i <= MAXSIZE; i++) {
		l0.r[i] = d[i];
	}
	l0.length = MAXSIZE;
	l1 = l2 = l3 = l4 = l5 = l6 = l7 = l8 = l9 = l10 = l0;
	printf("排序前：");
	Print(l0);
	printf("\n");
 
	printf("交换排序:\n");
	SwapSort(&l0);
	Print(l0);
 
	printf("冒泡排序:\n");
	BubbleSort(&l1);
	Print(l1);
 
	printf("带标志冒泡排序:\n");
	FlagBubbleSort(&l2);
	Print(l2);
 
	printf("选择排序:\n");
	SelectSort(&l3);
	Print(l3);
 
	printf("直接插入排序:\n");
	InsertSort(&l4);
	Print(l4);
 
	printf("希尔排序:\n");
	ShellSort(&l5);
	Print(l5);
 
	printf("堆排序:\n");
	HeapSort(&l6);
	Print(l6);
 
	printf("归并排序（递归）:\n");
	ReMergeSort(&l7);
	Print(l7);
 
	printf("归并排序（非递归）:\n");
	MergeSort(&l8);
	Print(l8);
 
	printf("快速排序:\n");
	QuickSort(&l9);
	Print(l9);
 
	printf("优化快速排序:\n");
	OpQuickSort(&l10);
	Print(l10);
 
	return 0;
}

