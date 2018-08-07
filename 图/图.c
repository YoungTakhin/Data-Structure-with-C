#include <stdio.h>
#include <stdlib.h> // malloc
 
/* 状态码 */
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
 
#define MAXSIZE	50 // 最大储存长度
#define MAXVEX 50 // 最大顶点数，应由用户定义
#define MAXEDGE MAXVEX * MAXVEX - 1 // 最大边数
#define INF 0x3f3f3f3f // 定义无穷大
 
typedef int Status; // Status是函数的类型,其值是函数结果状态代码，如OK等
typedef char VertexType; // 顶点VertexType类型根据实际情况而定，这里假设为char
typedef int EdgeType; // 边上的权值EdgeType类型根据实际情况而定，这里假设为int
 
/* 邻接矩阵********************* */
/* 定义图的邻接矩阵结构 */
typedef struct {
	VertexType vexs[MAXVEX]; // 顶点表
	EdgeType arc[MAXVEX][MAXVEX]; // 邻接矩阵，可看作边表
	int numVertexes, numEdges; // 图中当前的顶点数和边数
	Status undirected; // 是否是无向图，值为TRUE是为无向图，否则为有向图
}MGraph;
/* ***************************** */
 
/* 邻接表*********************** */
/* 定义图的边表 */
typedef struct EdgeNode {
	int adjvex; // 邻接点域，存储该顶点对应的下标
	int weight; // 用于存储权值，对于非网图可以不需要
	struct EdgeNode *next; // 链域，指向下一个邻接点
}EdgeNode;
 
/* 定义图的顶点表结点 */
typedef struct VertexNode {
	int in; // 顶点入度
	VertexType data; // 顶点域，存储顶点信息
	EdgeNode *firstedge; // 边表头指针
}VertexNode, AdjList[MAXVEX];
 
/* 定义图的邻接表结构 */
typedef struct{
	AdjList adjList;
	int numVertexes, numEdges; // 图中当前顶点数和边数
}adjListGraph, *AdjListGraph;
/* ***************************** */
 
/* 对边集数组Edge结构的定义，最小生成树Kruskal算法使用 */
typedef struct {
	int begin;
	int end;
	int weight;
}Edge;
 
Status visited[MAXVEX]; // 访问标志的数组
 
typedef int PatharcDijkstra[MAXVEX]; // 用于存储Dijkstra算法最短路径下标的数组
typedef int ShortPathTableDijkstra[MAXVEX]; // 用于存储Dijkstra算法到各点最短路径的权值和
 
typedef int PatharcFloyd[MAXVEX][MAXVEX]; // 用于存储Floyd算法最短路径下标的数组
typedef int ShortPathTableFloyd[MAXVEX][MAXVEX]; // 用于存储Floyd算法到各点最短路径的权值和
 
#if 1 // 以下是引入的链队列的基本操作
 
/* 定义顺序循环队列 */
typedef EdgeType QElemType;
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
 
