#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <codecvt>
#include <iomanip>
#include <Windows.h>
#include <cstdlib>
#define MAX_WORD_NUM 50
#define MAX_ESSAY_NUM 900

//每篇论文的分词数组
struct Word_List {
	char *word[MAX_WORD_NUM];
	int word_num;
};