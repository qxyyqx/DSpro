#define MAX_VERTEX_NUM (1+800)//第0个顶点无意义，定义图的最大顶点数
#define PR_ALPHA 0.75 // page-rank factor

typedef char VertexData;

typedef struct EdgeNode//表结点
{
	int dstid;//邻接点域
	VertexData data;
	EdgeNode *next;//边结点所对应的下一个边结点
} EdgeNode;

typedef struct VertexNode//头结点
{
	//    VertexData data;
	int srcid;
	int hash_addr;
	int indegree;
	int outdegree;
	char url[128];

	double cost; // PR 值

	EdgeNode *firstedge;//头结点所对应的第一个边结点
} VertexNode;

typedef struct AdjList {
	int VexNum, ArcNum;//定义图的顶点数和边数
	VertexNode* vertex[MAX_VERTEX_NUM];//定义头结点数组。
} AdjList;



// ==================== Page Rank ======================

typedef struct TriGroup // 稀疏矩阵三元组
{
	int row, col;
	double val;
} Item;

typedef struct SparseMatrix {
	bool dirty; // dirty位

	int ROW, COL;
	int size;
	struct TriGroup* item;
} SM;

// =====================================================
