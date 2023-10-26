// Exp.cpp: Exp クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////
#include "Exp.h"
#include <iostream>
#include <fstream>
using namespace std;

// コンストラクタ
// dataFname : データファイル名
// pafaFname : パラメータファイル名
Exp::Exp(char* dataName, const char* paraFname)
{
	para = new Para(paraFname);
	Data::para = para;
	TreePop::para = para;
	Tree::para = para;
	TreeNode::para = para;
	SprigPop::para = para;
	Sprig::para = para;
	data = new Data(dataName);
	TreePop::data = data;
	Tree::data = data;
	TreeNode::data = data;
	SprigPop::data = data;
	Sprig::data = data;

	// 各メンバ初期化
	sprigPop = NULL;
	treePop = NULL;
	bestGen = -1;
}

// デストラクタ
Exp::~Exp()
{
	delete para;
	delete data;
	if(treePop)
		delete treePop;
	if(sprigPop)
		delete sprigPop;
}

// 新たなクロスバリデーションの開始にあたり，各メンバを初期化
// n : クロスバリデーションの回数
void Exp::initialize(short n)
{
	char outFname[64], outFname2[64];

	// 訓練データとテストデータの生成
	data->makeTestData(n);

	// 各メンバ初期化
	if(treePop) {
		delete treePop;
		treePop = NULL;
	}
	if(sprigPop) {
		delete sprigPop;
		sprigPop = NULL;
	}
	bestFit = 0;
	bestGen = 0;

	// 乱数初期化
	srand(time(NULL));


	cout << "適合率\t正解率\tノード数\t木の深さ\t平均適合率\t平均正解率\t平均ノード数\n";
}

// 学習フェーズ
void Exp::training()
{
	int i;
	cout << data->dataname << endl;
	start = clock();

	// 初期集団生成＆評価
	sprigPop = new SprigPop();
	TreeNode::sprigPop = sprigPop;
	treePop = new TreePop();
	sprigPop->sort(0, para->SprigPopNum - 1);
	//log();
	para->DepthMax = 80;
	while(treePop->genNum <= para->GenerationMax) {
		cout << "第" << treePop->genNum << "世代" << endl;
		newGeneration(); // 新しい世代を生成
		/*
		for(i = 0; i < para->TreePopNum; i++) {
			cout << treePop->pop[i] << " ";
			cout << treePop->pop[i]->root->classNo << " ";
			cout << treePop->pop[i]->root->childNum << " ";
			cout << treePop->pop[i]->root->attribute << " ";
			treePop->pop[i]->print(treePop->pop[i]->root);
		}
		*/
		//log();
	}
	finish = clock();
	mktime = (float)(finish - start) / CLOCKS_PER_SEC;
	
	for(i = 0; i < para->TreePopNum / 2; i++) {
		//cout << treePop->pop[i]->accuracy << " " << treePop->pop[i]->entropy << " " << treePop->pop[i]->fitness << endl;
		cout << treePop->pop[i]->accuracy << " " << treePop->pop[i]->entropy << endl;
	}
}

// テストフェーズ
void Exp::test()
{
	ofstream outputfile1("../result.txt");
	ofstream outputfile2("treedotfile.txt");

	int i;
	for(i = 0; i < para->TreePopNum / 2; i++) {
		for(short j = 0; j < data->testDataNum; j++)
			treePop->pop[i]->traverse(data->testData[j]);
		treePop->pop[i]->calcFit();
		outputfile1 << treePop->pop[i]->accuracy << " " << treePop->pop[i]->entropy << endl;
	}
	for(i = 0; i < para->TreePopNum / 2; i++) {
		// treePop->pop[i]->print(treePop->pop[i]->root);
		outputfile2 << i << endl;
		treePop->pop[i]->printDot(treePop->pop[i]->root, outputfile2);
	}
}

// 新しい世代を生成
void Exp::newGeneration()
{
	sprigPop->newGeneration();
	treePop->newGeneration();
	sprigPop->sort(0, para->SprigPopNum - 1);
}

// ログ出力
void Exp::log()
{
	double avefit , aveacc, avenode;

	if(para->TreeLog);				// 途中経過をファイルに書き込む
	// 最良解を出力
	if(para->TreeLog2) {
		avefit = 0.0;
		aveacc = 0.0;
		avenode = 0.0;
		for(int i = 0; i < para->TreePopNum; i++) {
			avefit += treePop->pop[i]->fitness;
			aveacc += treePop->pop[i]->accuracy;
			avenode += treePop->pop[i]->nodeNum;
		}
		avefit /= para->TreePopNum;
		aveacc /= para->TreePopNum;
		avenode /= para->TreePopNum;
	}
}