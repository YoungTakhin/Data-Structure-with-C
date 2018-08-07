#include <stdio.h>
#include <string.h> // strlen
 
/* 状态码 */
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
 
#define MAXSIZE 20 // 存储空间初始分配量
 
typedef int Status;	// Status是函数的类型,其值是函数结果状态代码，如OK等
 
typedef struct {
	char ch[MAXSIZE]; // 储存字符串
	int Length; // 串长
}String;

Status StrInsert(String *S, int pos, String T); // 字符串插入，供其他函数使用
Status StrDelete(String *S, int pos, int len); // 字符串删除，供其他函数使用

/* KMP算法next函数，结果存入数组nextval */
void get_next(String T, int *next) {
	int i, j;
	i = 1;
	j = 0;
	next[1] = 0;
	while(i < T.Length) {
		/* T[i]表示后缀的单个字符，T[j]表示前缀的单个字符 */
		if(j == 0 || T.ch[i] == T.ch[j]) {
			++i;
			++j;
			next[i] = j;
		}
		else {
			j = next[j]; // 若字符不相同，则j值迭代
		}
	}
}
 
/* KMP算法next函数修正值,结果存入数组nextval */
void get_nextval(String T, int *nextval) {
	int i, j;
	i = 1;
	j = 0;
	nextval[1] = 0;
	while(i < T.ch[0]) {
		/* T[i]表示后缀的单个字符，T[j]表示前缀的单个字符 */
		if(j == 0 || T.ch[i] == T.ch[j]) {
			++i;
			++j;
			/* 若当前字符与前缀字符不同 */
			if(T.ch[i] != T.ch[j]) {
				nextval[i] = j; // 则当前的j为nextval在i位置的值
			}
			else {
				nextval[i] = nextval[j]; // 如果与前缀字符相同，则将前缀字符的nextval值赋值给nextval在i位置的值
			}
		}
		else {
			j = nextval[j]; // 若字符不相同，则j值迭代
		}
	}
}

Status StrAssign(String *T, char *chars) {
	int i;
	if(strlen(chars) > MAXSIZE) {
		return ERROR;
	}
	else {
		T->Length = strlen(chars);
		for(i = 0; i < T->Length; i++) {
			T->ch[i] = *(chars + i);
		}
		return OK;
	}
}

Status StrCopy(String *T, String S) {
	int i;
	for(i = 0; i < S.Length; i++) {
		T->ch[i] = S.ch[i];
	}
	T->Length = S.Length;
	return OK;
}

Status StrEmpty(String S) {
	if(S.Length == 0) {
		return TRUE;
	}
	return FALSE;
}

int StrCompare(String S, String T) {
	int i;
	for(i = 0; i < S.Length && i < T.Length; ++i)
		if(S.ch[i] != T.ch[i]) {
			return S.ch[i] - T.ch[i];
		}
	return S.Length - T.Length;
}

int StrLength(String S) {
	return S.Length;
}

Status ClearString(String *S) {
	S->Length = 0; // 令串长为零
	return OK;
}

Status Concat(String *T, String S1, String S2) {
	int i;
	/* 未截断S2 */
	if(S1.Length + S2.Length < MAXSIZE) {
		for(i = 0; i < S1.Length; i++) {
			T->ch[i] = S1.ch[i];
		}
		for(i = 0; i < S2.Length; i++) {
			T->ch[S1.Length + i] = S2.ch[i];
		}
		T->Length = S1.Length + S2.Length;
		return TRUE;
	}
	/* 截断S2 */
	else {
		for(i = 0; i < S1.Length; i++) {
			T->ch[i] = S1.ch[i];
		}
		for(i = 0; i < MAXSIZE - S1.Length; i++) {
			T->ch[S1.Length + i] = S2.ch[i];
		}
		T->Length = MAXSIZE;
		return FALSE;
	}
}

Status SubString(String *Sub, String S, int pos, int len) {
	int i;
	if(pos <= 0 || pos > S.Length || len < 0 || len > S.Length - pos + 1) {
		return ERROR;
	}
	for(i = 0; i < len; i++) {
		Sub->ch[i] = S.ch[pos + i - 1];
	}
	Sub->Length = len;
	return OK;
}

int Index(String S, String T, int pos) {
	int i = pos; // i用于主串S中当前位置下标值，若pos不为1，则从pos位置开始匹配
	int j = 1; // j用于子串T中当前位置下标值
	/* 若i小于S的长度并且j小于T的长度时，循环继续 */
	while(i <= S.Length && j <= T.Length) {
		/* 两字母相等则继续 */
		if(S.ch[i - 1] == T.ch[j - 1]) {
			++i;
			++j;
		}
		/* 指针后退重新开始匹配 */
		else {
			i = i - j + 2; // i退回到上次匹配首位的下一位
			j = 1; // j退回到子串T的首位
		}
	}
	if(j > T.Length) {
		return i - T.Length;
	}
	else {
		return 0;
	}
}

int Index_KMP(String S, String T, int pos) {
	int i = pos; // i用于主串S中当前位置下标值，若pos不为1，则从pos位置开始匹配
	int j = 1; // j用于子串T中当前位置下标值
	int next[255]; // 定义next数组
	//get_next(T, next);
	get_nextval(T, next); // 对串T作分析，得到next数组
	/* 若i小于S的长度并且j小于T的长度时，循环继续 */
	while(i <= S.Length && j <= T.Length) {
		/* 两字母相等则继续，与朴素算法增加了 j=0 判断 */
		if(j == 0 || S.ch[i - 1] == T.ch[j - 1]) {
			++i;
			++j;
		}
		/* 指针后退重新开始匹配 */
		else {
			j = next[j]; // j退回合适的位置，i值不变
		}
	}
	if(j > T.Length) {
		return i - T.Length;
	}
	else {
		return 0;
	}
}

