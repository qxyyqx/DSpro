#define MAX_VERTEX_NUM (1+800)//��0�����������壬����ͼ����󶥵���
#define PR_ALPHA 0.75 // page-rank factor

typedef char VertexData;

typedef struct EdgeNode//����
{
	int dstid;//�ڽӵ���
	VertexData data;
	EdgeNode *next;//�߽������Ӧ����һ���߽��
} EdgeNode;

typedef struct VertexNode//ͷ���
{
	//    VertexData data;
	int srcid;
	int hash_addr;
	int indegree;
	int outdegree;
	char url[128];

	double cost; // PR ֵ

	EdgeNode *firstedge;//ͷ�������Ӧ�ĵ�һ���߽��
} VertexNode;

typedef struct AdjList {
	int VexNum, ArcNum;//����ͼ�Ķ������ͱ���
	VertexNode* vertex[MAX_VERTEX_NUM];//����ͷ������顣
} AdjList;



// ==================== Page Rank ======================

typedef struct TriGroup // ϡ�������Ԫ��
{
	int row, col;
	double val;
} Item;

typedef struct SparseMatrix {
	bool dirty; // dirtyλ

	int ROW, COL;
	int size;
	struct TriGroup* item;
} SM;

// =====================================================
