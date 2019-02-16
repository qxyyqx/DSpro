#include "stdafx.h"
#include <iostream>
using namespace std;
#include "link.h"
extern void QsortPR(VertexNode a[], int low, int high);
extern unsigned int RealHash(char* url);
extern VertexNode* HashTable[MAX_VERTEX_NUM];

void getAllRightAns()
{
	VertexNode allVertex[MAX_VERTEX_NUM];
	int index = 0;
	FILE *fa = fopen("info.txt", "r");
	int numOfAllVertex = 0;
	while (!feof(fa))
	{
		char buf[10];
		fgets(buf, 10, fa);
		if (buf[0] != '\n')
		{
			int num = atoi(buf);
			for (int i = 0; i < 10; i++)
			{
				if (buf[i] == '\n')
				{
					buf[i] = '\0';
					break;
				}
			}
			char url[64] = { '\0' };
			FILE *fb = fopen("allVertex.txt", "r");
			int count = 0;

			while (!feof(fb))
			{
				fgets(url, 64, fb);
				if (count == num)
				{
					for (int i = 0; i < 64; i++)
					{
						if (url[i] == '\n' || url[i] == ',')
						{
							url[i] = '\0';
							break;
						}
					}
					break;
				}
				count++;
			}
			int hash_addr = RealHash(url);
			VertexNode tmp = *HashTable[hash_addr];
			allVertex[index] = tmp;
			std::cout << tmp.cost << endl;
			std::cout << tmp.url << endl;
			index++;
			numOfAllVertex++;
		}
	}
	//将所有符合要求的索引结果放进allVertex

	/*****根据pr值进行一次快排******/
	QsortPR(allVertex, 0, numOfAllVertex - 1);
	for (int i = 0; i < numOfAllVertex; i++)
		std::cout << allVertex[i].cost << endl;
	FILE *fr = fopen("orderedURL.txt", "w");
	for (int i = 0; i < numOfAllVertex; i++)
	{
		fwrite(allVertex[i].url, strlen(allVertex[i].url), 1, fr);
		fwrite("\n", sizeof(char), 1, fr);
	}
	fclose(fr);
}