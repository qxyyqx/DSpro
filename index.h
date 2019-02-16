//索引表中的论文结点
struct EssayInfo {
	int ID;
	EssayInfo *next;
};

//二叉搜索树的索引结点
struct Index {
	char *keyword;              //关键字
	EssayInfo *essay_head;      //索引表
	Index *lchild, *rchild;     //左右子女
};

void CreateBST(Index *&BST_Root, int i);
void InsertBST(Index *&p, char *key, int essay_id);
int wordCmp(const char* chineseWord1, const char* chineseWord2);