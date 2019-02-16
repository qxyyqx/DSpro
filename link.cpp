#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "link.h"
using namespace std;
extern VertexNode* HashTable[MAX_VERTEX_NUM];

unsigned int RS_Hash(char *str) {
	unsigned int a = 63689;
	unsigned int b = 378551;
	unsigned int v = 0;
	while (*str) {
		v = v * a + (*str++);
		a *= b;
	}
	return v % MAX_VERTEX_NUM;
}

unsigned int RealHash(char* url) {
	int addr = RS_Hash(url);
	int i = 0;

	while (i < MAX_VERTEX_NUM && HashTable[addr] != NULL && strcmp(HashTable[addr]->url, url) != 0) {
		addr++;
		addr %= MAX_VERTEX_NUM;
		i++;

	}

	// ˵���ڵ㲻����
	if (HashTable[addr] == NULL) {
		VertexNode* v = new VertexNode;

		// �����v���г�ʼ��
		// /TODO
		v->firstedge = NULL;
		v->hash_addr = addr;
		v->indegree = 0;
		v->outdegree = 0;
		v->srcid = -1;

		HashTable[addr] = v;
	}

	// HashTable[addr] ���� url ��Ӧ�Ľڵ㣬��������ͨ������Ĳ��裬�Ѿ���֤
	// HashTable[addr] һ����һ�����ڵĽڵ�
	return addr;
}

void CreateGraph(AdjList *adj) {
	//��ʼ����ͷ���.data
	char buf[2048], *p;
	unsigned int dstindex, srcindex; // index �� HashTable ���±�
	unsigned int srcid, dstid; // id �� adj->vertex ���±�
	int nr_vertex = 0, nr_edge = 0;

	FILE *fp = fopen("link.txt", "r");
//	assert(fp);
	while (!feof(fp)) {
		fgets(buf, 2048, fp);

		srcindex = RealHash(p = strtok(buf, ","));
		if (HashTable[srcindex]->srcid == -1) { // ����һ���½ڵ�
			HashTable[srcindex]->srcid = ++nr_vertex;
			strcpy(HashTable[srcindex]->url, p);

			adj->vertex[nr_vertex] = HashTable[srcindex];
		}

		srcid = HashTable[srcindex]->srcid; // ��ȡ�� adj->vertex ���±�

		while (p = strtok(NULL, ",")) {
			if (p[0] == '\n') continue;

			dstindex = RealHash(p);
			if (HashTable[dstindex]->srcid == -1) { // ����һ���½ڵ�
				HashTable[dstindex]->srcid = ++nr_vertex;
				strcpy(HashTable[dstindex]->url, p);

				adj->vertex[nr_vertex] = HashTable[dstindex];
			}

			dstid = HashTable[dstindex]->srcid; // ��ȡ�� adj->vertex ���±�

			EdgeNode* e = new EdgeNode; //����һ������

			e->dstid = dstid;
			e->next = adj->vertex[srcid]->firstedge;//�¼���Ľڵ㶼����ͷ���֮��ԭ����ͷ���֮��Ľڵ�Ҫ���ơ�

			adj->vertex[srcid]->firstedge = e;
			adj->vertex[srcid]->outdegree++;

			adj->vertex[dstid]->indegree++; //�õ��������

			nr_edge++;

		}
	}//���н�㶥�����ƣ���ַ

	adj->VexNum = nr_vertex;
	adj->ArcNum = nr_edge;

	fclose(fp);
}

//================================== Page Rank =================================
/* Here SM* A = trans(PageRank_A) */
static void init_matrixA(AdjList* adj, SM* A) {
	int i, j = 0; EdgeNode* edge;
	int nr_vertex = adj->VexNum;
	int nr_edge = adj->ArcNum;

	A->ROW = A->COL = nr_vertex;
	A->size = nr_edge;
	A->item = new Item[A->size];

	/* Here we make A to be (ALPHA * A) */
	/* ALPHA * A */
	for (i = 1; i <= nr_vertex; i++) {
		for (edge = adj->vertex[i]->firstedge; edge; edge = edge->next) {
			if (adj->vertex[edge->dstid]->indegree != 0) {
				A->item[j].col = edge->dstid - 1;
				A->item[j].row = i - 1;
				A->item[j++].val = PR_ALPHA / adj->vertex[i]->outdegree;
			}
		}
	}
}

