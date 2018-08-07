#include <stdio.h>
#include <stdlib.h> // malloc、srand、rand
#include <string.h> // memcpy
#include <time.h> // time
 
/* 状态码 */
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
 
#define MAXSIZE 50 // 存储空间初始分配量
#define INTERVAL 2 // 随机数波动区间
#define LIMIT MAXSIZE * INTERVAL - 1 // 随机数范围
 
typedef int Status; // Status是函数的类型，其值是函数结果状态代码，如OK等
typedef int KeyType; // 关键字的数据类型，类型根据实际情况而定，这里假设为int
 
/* 斐波那契函数，将斐波那契数列赋值到a数组，供Interpolation_Search()函数使用 */
Status Fibonacci(int *a) {
	int i;
	a[0] = 0;
	a[1] = 1;
	for(i = 2; i < MAXSIZE; ++i) {
		a[i] = a[i - 1] + a[i - 2];
	}
	return OK;
}
 
/* 生成顺序随机数组，测试用 */
int CreatSeqArray(int *a) {
	/*随机产生长度为LENGTH，数据范围在0~LIMIT的整形数组*/
	int i;
	for(i = 0; i < MAXSIZE; i++) {
		a[i] = rand() % INTERVAL + (i * INTERVAL); // 产生由小到大的随机数赋值给b
	}
	return *a;
}
 
/* 生成顺随机不重复数组，测试用 */
int CreatArray(int *a) {
	int i;
	int tmp;
	int flag = 0;
	int t = 0;
	/* 随机数的个数不能超出MAXSIZE和LIMIT之间的范围，不然会导致死循 */
	while(1) {
		flag = 0;
		if(t == MAXSIZE + 1) {
			break;
		}
		tmp = (rand() % (LIMIT - 0)) + 0;
		/* 0号元素不赋值 */
		for(i = 1; i < t; i++) {
			if(a[i] == tmp) {
				flag = 1;
			}
		}
		if(flag != 1) {
			a[t] = tmp;
			t++;
		}
	}
	return *a;
}

/* 顺序查找，a为数组，n为要查找的数组长度，key为要查找的关键字 */
/* 平均时间复杂度:O(n)，当n很大时，查找效率低 */
int SequentialSearch(int *a, int n, KeyType key) {
	int i;
	for(i = 1; i <= n; i++) {
		if(a[i] == key) {
			return i;
		}
	}
	return -1; // 没找到
}

/* 哨兵顺序查找，a为数组，n为要查找的数组长度，key为要查找的关键字 */
/* 平均时间复杂度:O(n)；当n很大时，查找效率低 */
int GuardSequentialSearch(int *a, int n, KeyType key) {
	int i;
	a[0] = key;
	i = n;
	while(a[i] != key) {
		i--;
	}
	if(i == 0) {
		return -1; // 没找到
	}
	return i;
}

/* 折半查找，a为数组，n为要查找的数组长度，key为要查找的关键字 */
/* 平均时间复杂度:O(logn)，前提条件:有序数列 */
int BinarySearch(int *a, int n, KeyType key) {
	int low, high, mid;
	low = 0; // 定义最低下标为记录首位
	high = n; // 定义最高下标为记录末位
	while(low <= high) {
		mid = (low + high) / 2; // 折半
		/* 若查找值比中值小 */
		if(key < a[mid]) {
			high = mid - 1; // 最高下标调整到中位下标小一位
		}
		/* 若查找值比中值大 */
		else if(key > a[mid]) {
			low = mid + 1; // 最低下标调整到中位下标大一位
		}
		else {
			return mid + 1; // 若相等则说明mid即为查找到的位置
		}
	}
	return -1; // 没找到
}

/* 插值查找，a为数组，n为要查找的数组长度，key为要查找的关键字 */
/* 平均时间复杂度:O(log(logn))，前提条件:已排序数；
	当数据分布比较均匀（成线性）时平均性能比折半查找要好得多，
	但当数据数据分布不均匀时，计算插值的代价比较大，性能不如二分查找 */
