// fenci.cpp �����ķִ�ģ��
#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include "fenci.h"
using namespace std;

// ������ҳ�ķִ�����
Word_List essay_word[MAX_ESSAY_NUM];

#include "NLPIR.h"
char ffix[6] = ".html";

// �����ȡ�ļ���·��
void makepath(char *name, int n, int start, int ad) {
	name[start] = '\0';
	char *str = new char[ad + 1];
	_itoa(n, str, 10);
	strcat(name, str);
	strcat(name, ffix);
}

// ����ִ�����
void savv(const char *s,int n) {
	char *p, *q = new char[strlen(s)];
	strcpy(q, s);
	p = strtok(q, "#");
	int i = 0;
	while (p) {
		if (p[0] < 0) {
			essay_word[n].word[i] = p;
			i++;
		}
		p = strtok(NULL, "#");
	}
	essay_word[n].word_num = i;
}

// ʵ�����ķִ�ģ��
int fun(int length, char *path){

	// ��ʼ��
	if (!NLPIR_Init()) {
		printf("Init fails\n");
		return -1;
	}
	
	// �ļ�·��
	int len1, len2 = 0, len3 = 6, l = length+1;
	len1 = strlen(path);
	for (; l > 0; l = l / 10)
		len2++;
	char *name = new char[len1+len2+len3];
	strcpy(name, path);

	for (int i = 0; i < length; i++) {
		makepath(name, i, len1, len2);

		// �ÿ��ַ���ȡhtml�ļ�
		wifstream file(name);
		file.imbue(locale(locale::classic(), new std::codecvt_utf8<wchar_t>));
		if (!file) {
//			printf("fail to open file %s\n", name);
			file.close();
		}
		else {
			file.seekg(0, ios::end);
			size_t n = file.tellg();
			file.seekg(0, ios::beg);

			wchar_t *pt = new wchar_t[n];
			file.read(pt, n);
			pt[n] = '\0';
			file.close();

			int len = WideCharToMultiByte(CP_ACP, 0, pt, wcslen(pt), NULL, 0, NULL, NULL);
			char *p = new char[len + 1];
			WideCharToMultiByte(CP_ACP, 0, pt, wcslen(pt), p, len, NULL, NULL);
			p[len] = '\0';

			//nlpir���ķִ�
			const char *re = NLPIR_GetKeyWords(p);
			savv(re,i);

			delete[]p;
		}
	}
	NLPIR_Exit();
	delete[]name;
    return 0;
}