// Para.cpp: Para クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////
#include "Para.h"
#include <iostream>
#include <cstring>
using namespace std;

// コンストラクタ
// fname : パラメータファイル名
Para::Para(const char* fname)
{
	char paraName[64];

	// 各設定値の読み込み
	ifstream paraS(fname, ios::in);
	if(!paraS) {
		cerr << "Cannot open " << fname << " for input.\n";
		exit(1);
	}
	while(!paraS.eof()) {
		paraS.getline(paraName, 64, '\t');
		if(!strcmp(paraName, "TreeLog"))
			paraS >> TreeLog;
		else if(!strcmp(paraName, "TreeLog2"))
			paraS >> TreeLog2;
		else if(!strcmp(paraName, "CrossValNum"))
			paraS >> CrossValNum;
		else if(!strcmp(paraName, "ProbMutate"))
			paraS >> ProbMutate;
		else if(!strcmp(paraName, "TreePopNum"))
			paraS >> TreePopNum;
		else if(!strcmp(paraName, "SprigPopNum"))
			paraS >> SprigPopNum;
		else if(!strcmp(paraName, "ChildNodeNumMax"))
			paraS >> ChildNodeNumMax;
		else if(!strcmp(paraName, "GenerationMax"))
			paraS >> GenerationMax;
		else if(!strcmp(paraName, "DepthMax"))
			paraS >> DepthMax;
		paraS.getline(paraName, 64, '\n');
	}
	paraS.close();

	SprigChromLen = ChildNodeNumMax + 1;
}

// デストラクタ
Para::~Para()
{
}
