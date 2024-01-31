// Data.cpp: Data クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////
#include "Data.h"
#include <cstring>
#include <iostream>
Para* Data::para = NULL;
using namespace std;

// コンストラクタ
// fname : データファイル名
Data::Data(char* dataName)
{
	short i, j;
	char fname[64];
	ifstream dataS;

	// infoデータの読み込み
	strcpy(dataname, dataName);
	sprintf(fname, "Data/%s.info", dataName);
	dataS.open(fname, ios :: in);
	if(!dataS) {
		cerr << "Cannot open " << fname << " for input.\n";
		exit(1);
	}
	dataS >> AttNum >> ClassNum >> allDataNum;
	dataS.close();

	// 各パラメータ設定
	if(!para->DepthMax)
		para->DepthMax = AttNum;

	// txtデータの読み込み
	sprintf(fname, "Data/%s.txt", dataName);
	dataS.open(fname, ios :: in);
	if(!dataS) {
		cerr << "Cannot open " << fname << " for input.\n";
		exit(1);
	}
	allData = new float* [allDataNum];
	for(i = 0; i < allDataNum; i++) {
		allData[i] = new float [AttNum + 1];
		for (j = 0; j < AttNum + 1; j++)
			dataS >> allData[i][j];
	}
	dataS.close();

	// 領域確保
	minAttValue = new float[AttNum];
	gapAttValue = new float[AttNum];
	if(para->CrossValNum == 1) {				// テスト用
		trainDataNum = allDataNum;
		testDataNum = allDataNum;
	} else if(allDataNum < para->CrossValNum) {	// データ数が少ないときへの対処
		para->CrossValNum = allDataNum;
		testDataNum = 1;
		trainDataNum = allDataNum - 1;
	} else {
		testDataNum = allDataNum / para->CrossValNum;
		trainDataNum = allDataNum - testDataNum;
	}
	testData = new float* [testDataNum];
	trainData = new float* [trainDataNum];
	crossNum = -1;
}

// デストラクタ
Data::~Data()
{
	for(short i = 0; i < allDataNum; i++)
	delete [] allData[i];
	delete [] allData;
	delete [] trainData;
	delete [] testData;
	delete [] minAttValue;
	delete [] gapAttValue;
}

// 訓練データとテストデータの作成
// n : クロスバリデーションの回数
void Data::makeTestData(short n)
{
	short i, j, k;

	crossNum = n;
	// データ作成
	if(trainDataNum == allDataNum) {		// テスト用
		for(i = 0; i < allDataNum; i++) {
			trainData[i] = allData[i];
			testData[i] = allData[i];
		}
	} else {
		for(i = 0, j = 0, k = 0; i < allDataNum; i++) {
			if((j < testDataNum) && (i % para->CrossValNum == n))
				testData[j++] = allData[i];
			else
				trainData[k++] = allData[i];
		}
	}
	// 各属性値の最低値と最高値との差を求める
	for(i = 0; i < AttNum; i++) {
		minAttValue[i] = trainData[0][i];
		gapAttValue[i] = trainData[0][i];
		for(j = 1; j < trainDataNum; j++) {
			if(trainData[j][i] < minAttValue[i])
				minAttValue[i] = trainData[j][i];
			if(gapAttValue[i] < trainData[j][i])
				gapAttValue[i] = trainData[j][i];
		}
		gapAttValue[i] = (gapAttValue[i] - minAttValue[i]) / 2.0 / (float)(para->ChildNodeNumMax - 1);
	}
}