Status QueueEmpty(SqQueue Q) {
	/* 队列空的标志 */
	if(Q.front == Q.rear) {
		return TRUE;
	}
	return FALSE;
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
 
#endif // 1
 
/* 邻接矩阵的深度优先递归算法，内部接口，M_DFSTaverse()调用 */
void M_DFS(MGraph G, int i) {
	int j;
	visited[i] = TRUE;
	printf("%c ", G.vexs[i]); // 打印顶点，也可以其它操作
	for(j = 0; j < G.numVertexes; j++) {
		if(G.arc[i][j] == 1 && !visited[j]) {
			M_DFS(G, j); // 对为访问的邻接顶点递归调用
		}
	}
}
 
/* 邻接表的深度优先递归算法，A_DFSTraverse()调用 */
void A_DFS(AdjListGraph GL, int i) {
	EdgeNode *p;
	visited[i] = TRUE;
	printf("%c ", GL->adjList[i].data); // 打印顶点,也可以其它操作
	p = GL->adjList[i].firstedge;
	while(p) {
		if(!visited[p->adjvex]) {
			A_DFS(GL, p->adjvex); // 对为访问的邻接顶点递归调用
		}
		p = p->next;
	}
}
 
/* 交换权值以及头和尾，kruskal算法内部接口 */
void Swapn(Edge *edges, int i, int j) {
	int temp;
	temp = edges[i].begin;
	edges[i].begin = edges[j].begin;
	edges[j].begin = temp;
	temp = edges[i].end;
	edges[i].end = edges[j].end;
	edges[j].end = temp;
	temp = edges[i].weight;
	edges[i].weight = edges[j].weight;
	edges[j].weight = temp;
}
 
/* 对权值进行排序，kruskal算法内部接口 */
void sort(Edge edges[], MGraph *G) {
	int i, j;
	for(i = 0; i < G->numEdges; i++) {
		for(j = i + 1; j < G->numEdges; j++) {
			if(edges[i].weight > edges[j].weight) {
				Swapn(edges, i, j);
			}
		}
	}
	printf("\n权排序之后的为:\n");
	for(i = 0; i < G->numEdges; i++) {
		printf("(%c, %c) %d\n", G->vexs[edges[i].begin], G->vexs[edges[i].end], edges[i].weight);
	}
 
}
 
/* 查找连线顶点的尾部下标，kruskal算法内部接口 */
int Find(int *parent, int f) {
	while(parent[f] > 0) {
		f = parent[f];
	}
	return f;
}

void CreateMGraph(MGraph *G) {
	int i, j, k, w;
	printf("是否是无向图(1:是；0:否)：");
	scanf("%d", &G->undirected);
	printf("输入顶点数和边数:\n");
	scanf("%d %d", &G->numVertexes, &G->numEdges); // 输入顶点数和边数
	/* 读入顶点信息,建立顶点表 */
	for(i = 0; i < G->numVertexes; i++) {
		printf("%d号顶点:  ", i);
		rewind(stdin); // 清空缓冲区
		scanf("%c", &G->vexs[i]);
	}
	for(i = 0; i <G->numVertexes; i++) {
		for(j = 0; j < G->numVertexes; j++) {
			if(i == j) {
				G->arc[i][j] = 0; // 自己到自己的距离是0
			}
			else {
				G->arc[i][j] = (int)INF; // 邻接矩阵初始化
			}
		}
	}
	/* 读入numEdges条边，建立邻接矩阵 */
	for(k = 0; k <G->numEdges; k++) {
		printf("输入边(vi,vj)上的下标i，下标j和权w:\n");
		scanf("%d %d %d", &i, &j, &w); // 输入边(vi,vj)上的权w
		G->arc[i][j] = w;
		/* 判断是否是无向图 */
		if(G->undirected) {
			G->arc[j][i] = G->arc[i][j]; // 因为是无向图，矩阵对称
		}
	}
}

Status ChangeALGraph(MGraph G, AdjListGraph *GL) {
	int i, j;
	EdgeNode *e;
	*GL = (AdjListGraph)malloc(sizeof(adjListGraph));
	(*GL)->numVertexes = G.numVertexes;
	(*GL)->numEdges = G.numEdges;
	/* 读入顶点信息，建立顶点表 */
	for(i = 0; i < G.numVertexes; i++) {
		(*GL)->adjList[i].in = 0;
		(*GL)->adjList[i].data = G.vexs[i];
		(*GL)->adjList[i].firstedge = NULL; // 将边表置为空表
	}
	/* 建立边表 */
	for(i = 0; i < G.numVertexes; i++) {
		for(j = 0; j < G.numVertexes; j++) {
			if(G.arc[i][j] != 0 && G.arc[i][j] < INF) {
				e = (EdgeNode *)malloc(sizeof(EdgeNode));
				e->adjvex = j; // 邻接序号为j
				e->weight = G.arc[i][j];
				e->next = (*GL)->adjList[i].firstedge; // 将当前顶点上的指向的结点指针赋值给e
				(*GL)->adjList[i].firstedge = e; // 将当前顶点的指针指向e
				(*GL)->adjList[j].in++;
			}
		}
	}
	return TRUE;
}

int M_LocateVex(MGraph G, VertexType u) {
	int i;
	for(i = 0; i < G.numVertexes; i++) {
		if(u == G.vexs[i]) {
			return i;
		}
	}
	return -1;
}

int A_LocateVex(AdjListGraph G, VertexType u) {
	int i;
	for(i = 0; i < G->numVertexes; i++) {
		if(u == G->adjList[i].data) {
			return i;
		}
	}
	return -1;
}

VertexType M_GetVex(MGraph G, int v) {
	if(v < 0 || v > G.numVertexes) {
		return '?';
	}
	return G.vexs[v];
}

VertexType A_GetVex(AdjListGraph G, int v) {
	if(v < 0 || v > G->numVertexes) {
		return '?';
	}
	return G->adjList[v].data;
}

void M_DFSTraverse(MGraph G) {
	int i;
	for(i = 0; i < G.numVertexes; i++) {
		visited[i] = FALSE; // 初始所有顶点状态都是未访问过状态
	}
	for(i = 0; i < G.numVertexes; i++) {
		/* 对未访问过的顶点调用DFS，若是连通图，只会执行一次 */
		if(!visited[i]) {
			M_DFS(G, i);
		}
	}
}

void A_DFSTraverse(AdjListGraph GL) {
	int i;
	for(i = 0; i < GL->numVertexes; i++) {
		visited[i] = FALSE; // 初始所有顶点状态都是未访问过状态
	}
	for(i = 0; i < GL->numVertexes; i++) {
		/* 对未访问过的顶点调用DFS,若是连通图,只会执行一次 */
		if(!visited[i]) {
			A_DFS(GL, i);
		}
	}
}

void M_BFSTraverse(MGraph G) {
	int i, j;
	SqQueue Q;
	for(i = 0; i < G.numVertexes; i++) {
		visited[i] = FALSE;
	}
	InitQueue(&Q); // 初始化一辅助用的队列
	/* 对每一个顶点做循环 */
	for(i = 0; i < G.numVertexes; i++) {
		/* 若是未访问过就处理 */
		if(!visited[i]) {
			visited[i] = TRUE; // 设置当前顶点访问过
			printf("%c ", G.vexs[i]); // 打印顶点，也可以其它操作
			EnQueue(&Q, i);	// 将此顶点入队列
			/* 若当前队列不为空 */
			while(!QueueEmpty(Q)) {
				DeQueue(&Q, &i); // 将队对元素出队列，赋值给i
				for(j = 0; j < G.numVertexes; j++) {
					/* 判断其它顶点若与当前顶点存在边且未访问过 */
					if(G.arc[i][j] == 1 && !visited[j]) {
						visited[j] = TRUE; // 将找到的此顶点标记为已访问
						printf("%c ", G.vexs[j]); // 打印顶点
						EnQueue(&Q, j); // 将找到的此顶点入队列
					}
				}
			}
		}
	}
}

void A_BFSTraverse(AdjListGraph GL) {
	int i;
	EdgeNode *p;
	SqQueue Q;
	for(i = 0; i < GL->numVertexes; i++) {
		visited[i] = FALSE;
	}
	InitQueue(&Q);
	for(i = 0; i < GL->numVertexes; i++) {
		if(!visited[i]) {
			visited[i] = TRUE;
			printf("%c ", GL->adjList[i].data);/* 打印顶点,也可以其它操作 */
			EnQueue(&Q, i);
			while(!QueueEmpty(Q)) {
				DeQueue(&Q, &i);
				p = GL->adjList[i].firstedge;	/* 找到当前顶点的边表链表头指针 */
				while(p) {
					/* 若此顶点未被访问 */
					if(!visited[p->adjvex]) {
						visited[p->adjvex] = TRUE;
						printf("%c ", GL->adjList[p->adjvex].data);
						EnQueue(&Q, p->adjvex);	/* 将此顶点入队列 */
					}
					p = p->next; // 指针指向下一个邻接点
				}
			}
		}
	}
}

void MiniSpanTree_Prim(MGraph G) {
	int min, i, j, k;
	int adjvex[MAXVEX]; // 保存相关顶点下标
	int lowcost[MAXVEX]; // 保存相关顶点间边的权值
	lowcost[0] = 0; // 初始化第一个权值为0，即v0加入生成树，lowcost的值为0，在这里就是此下标的顶点已经加入生成树
	adjvex[0] = 0; // 初始化第一个顶点下标为 0
	/* 循环除下标为 0 外的全部顶点 */
	for(i = 1; i < G.numVertexes; i++) {
		lowcost[i] = G.arc[0][i]; // 将 v0 顶点与之有边的权值存入数组
		adjvex[i] = 0; // 初始化都为 v0 的下标
	}
	printf("\n");
	for(i = 1; i < G.numVertexes; i++) {
		min = INF; // 初始化最小权值为∞
		j = 1; k = 0;
		/* 循环全部顶点 */
		while(j < G.numVertexes) {
			/* 如果权值不为 0 且权值小于 min */
			if(lowcost[j] != 0 && lowcost[j] < min) {
				min = lowcost[j]; // 则让当前权值成为最小值
				k = j; // 将当前最小值的下标存入k
			}
			j++;
		}
		printf("边:(%c, %c)\t权值: %d\n", G.vexs[adjvex[k]], G.vexs[k]\
			, G.arc[adjvex[k]][k]); // 打印当前顶点边中权值最小的边
		lowcost[k] = 0; // 将当前顶点的权值设置为 0,表示此顶点已经完成任务
		/* 循环所有顶点 */
		for(j = 1; j < G.numVertexes; j++) {
			/* 如果下标为 k 顶点各边权值小于此前这些顶点未被加入生成树权值 */
			if(lowcost[j] != 0 && G.arc[k][j] < lowcost[j]) {
				lowcost[j] = G.arc[k][j]; // 将较小的权值存入lowcost相应位置
				adjvex[j] = k; // 将下标为k的顶点存入adjvex
			}
		}
	}
}

void MiniSpanTree_Kruskal(MGraph G) {
	int i, j, n, m;
	int k = 0;
	int parent[MAXVEX]; // 定义一数组用来判断边与边是否形成环路
	Edge edges[MAXEDGE]; // 定义边集数组,edge的结构为begin,end,weight,均为整型
	/* 构建边集数组 */
	for(i = 0; i < G.numVertexes - 1; i++) {
		for(j = i + 1; j < G.numVertexes; j++) {
			if(G.arc[i][j] < INF) {
				edges[k].begin = i;
				edges[k].end = j;
				edges[k].weight = G.arc[i][j];
				k++;
			}
		}
	}
	sort(edges, &G); // 排序边集数组
	for(i = 0; i < G.numVertexes; i++) {
		parent[i] = 0; // 初始化数组值为0
	}
	printf("打印最小生成树：\n");
	/* 循环每一条边 */
	for(i = 0; i < G.numEdges; i++) {
		n = Find(parent, edges[i].begin);
		m = Find(parent, edges[i].end);
		/* 假如n与m不等，说明此边没有与现有的生成树形成环路 */
		if(n != m) {
			parent[n] = m; // 将此边的结尾顶点放入下标为起点的parent中，表示此顶点已经在生成树集合中
			printf("边:(%c, %c)\t权值: %d\n", G.vexs[edges[i].begin]\
				, G.vexs[edges[i].end], edges[i].weight);
		}
	}
}

Status ShortestPath_Dijkstra(MGraph G, int vstart, int vend) {
	if(vend < 0 || vstart < 0 || vstart > G.numVertexes || vend > G.numVertexes) {
		printf("输入有误\n");
		return FALSE;
	}
	PatharcDijkstra P; // 存储最短路径下标的数组
	ShortPathTableDijkstra D; // 某点到其余各点的最短路径
	int v, w, k, min;
	int final[MAXVEX]; // final[w]=1表示求得顶点vstart至vw的最短路径
	int i, j; // 临时变量
	/* 若起点位置比终点位置大，则调换 */
	if(vstart > vend) {
		int tmp = vstart;
		vstart = vend;
		vend = tmp;
	}
	/* 初始化数据 */
	for(v = 0; v < G.numVertexes; v++) {
		final[v] = 0; // 全部顶点初始化为未知最短路径状态
		D[v] = G.arc[vstart][v]; // 将与vstart点有连线的顶点加上权值
		P[v] = 0; // 初始化路径数组P为0
	}
	D[vstart] = 0; // vstart至vstart路径为0
	final[vstart] = 1; // vstart至vstart不需要求路径
	/* 开始主循环，每次求得vstart到某个v顶点的最短路径 */
	for(v = 0; v < G.numVertexes; v++) {
		min = INF; // 当前所知离v0顶点的最近距离
		/* 寻找离vstart最近的顶点 */
		for(w = 0; w < G.numVertexes; w++) {
			if(!final[w] && D[w] < min) {
				k = w;
				min = D[w]; // w顶点离vstart顶点更近
			}
		}
		final[k] = 1; // 将目前找到的最近的顶点置为1
		/* 修正当前最短路径及距离 */
		for(w = 0; w < G.numVertexes; w++) {
			/* 若经过v顶点的路径比现在这条路径的长度短，则说明找到了更短的路径，修改D[w]和P[w] */
			if(!final[w] && (min + G.arc[k][w] < D[w])) {
				D[w] = min + G.arc[k][w]; // 修改当前路径长度
				P[w] = k;
			}
		}
	}
 
	/* 以下为输出 */
	printf("最短路径倒序如下:\n");
	for(i = 0; i <= vend; ++i) {
		printf("%c - %c : ", G.vexs[vstart], G.vexs[i]);
		j = i;
		while(P[j] != 0) {
			printf("%c ", G.vexs[P[j]]);
			j = P[j];
		}
		printf("\n");
	}
	printf("顶点 %c 到各顶点的最短路径长度为:\n", G.vexs[vstart]);
	for(i = 0; i < G.numVertexes; ++i) {
		if(D[i] == INF) {
			printf("%c --> %c :∞\n", G.vexs[vstart], G.vexs[i]);
		}
		else {
			printf("%c --> %c : %d \n", G.vexs[vstart], G.vexs[i], D[i]);
		}
	}
	printf("%c 到 %c 的最短路径长度：%d\n"\
		, G.vexs[vstart], G.vexs[vend], D[vend]);
 
	return TRUE;
}

Status ShortestPath_Floyd(MGraph G, int vstart, int vend) {
	if(vend < 0 || vstart < 0 || vstart > G.numVertexes || vend > G.numVertexes) {
		printf("输入有误\n");
		return FALSE;
	}
	PatharcFloyd P;
	ShortPathTableFloyd D;
	int v, w; // v为各起点，w为各终点
	int k; // k为临时变量
	/* 初始化D与P */
	for(v = 0; v < G.numVertexes; ++v) {
		for(w = 0; w < G.numVertexes; ++w) {
			D[v][w] = G.arc[v][w]; // D[v][w]值即为对应点间的权值
			P[v][w] = w; // 初始化P
		}
	}
	for(k = 0; k < G.numVertexes; ++k) {
		for(v = 0; v < G.numVertexes; ++v) {
			for(w = 0; w < G.numVertexes; ++w) {
				/* 如果经过下标为k顶点路径比原两点间路径更短 */
				if(D[v][w] > D[v][k] + D[k][w]) {
					D[v][w] = D[v][k] + D[k][w]; // 将当前两点间权值设为更小的一个
					P[v][w] = P[v][k]; // 路径设置为经过下标为k的顶点
				}
			}
		}
	}
 
	/* 以下为输出 */
	printf("起点到终点最短下一个路径P(纵坐标：起点；横坐标：终点)：\n");
	printf("\t");
	for(w = 0; w < G.numVertexes; ++w) {
		printf("%c:\t", G.vexs[w]);
	}
	printf("\n\n");
	for(v = 0; v < G.numVertexes; ++v) {
		printf("%c:\t", G.vexs[v]);
		for(w = 0; w < G.numVertexes; ++w) {
			printf("%c\t", G.vexs[P[v][w]]);
		}
		printf("\n");
	}
	printf("各顶点间最短路径如下:\n");
	for(v = 0; v < G.numVertexes; ++v) {
		for(w = v + 1; w < G.numVertexes; w++) {
			if(D[v][w] == INF) {
				printf("%c - %c 路径长度: ∞ ", G.vexs[v], G.vexs[w]);
			}
			else {
				printf("%c - %c 路径长度: %d "\
					, G.vexs[v], G.vexs[w], D[v][w]);
			}
			k = P[v][w];				/* 获得第一个路径顶点下标 */
			printf("\t路径: %c", G.vexs[v]);	/* 打印源点 */
			/* 如果路径顶点下标不是终点 */
			while(k != w) {
				printf(" -> %c", G.vexs[k]);	/* 打印路径顶点 */
				k = P[k][w];			/* 获得下一个路径顶点下标 */
			}
			printf(" -> %c\n", G.vexs[w]);	/* 打印终点 */
		}
		printf("\n");
	}
	printf("起点到终点最短路径长度D(纵坐标：起点；横坐标：终点)：\n");
	printf("\t");
	for(w = 0; w < G.numVertexes; ++w) {
		printf("%c:\t", G.vexs[w]);
	}
	printf("\n\n");
	for(v = 0; v < G.numVertexes; ++v) {
		printf("%c:\t", G.vexs[v]);
		for(w = 0; w < G.numVertexes; ++w) {
			if(D[v][w] == INF) {
				printf("∞\t");
			}
			else{
				printf("%d\t", D[v][w]);
			}
		}
		printf("\n");
	}
	printf("%c 到 %c 的最短路径长度：%d\n"\
		, G.vexs[vstart], G.vexs[vend], D[vstart][vend]);
	return TRUE;
}

Status TopologicalSort(AdjListGraph GL) {
	EdgeNode *e;
	int i, k;
	int m = 0;
	EdgeType gettop;
	EdgeType top = 0;  /* 用于栈指针下标  */
	int count = 0;/* 用于统计输出顶点的个数  */
	EdgeType *stack;	/* 建栈将入度为0的顶点入栈  */
	stack = (EdgeType *)malloc(GL->numVertexes * sizeof(EdgeType));
	for(i = 0; i < GL->numVertexes; i++) {
		/* 将入度为0的顶点入栈 */
		if(0 == GL->adjList[i].in) {
			stack[++top] = i;
		}
	}
	while(top != 0) {
		gettop = stack[top--];
		printf("%c -> ", GL->adjList[gettop].data);
		count++; // 输出i号顶点，并计数
		for(e = GL->adjList[gettop].firstedge; e; e = e->next) {
			k = e->adjvex;
			/* 将i号顶点的邻接点的入度减1，如果减1后为0，则入栈 */
			if(!(--GL->adjList[k].in)) {
				stack[++top] = k;
			}
		}
	}
	printf("\n");
	if(count < GL->numVertexes) {
		return ERROR;
	}
	else {
		return OK;
	}
}

int main(int argc, char** argv) {
	MGraph G; // 声明图邻接矩阵表示G
	AdjListGraph GL; // 声明图邻接表表示GL
	VertexType ch; // 声明顶点元素 ch
	int i; // 声明临时变量
	int start, end; // 声明临时变量
 
	CreateMGraph(&G);
	ChangeALGraph(G, &GL);
 
	printf("请输入要查找的顶点：");
	rewind(stdin); // 刷新缓冲区
	scanf("%c", &ch);
	printf("邻接矩阵查找 %c 的位置是：%d\n", ch, M_LocateVex(G, ch));
	printf("邻接表查找 %c 的位置是：%d\n", ch, A_LocateVex(GL, ch));
 
	printf("请输入要查找的顶点的位置：");
	rewind(stdin); // 刷新缓冲区
	scanf("%d", &i);
	printf("邻接矩阵 %d 的位置是顶点：%c\n", i, M_GetVex(G, i));
	printf("邻接表 %d 的位置是顶点：%c\n", i, A_GetVex(GL, i));
 
	printf("邻接矩阵深度优先遍历：");
	M_DFSTraverse(G);
	printf("\n");
 
	printf("邻接矩阵广度优先遍历：");
	M_BFSTraverse(G);
	printf("\n");
 
	printf("邻接表深度优先遍历：");
	A_DFSTraverse(GL);
	printf("\n");
 
	printf("邻接表广度优先遍历：");
	A_BFSTraverse(GL);
	printf("\n");
 
	printf("最小生成树Prim算法：");
	MiniSpanTree_Prim(G);
	printf("\n");
 
	printf("最小生成树Kruskal算法：");
	MiniSpanTree_Kruskal(G);
	printf("\n");
 
	printf("请输入待求路径的起点号和终点号：");
	rewind(stdin); // 刷新缓冲区
	scanf("%d %d", &start, &end);
 
	printf("最短路径Dijkstra算法：\n\n");
	ShortestPath_Dijkstra(G, start, end);
	printf("\n");
 
	printf("最短路径Floyd算法：\n\n");
	ShortestPath_Floyd(G, start, end);
	printf("\n");
 
	printf("拓扑排序：\n");
	TopologicalSort(GL);
	printf("\n");	
 
	return 0;
}


