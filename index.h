//�������е����Ľ��
struct EssayInfo {
	int ID;
	EssayInfo *next;
};

//�������������������
struct Index {
	char *keyword;              //�ؼ���
	EssayInfo *essay_head;      //������
	Index *lchild, *rchild;     //������Ů
};

void CreateBST(Index *&BST_Root, int i);
void InsertBST(Index *&p, char *key, int essay_id);
int wordCmp(const char* chineseWord1, const char* chineseWord2);