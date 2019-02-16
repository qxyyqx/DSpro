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

//建立倒排索引
void inverted_index() {
	for (int i = 0; i < MAX_ESSAY_NUM; i++) {
		CreateBST(BST_Root, i);
	}
}

//对于特定一篇论文的分词数组的处理->建立二叉搜索树
void CreateBST(Index *&BST_Root, int essay_i) {
	for (int i = 0; i < essay_word[essay_i].word_num; i++){
		InsertBST(BST_Root, essay_word[essay_i].word[i], essay_i);
	}
}

void InsertBST(Index *&p, char *key, int essay_id) {
	if (p == NULL) { //空树
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
	if (cmp_result < 0) //在左子树中插入
		InsertBST(p->lchild, key, essay_id);
	else if (cmp_result > 0) //在右子树中插入
		InsertBST(p->rchild, key, essay_id);
	else {
		//已有该结点，无需插入新索引结点，只要更新索引表信息
		EssayInfo *q = new EssayInfo;
		q->ID = essay_id;
		q->next = p->essay_head;
		p->essay_head = q;
	}
}

/* 根据区位码比较2个汉字序列的大小
输入: 2个汉字序列
输出: 0表示2个汉字序列相等;正数1表示前者大，负数1表示前者小
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

		//2个首字都是gb2312的常用汉字
		if ((firstIndex >= 0) && (secondIndex >= 0) && (firstIndex < secondIndex))
			return -1;
		else if ((firstIndex >= 0) && (secondIndex >= 0) && (firstIndex > secondIndex))
			return 1;
		//第1个汉字不在gb2312编码中(gbk中),第2个汉字是gb2312常用汉字
		else if ((firstIndex < 0) && (secondIndex >= 0))
			return 1;
		//第1个汉字是gb2312常用汉字，第2个汉字不在gb2312编码中(gbk中)
		else if ((firstIndex >= 0) && (secondIndex < 0))
			return -1;
		//2个汉字都不是gb2312常用汉字
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