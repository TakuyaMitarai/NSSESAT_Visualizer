// Data.h: Data クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <math.h>
#include "Para.h"

class Data  
{
public:
	static Para* para;

	Data(char* dname);
	virtual ~Data();
	void makeTestData(short n);

	char dataname[64];	// データの名前
	float *minAttValue;	// 各属性値の最低値
	float *gapAttValue;	// 各属性値の最高値と最低値の差
	short AttNum;		// 属性数
	short ClassNum;		// クラス数
	short allDataNum;	// 全データ数
	short trainDataNum;	// 訓練データ数
	short testDataNum;	// テストデータ数
	short crossNum;		// クロスバリデーションの回数
	float **allData;	// 全データ
	float **trainData;	// 訓練データ
	float **testData;	// テストデータ
};