/* Here SM* B = transverse(Page_Rank_B) */
static void init_matrixB(AdjList* adj, SM* B) {
	int i;
	int nr_vertex = adj->VexNum, nr_edge = adj->ArcNum;
	double init_val = PR_ALPHA / nr_vertex;

	B->COL = nr_vertex; B->ROW = 1;
	B->size = nr_vertex;
	B->dirty = false;
	B->item = new Item[B->size];

	for (i = 0; i < nr_vertex; i++) {
		B->item[i].row = 0;
		B->item[i].col = i;
		B->item[i].val = init_val;
	}
}

static void page_rank_matrix_mul(AdjList* adj, SM* B, SM* A, int* row_start) {
	int nr_vertex = adj->VexNum, nr_edge = adj->ArcNum;
	double *result = NULL, c_val = (1 - PR_ALPHA) / nr_vertex;
	int i, cur, cur_col, cur_row;

	result = new double[B->size];

	for (i = 0; i < B->size; i++) {
		result[i] = 0;
	}

	/* result = (A * preB) */
	cur = 0;
	while (cur < B->size) {
		/* c[i][j] = sum( a[i][k] * b[k][j] ) */
		cur_row = B->item[cur].col;
		for (i = row_start[cur_row]; i < row_start[cur_row + 1]; i++) {
			cur_col = A->item[i].col;
			result[cur_col] += B->item[cur].val * A->item[i].val;
		}
		cur++;
	}

	/* curB = result + C */
	for (i = 0; i < B->size; i++) {
		double gap = (B->item[i].val - result[i] - c_val);
		if (gap < -1e-6 || gap > 1e-6) {
			B->item[i].val = result[i] + c_val;
			B->dirty = true;
		}
	}

	delete[] result;
}

static void cpy2vertex(AdjList* adj, SM* B) {
	int i, nr_vertex = adj->VexNum;
	for (i = 0; i < nr_vertex; i++) {
		adj->vertex[i + 1]->cost = B->item[i].val;
	}
}

void init_cost(AdjList* adj) {
	/* A is for the real sparse matrix of the linking relation */
	/* B is for the cost vector of every vertex */
	SM* matrixA = new SM;
	SM* matrixB = new SM;
	int i = 0;
	// int* row_size = NULL, *row_start = NULL;

	init_matrixA(adj, matrixA);
	init_matrixB(adj, matrixB);

	int* row_size = new int[matrixA->ROW];
	int* row_start = new int[matrixA->ROW + 1];

	for (i = 0; i < matrixA->ROW; i++) {
		row_size[i] = 0;
	}

	for (i = 0; i < matrixA->size; i++) {
		row_size[matrixA->item[i].row] ++;
	}

	row_start[0] = 0;
	for (i = 1; i <= matrixA->ROW; i++) {
		row_start[i] = row_start[i - 1] + row_size[i - 1];
	}

	/* do Page Rank */
	do {
		matrixB->dirty = false;
		page_rank_matrix_mul(adj, matrixB, matrixA, row_start);
	} while (matrixB->dirty == true);

	cpy2vertex(adj, matrixB);

	delete[]row_size;
	delete[]row_start;

	delete[]matrixA->item;
	delete[]matrixB->item;

	delete matrixA;
	delete matrixB;
}

//==============================================================================

void QsortPR(VertexNode a[], int low, int high)
{
	if (low >= high)
	{
		return;
	}
	int first = low;
	int last = high;
	//	int key = a[first];/*���ֱ�ĵ�һ����¼��Ϊ����*/
	double key = a[first].cost;

	while (first < last)
	{
		while (first < last && a[last].cost <= key)
		{
			--last;
		}
		a[first] = a[last];/*���ȵ�һ��С���Ƶ��Ͷ�*/

		while (first < last && a[first].cost >= key)
		{
			++first;
		}

		a[last] = a[first];
		/*���ȵ�һ������Ƶ��߶�*/
	}
	a[first].cost = key;/*�����¼��λ*/
	QsortPR(a, low, first - 1);
	QsortPR(a, first + 1, high);
}