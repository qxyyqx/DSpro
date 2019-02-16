#define  _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include "firstChHash.h"
#include "fenci.h"
#include "index.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
using namespace std;

Index *BST_Root = NULL;
extern Word_List essay_word[MAX_ESSAY_NUM];
extern int fun(int length, char *path);

//������������
void inverted_index() {
	for (int i = 0; i < MAX_ESSAY_NUM; i++) {
		CreateBST(BST_Root, i);
	}
}

//�����ض�һƪ���ĵķִ�����Ĵ���->��������������
void CreateBST(Index *&BST_Root, int essay_i) {
	for (int i = 0; i < essay_word[essay_i].word_num; i++){
		InsertBST(BST_Root, essay_word[essay_i].word[i], essay_i);
	}
}

void InsertBST(Index *&p, char *key, int essay_id) {
	if (p == NULL) { //����
		p = new Index;
		p->keyword = new char[100];
		strcpy(p->keyword, key);
		p->essay_head = new EssayInfo;
		p->essay_head->ID = essay_id;
		p->essay_head->next = NULL;
		p->lchild = NULL;
		p->rchild = NULL;
		return;
	}

	int cmp_result = wordCmp(key, p->keyword);
	if (cmp_result < 0) //���������в���
		InsertBST(p->lchild, key, essay_id);
	else if (cmp_result > 0) //���������в���
		InsertBST(p->rchild, key, essay_id);
	else {
		//���иý�㣬���������������㣬ֻҪ������������Ϣ
		EssayInfo *q = new EssayInfo;
		q->ID = essay_id;
		q->next = p->essay_head;
		p->essay_head = q;
	}
}

/* ������λ��Ƚ�2���������еĴ�С
����: 2����������
���: 0��ʾ2�������������;����1��ʾǰ�ߴ󣬸���1��ʾǰ��С
*/
int wordCmp(const char* chineseWord1, const char* chineseWord2)
{
	size_t len1 = strlen(chineseWord1);
	size_t len2 = strlen(chineseWord2);
	int firstIndex;
	int secondIndex;
	size_t i;
	for (i = 0; i < (len1 < len2 ? len1 : len2); i += 2)
	{
		char ch1[3];
		ch1[0] = chineseWord1[i];
		ch1[1] = chineseWord1[i + 1];
		ch1[2] = '\0';
		char ch2[3];
		ch2[0] = chineseWord2[i];
		ch2[1] = chineseWord2[i + 1];
		ch2[2] = '\0';
		firstIndex = hashGb2312(ch1);
		secondIndex = hashGb2312(ch2);

		//2�����ֶ���gb2312�ĳ��ú���
		if ((firstIndex >= 0) && (secondIndex >= 0) && (firstIndex < secondIndex))
			return -1;
		else if ((firstIndex >= 0) && (secondIndex >= 0) && (firstIndex > secondIndex))
			return 1;
		//��1�����ֲ���gb2312������(gbk��),��2��������gb2312���ú���
		else if ((firstIndex < 0) && (secondIndex >= 0))
			return 1;
		//��1��������gb2312���ú��֣���2�����ֲ���gb2312������(gbk��)
		else if ((firstIndex >= 0) && (secondIndex < 0))
			return -1;
		//2�����ֶ�����gb2312���ú���
		else if ((firstIndex < 0) && (secondIndex < 0))
		{
			firstIndex = hashGbk(ch1);
			secondIndex = hashGbk(ch2);
			if (firstIndex < secondIndex)
				return -1;
			else if (firstIndex > secondIndex)
				return 1;
		}
	}

	if (i < len1)
		return 1;
	else if (i < len2)
		return -1;

	return 0;
}

void preOrderTraverse(Index *r)
{
	if (r)
	{
		cout << r->keyword << ":";
		for (EssayInfo *p = r->essay_head; p != NULL; p = p->next)
			cout << p->ID << " ";
		if (r->lchild)
			cout << "left:" << r->lchild->keyword << " ";
		if (r->rchild)
			cout << "right:" << r->rchild->keyword << " ";
		cout << endl;
		preOrderTraverse(r->lchild);
		preOrderTraverse(r->rchild);
	}
}