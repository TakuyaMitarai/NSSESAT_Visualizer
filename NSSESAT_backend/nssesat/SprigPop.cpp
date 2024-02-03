// SprigPop.cpp: SprigPop クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////
#include "SprigPop.h"
#include <iostream>
using namespace std;

Para* SprigPop::para = NULL;
Data* SprigPop::data = NULL;

// コンストラクタ（新規）
SprigPop::SprigPop()
{
	pop = new Sprig* [para->SprigPopNum];
	for(short i = 0; i < para->SprigPopNum; i++)
		pop[i] = new Sprig(i % data->AttNum);
}

// デストラクタ
SprigPop::~SprigPop()
{
	for(short i = 0; i < para->SprigPopNum; i++)
		delete pop[i];
	delete [] pop;
}

// 次世代生成
void SprigPop::newGeneration()
{
	short i, j, a, b, c, tmp1, tmp2, fit1;
	int TOURNAMENT_SIZE = 3;
	
	for(i = para->SprigPopNum / 2; i < para->SprigPopNum; i++) {
		tmp1 = randomInt(para->SprigPopNum-1);
		fit1 = pop[tmp1]->fitness;
		for(j = 0; j < TOURNAMENT_SIZE; j++) {
			a = randomInt(para->SprigPopNum-1);
			if(fit1 > pop[a]->fitness) {
				fit1 = pop[a]->fitness;
				tmp1 = a;
			}
		}
		tmp2 = randomInt(para->SprigPopNum-1);
		fit1 = pop[tmp2]->fitness;
		for(j = 0; j < TOURNAMENT_SIZE; j++) {
			a = randomInt(para->SprigPopNum-1);
			if(fit1 > pop[a]->fitness) {
				fit1 = pop[a]->fitness;
				tmp2 = a;
			}
		}
		c = randomInt(para->SprigChromLen - 1);
		pop[i]->newGeneration(pop[tmp1], pop[tmp2], c);
	}
}

// popをfitnessの値によって降順にソート（クイックソート）
// lb : ソートするpopの範囲の最小インデックス
// rb : ソートするpopの範囲の最大インデックス
void SprigPop::sort(short lb, short ub)
{
	short i, j, k;
	float pivot;
	Sprig* swap;

	if(lb < ub) {
		k = (lb + ub) / 2;
		pivot = pop[k]->fitness;
		i = lb;
		j = ub;
		do {
			while(pop[i]->fitness < pivot)
				i++;
			while(pop[j]->fitness > pivot)
				j--;
			if(i <= j) {
				swap = pop[i];
				pop[i] = pop[j];
				pop[j] = swap;
				i++;
				j--;
			}
		} while(i <= j);
		sort(lb, j);
		sort(i, ub);
	}
}