Status Replace(String *S, String T, String V) {
	int i = 0; // 从串S的第一个字符起查找串T
	/* T是空串 */
	if(StrEmpty(T)) {
		return ERROR;
	}
	do {
		/* 结果i为从上一个i之后找到的子串T的位置 */
		i = Index(*S, T, i);
		/* 串S中存在串T */
		if(i) {
			StrDelete(S, i, StrLength(T)); // 删除该串T
			StrInsert(S, i, V); // 在原串T的位置插入串V
			i += StrLength(V); // 在插入的串V后面继续查找串T
		}
	} while (i);
	return OK;
}

Status StrInsert(String *S, int pos, String T) {
	int i;
	if(pos < 1 || pos > S->Length + 1) {
		return ERROR;
	}
	/* 完全插入 */
	if(S->Length + T.Length <= MAXSIZE) { 
		for(i = S->Length; i >= pos; i--) {
			S->ch[i + T.Length - 1] = S->ch[i - 1];
		}
		for(i = pos; i < pos + T.Length; i++) {
			S->ch[i - 1] = T.ch[i - pos];
		}
		S->Length = S->Length + T.Length;
		return TRUE;
	}
	/* 部分插入 */
	else { 
		for(i = MAXSIZE; i <= pos; i--) {
			S->ch[i - 1] = S->ch[i - T.Length - 1];
		}
		for(i = pos; i < pos + T.Length; i++) {
			S->ch[i - 1] = T.ch[i - pos];
		}
		S->Length = MAXSIZE;
		return FALSE;
	}
}

Status StrDelete(String *S, int pos, int len) {
	int i;
	if(pos < 1 || pos > S->Length - len + 1 || len < 0) {
		return ERROR;
	}
	for(i = pos + len; i <= S->Length; i++) {
		S->ch[i - len - 1] = S->ch[i - 1];
	}
	S->Length -= len;
	return OK;
}

void StrPrint(String T) {
	int i;
	for(i = 0; i < T.Length; i++) {
		printf("%c", T.ch[i]);
	}
	printf("\n");
}

int main(int argc, char** argv) {
	String t, s1, s2; // 声明串t, s1, s2
	String t1, t2, t3; // 声明串t1, t2, t3
	int i; // 声明临时变量
	char s; // 声明临时变量
 
	StrAssign(&s1, "abcde");
	printf("串s1：");
	StrPrint(s1);
	printf("s1是否为空串：%d（0:否；1:空）\n", StrEmpty(s1));
	printf("s1的串长：%d\n", StrLength(s1));
 
	StrCopy(&t, s1);
	printf("串s1复制到t后，串t：");
	StrPrint(t);
 
	StrAssign(&s2, "fghij");
	printf("串s2：");
	StrPrint(s2);
 
	i = StrCompare(s1, s2);
	if(i < 0) {
		s = '<';
	}
	else if(i == 0) {
		s = '=';
	}
	else {
		s = '>';
	}
	printf("s1 %c s2\n", s);
 
	ClearString(&t);
	printf("清空后t是否为空串：%d（0:否；1:空）\n", StrEmpty(t));
	printf("清空后t的串长：%d\n", StrLength(t));
 
	SubString(&t, s1, 2, 3);
	printf("s1中第2个位置起长度为3的子串t为：");
	StrPrint(t);
	printf("t是否为空串：%d（0:否；1:空）\n", StrEmpty(t));
	printf("t的串长：%d\n", StrLength(t));
 
	ClearString(&t);
	Concat(&t, s1, s2);
	printf("由s1和s2联接而成的串t：");
	StrPrint(t);
	printf("联接后t是否为空串：%d（0:否；1:空）\n", StrEmpty(t));
	printf("联接后t的串长：%d\n", StrLength(t));
 
	printf("串t:");
	StrPrint(t);
	printf("串s2:");
	StrPrint(s2);
	i = Index(t, s2, 0);
	printf("s2在t的第%d个位置出现(BF算法)\n", i);
	i = Index_KMP(t, s2, 0);
	printf("s2在t的第%d个位置出现(KMP算法)\n", i);
 
	StrInsert(&s1, 2, s2);
	printf("在s1的第2个位置插入s2后，串s1：");
	StrPrint(s1);
	printf("插入后s1的串长：%d\n", StrLength(s1));
 
	StrDelete(&s1, 2, 5);
	printf("从s1的第2个位置删除长度为5的子串后：");
	StrPrint(s1); 
	printf("删除后s1的串长：%d\n", StrLength(s1));
 
	StrAssign(&t1, "abcdefgh");
	StrAssign(&t2, "bcd");
	StrAssign(&t3, "wxyz");
 
	printf("串t1:");
	StrPrint(t1);
	printf("串t2:");
	StrPrint(t2);
	printf("串t3:");
	StrPrint(t3);
	Replace(&t1, t2, t3);
	printf("用t3替换t1中与t2匹配的子串后，串t1:");
	StrPrint(t1);
 
	return 0;
}

