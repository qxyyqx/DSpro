#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include "index.h"
#include <string.h>
#include <iostream>
using namespace std;
extern Index *BST_Root;

// 在字典中查找关键字，获取倒排索引表
EssayInfo * find_essay(char *key) {
	Index *p = BST_Root;

	while (p != NULL) {
		int n = wordCmp(p->keyword, key);
		if (n == 0)
			break;
		else if (n > 0)
			p = p->lchild;
		else
			p = p->rchild;
	}	
	if (p == NULL)
		return NULL;
	else
		return p->essay_head;
}

// 布尔检索——“或”操作
void Orr(char* a, char* b)
{
	EssayInfo *p = find_essay(a);
	EssayInfo *q = find_essay(b);
	bool t = 0;
	FILE *m = fopen("info.txt","w");
	while (p || q) {
		if (t == 1)
			fprintf(m, "\n");
		else 
			t = 1;

		if (!p) {
			fprintf(m, "%d", q->ID);
			q = q->next;
		}
		else if (!q) {
			fprintf(m, "%d", p->ID);
			p = p->next;
		}
		else if (p->ID == q->ID) {
			fprintf(m, "%d", p->ID);
			p = p->next;
			q = q->next;
		}
		else if (p->ID > q->ID) {
			fprintf(m, "%d", p->ID);
			p = p->next;
		}
		else {
			fprintf(m, "%d", q->ID);
			q = q->next;
		}
	}
	fclose(m);
}

// 布尔检索——“与”操作
void Andd(char* a, char* b)
{
	EssayInfo *p = find_essay(a);
	EssayInfo *q = find_essay(b);
	FILE *m = fopen("info.txt", "w");
	bool t = 0;
	while (p && q) {
		if (t == 1)
			fprintf(m, "\n");
		else
			t = 1;

		if (p->ID == q->ID) {
			fprintf(m, "%d", p->ID);
			p = p->next;
			q = q->next;
		}
		else if (p->ID > q->ID) {
			p = p->next;
		}
		else 
			q = q->next;
	}
	fclose(m);
}

void search(char *aid) {
	char *p1, *p2, *p3;
	char *tmp;
	tmp = strtok(aid, "\n");
	p1 = strtok(tmp, " ");
	p2 = strtok(NULL, " ");
	if (p2&&strcmp(p2, "AND") == 0) {
		p3 = strtok(NULL, " ");
		Andd(p1, p3);
	}
	else if (p2&&strcmp(p2, "OR") == 0) {
		p3 = strtok(NULL, " ");
		Orr(p1, p3);
	}
	else
		Andd(p1, p1);
//	cout << "search end" << endl;
}