int InterpolationSearch(int *a, int n, KeyType key) {
	int low, high, mid;
	low = 0; // 定义最低下标为记录首位
	high = n; // 定义最高下标为记录末位
	while(low < high) {
		mid = low + (high - low) * (key - a[low]) / (a[high] - a[low]); // 插值
		/* 排除越界情况 */
		if(mid >= MAXSIZE) {
			return -1;
		}
		/* 若查找值比插值小 */
		if(key < a[mid]) {
			high = mid; // 最高下标调整到插值下标小一位
		}
		/* 若查找值比插值大 */
		else if(key > a[mid]) {
			low = mid + 1; // 最低下标调整到插值下标大一位
		}
		else {
			return mid + 1; // 若相等则说明mid即为查找到的位置
		}
	}
	return -1; // 没找到
}

/* 斐波那契查找，a为数组，n为要查找的数组长度，key为要查找的关键字 */
/* 平均时间复杂度:O(logn)，前提条件:有序数列；
	只用到加减运算，查找海量数据时平均性能优于折半查找，
	但目标元素若都处于左侧长半区，则查找效率要低于折半查找 */
int FibonacciSearch(int *a, int n, KeyType key) {
	int F[MAXSIZE]; // 定义斐波那契数列
	int low, high, mid, i, k = 0;
	low = 0; // 定义最低下标为记录首位
	high = n - 1; // 定义最高下标为记录末位
	Fibonacci(F);
	while(n > F[k] - 1) {
		++k;
	}
	int *tmp = (int*)malloc((F[k] - 1) * sizeof(int));
	memcpy(tmp, a, (n * sizeof(int))); // 将数组a扩展到F[k]-1的长度
	for(i = n; i < F[k] - 1; ++i) {
		tmp[i] = tmp[high];
	}
	while(low <= high) {
		mid = low + F[k - 1] - 1; // 黄金分割
		if(key < tmp[mid]) {
			high = mid - 1;
			k = k - 1;
		}
		else if(key > tmp[mid]) {
			low = mid + 1;
			k = k - 2;
		}
		else {
			if(mid <= high) {
				return mid + 1; // 若相等则说明mid即为查找到的位置
			}
			else {
				return high + 1;
			}
		}
	}
	return -1; // 没找到
}

int main(int argc, char** argv) {
	int a[MAXSIZE + 1]; // 声明数组，0号位置存放哨兵
	int b[MAXSIZE]; // 声明数组
	KeyType k; // 声明关键字k
	int i, result; // 声明临时变量
 
	srand((unsigned int)time(NULL)); //设置随机数种子
 
	*a = CreatArray(a); // 随机不重复数组生成器
	printf("产生的范围在0~%d的随机数:\n", LIMIT);
	/* 打印随机不重复数组 */
	for(i = 1; i <= MAXSIZE; i++) {
		printf("%d：%d\t", i, a[i]);
	}
	printf("\n");
 
	printf("请输入要查找的关键字：");
	scanf("%d", &k);
 
	result = SequentialSearch(a, MAXSIZE, k);
	printf("顺序查找 %d 的位置：%d \n", k, result);
	result = GuardSequentialSearch(a, MAXSIZE, k);
	printf("带哨兵顺序查找 %d 的位置：%d \n", k, result);
	printf("注：-1表示没找到\n");
 
	printf("\n");
	*b = CreatSeqArray(b); // 随机顺序数组生成器
	printf("产生的范围在0~%d，区间在%d由小到大的随机数：\n", LIMIT, INTERVAL);
	/* 打印随机顺序数组 */
	for(i = 0; i < MAXSIZE; i++) {
		printf("%d：%d\t", i + 1, b[i]);
	}
	printf("\n");
 
	printf("请输入要查找的关键字：");
	scanf("%d", &k);
 
	result = BinarySearch(b, MAXSIZE, k);
	printf("二分查找 %d 的位置：%d \n", k, result);
	result = InterpolationSearch(b, MAXSIZE, k);
	printf("插值查找 %d 的位置：%d \n", k, result);
	result = FibonacciSearch(b, MAXSIZE, k);
	printf("斐波那契查找 %d 的位置：%d \n", k, result);
	printf("注：-1表示没找到\n");
 
	return 0;
}

