// Sprig.cpp: Sprig クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////
#include "Sprig.h"
#include <iostream>

Para* Sprig::para = NULL;
Data* Sprig::data = NULL;

// コンストラクタ（新規）
Sprig::Sprig(short att)
{
	short i, j, tmp;

	// 領域確保
	chrom = new short [para->SprigChromLen];

	// 子の数と属性
	childNum = randomInt(para->ChildNodeNumMax - 2) + 2;
	chrom[0] = att;
	// クラス
	for(i = 1; i < para->SprigChromLen; i++)
		chrom[i] = -2;
	for(i = 0; i < childNum;) {
		tmp = randomInt(para->ChildNodeNumMax - 1) + 1;
		if(chrom[tmp] == -2) {
			chrom[tmp] = 0;
			i++;
		}
	}
	for(i = 1, j = -1; i < para->SprigChromLen; i++) {
		if(!chrom[i]) {
			chrom[i] = selectNodeKind(j);
			j = chrom[i];
		}
	}
	// 適応度初期化
	fitness = 1000000;
}

// デストラクタ
Sprig::~Sprig()
{
	delete [] chrom;
}

// 遺伝操作（突然変異のみ）
// x, y : 親となるsprig
void Sprig::newGeneration()
{
	mutate();
}

// 遺伝操作（交叉）
// x, y : 親となるsprig
void Sprig::newGeneration(Sprig* x, Sprig* y, short index)
{
	short i;

	for(i = 0; i < index; i++)
		chrom[i] = x->chrom[i];
	while(i < para->SprigChromLen)
		chrom[i] = y->chrom[i++];
	for(i = 1, childNum = 0; i < para->SprigChromLen; i++) {
		if(chrom[i] != -2)
			childNum++;
	}
	fix();
	mutate();
	fitness = 1000000;
}

// 子ノード遺伝子をランダムに選択
// return : 子ノードの表す遺伝子（-1～ClassNum-1）
short Sprig::selectNodeKind(short nouse)
{
	short tmp, i;
	tmp = randomInt(1);
	if(tmp) {	// 終端ノード
		do {
			i = (short)data->trainData[randomInt(data->trainDataNum - 1)][data->AttNum];
		} while(i == nouse);
		return i;
	} else {	// 非終端ノード
		return -1;
	}
}

// 不正な遺伝子を修正する
void Sprig::fix()
{
	short i, j, k, tmp;

	// 子の数が足りなければ追加
	while(childNum < 2) {
		tmp = randomInt(para->ChildNodeNumMax - 1) + 1;
		if(chrom[tmp] == -2) {
			chrom[tmp] = selectNodeKind(-1);
			childNum++;
		}
	}
	// 同じクラスが連続していれば修正
	for(j = 1; chrom[j] == -2; j++);
	for(i = 0; i < childNum - 1; i++) {
		for(k = j + 1; chrom[k] == -2; k++);
		if((chrom[k] != -1) && (chrom[k] != data->ClassNum) && (chrom[j] == chrom[k]))
			chrom[k] = selectNodeKind(chrom[j]);
		j = k;
	}
}

// 突然変異
void Sprig::mutate()
{
	bool flg = false;

	for(short i = 0; i < para->SprigChromLen; i++) {
		if(flip(para->ProbMutate)) {
			if(!i) {
				chrom[0] = randomInt(data->AttNum - 1);
			} else if(chrom[i] == -2) {
				chrom[i] = selectNodeKind(-1);
				childNum++;
			} else {
				if(randomInt(1)) {
					chrom[i] = selectNodeKind(chrom[i]);
				} else {
					chrom[i] = -2;
					childNum--;
				}
			}
			flg = true;
		}
	}
	if(flg) {
		fix();
		fitness = 1000000;
	}
}
