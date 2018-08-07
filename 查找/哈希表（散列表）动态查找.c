#include <stdio.h>
#include <stdlib.h> // malloc、srand、rand
#include <time.h> // time
 
/* 状态码 */
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
 
#define MAXSIZE 100 // 数据元素存储空间初始分配量
#define HASHSIZE 15 // 哈希表长度
#define NULLKEY -0x3f3f3f3f // 哈希表单元为空的标记
 
#define INTERVAL 3 // 随机数波动区间
#define LIMIT HASHSIZE * INTERVAL - 1 // 随机数范围
 
typedef int Status; // Status是函数的类型，其值是函数结果状态代码，如OK等
typedef int KeyType; // 关键字的数据类型，类型根据实际情况而定，这里假设为 int
 
typedef struct {
	KeyType *elem; // 储存数据元素
	int count; // 当前已存进哈希表的数据元素个数
}HashTable;
 
/* 哈希函数，返回关键字key的地址，InsertHash()调用 */
int Hash(KeyType key) {
	return key % HASHSIZE; // 除留余数法
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
		if(t == HASHSIZE) {
			break;
		}
		tmp = (rand() % (LIMIT - 0)) + 0;
		for(i = 0; i < t; i++) {
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

/* 初始化哈希表H */
Status InitHashTable(HashTable *H) {
	int i;
	H->count = 0;
	H->elem = (KeyType *)malloc(HASHSIZE * sizeof(KeyType));
	for(i = 0; i < HASHSIZE; i++) {
		H->elem[i] = NULLKEY;
	}
	return OK;
}

/* 将关键字key插入哈希表H中 */
Status InsertHash(HashTable *H, KeyType key) {
	if(H->count >= HASHSIZE) {
		return ERROR;
	}
	int addr = Hash(key); // 求哈希地址
	/* 如果不为空，则冲突 */
	while(H->elem[addr] != NULLKEY) {
		addr = (addr + 1) % HASHSIZE; // 开放定址法的线性探测
	}
	H->elem[addr] = key; // 直到有空位后插入关键字
	H->count += 1;
	return OK;
}

/* 在哈希表H中查找关键字key，若成功，用*addr返回位置，函数返回OK */
Status SearchHash(HashTable H, KeyType key, int *addr) {
	*addr = Hash(key); // 求哈希地址
	/* 如果不为空，则冲突 */
	while(H.elem[*addr] != key) {
		*addr = (*addr + 1) % HASHSIZE; // 开放定址法的线性探测
		/* 如果循环回到原点 */
		if(H.elem[*addr] == NULLKEY || *addr == Hash(key)) {
			return FALSE; // 则说明关键字不存在
		}
	}
	*addr += 1;
	return OK;
}

/* 打印哈希表H，若某位置中没有数据元素，则在该位置打印NaN */
Status PrintHash(HashTable H) {
	int i;
	printf("哈希表： ");
	for(i = 0; i < HASHSIZE; i++) {
		if(H.elem[i] == NULLKEY) { 
			printf("NaN ");
		}
		else {
			printf("%d ", H.elem[i]);
		}
	}
	printf("\n");
	return OK;
}

int main(int argc, char** argv) {
	HashTable H; // 声明哈希表H
	KeyType a[MAXSIZE]; // 声明数组
	KeyType k; // 声明关键字key
	Status result; // 声明状态码
	int i, p; // 声明临时变量
 
	srand((unsigned int)time(NULL)); //设置随机数种子
 
	*a = CreatArray(a);
	printf("随机生成的不重复数组：");
	for(i = 0; i < 10; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");
 
	InitHashTable(&H);
	for(i = 0; i < 10; i++) {
		InsertHash(&H, a[i]); // 把随机生成的10个数存进去
	}
 
	PrintHash(H);
 
	printf("请输入要插入的数据元素：");
	scanf("%d", &k);
	InsertHash(&H, k);
	PrintHash(H);
 
	printf("请输入要查找的数据元素：");
	scanf("%d", &k);
	result = SearchHash(H, k, &p);
	if(result) {
		printf("查找 %d 的位置： %d \n", k, p);
	}
	else {
		printf("查找 %d 失败 \n", k);
	}
 
	return 0;
}

