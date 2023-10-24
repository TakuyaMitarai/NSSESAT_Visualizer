// TreeNode.cpp: TreeNode クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////
#include "TreeNode.h"
#include <iostream>
using namespace std;

Para* TreeNode::para = NULL;
Data* TreeNode::data = NULL;
SprigPop* TreeNode::sprigPop = NULL;

// コンストラクタ（非終端ノード）
// aParent : 親ノードへのポインタ
TreeNode::TreeNode(TreeNode* aParent)
{
	sprig = sprigPop->pop[randomInt(para->SprigPopNum - 1)];
	childNum = sprig->childNum;
	attribute = sprig->chrom[0];
	classNo = -1;
	parent = aParent;
	child = new TreeNode* [para->ChildNodeNumMax];
	for(short i = 0; i < para->ChildNodeNumMax; i++)
		child[i] = NULL;
	ballot = NULL;
	if(parent)
		level = parent->level + 1;
	else
		level = 0;
	evalInitA2();
}

// コンストラクタ（終端ノード）
// aParent : 親ノードへのポインタ
// aClassNo : クラス
TreeNode::TreeNode(TreeNode* aParent, short aClassNo)
{
	sprig = NULL;
	childNum = 0;
	attribute = -1;
	classNo = aClassNo;
	parent = aParent;
	child = NULL;
	ballot = new short[data->ClassNum];
	for(short i = 0; i < data->ClassNum; i++)
		ballot[i] = 0;
	if(parent)
		level = parent->level + 1;
	else
		level = 0;
	evalInitA2();
}

// コンストラクタ（コピー）
// aParent : 親ノードへのポインタ
// model : 手本とするノードへのポインタ
TreeNode::TreeNode(TreeNode* aParent, TreeNode* model)
{
	short i;

	sprig = model->sprig;
	childNum = model->childNum;
	attribute = model->attribute;
	classNo = model->classNo;
	parent = aParent;
	if(classNo == -1) { //非終端ノード
		child = new TreeNode* [para->ChildNodeNumMax];
		for(i = 0; i < para->ChildNodeNumMax; i++)
			child[i] = NULL;
		ballot = NULL;
	} else {	//終端ノード
		child = NULL;
		ballot = new short[data->ClassNum];
		/*
		if(model->sprig != NULL) {
			cout << "2True2" << endl;
			cout << classNo << endl;
			cout << childNum << endl;
			cout << attribute << endl;
		}
		*/
		for(i = 0; i < data->ClassNum; i++)
			ballot[i] = model->ballot[i];	//原因 model->sprig != NULL error 終端ノードにも関わらず、子ノードを持つ
	}
	if(parent)
		level = parent->level + 1;
	else
		level = 0;
	fitness = model->fitness;
	processNum = model->processNum;
	correctNum = model->correctNum;
}

// デストラクタ
TreeNode::~TreeNode()
{
	if(child) {
		for(short i = 0; i < para->ChildNodeNumMax; i++) {
			if(child[i])
				delete child[i];
		}
		delete [] child;
	} else {
		delete [] ballot;
	}
}

// （子が１つになったとき用）
void TreeNode::upNode()
{
	short i;
	TreeNode* model;
	TreeNode** oldchild = child;

	for(i = 0; !child[i]; i++);
	model = child[i];
	sprig = model->sprig;
	childNum = model->childNum;
	attribute = model->attribute;
	classNo = model->classNo;
	if(classNo == -1) {
		child = new TreeNode* [para->ChildNodeNumMax];
		for(i = 0; i < para->ChildNodeNumMax; i++) {
			child[i] = model->child[i];
			if(child[i]) {
				child[i]->parent = this;
				child[i]->setLevel();
			}
		}
	} else {
		ballot = new short[data->ClassNum];
		for(i = 0; i < data->ClassNum; i++)
			ballot[i] = 0;
		child = NULL;
	}
	fitness = model->fitness;
	processNum = model->processNum;
	correctNum = model->correctNum;
	for(i = 0; i < para->ChildNodeNumMax; i++)
		oldchild[i] = NULL;
	delete [] oldchild;
	if(model->child) {
		for(i = 0; i < para->ChildNodeNumMax; i++)
			model->child[i] = NULL;
	}
	delete model;
}

// i番目の子として内部節点を生成
void TreeNode::makeInternal(short i)
{
	if(level == para->DepthMax - 1) {
		child[i] = new TreeNode(this, data->ClassNum);
	} else {
		child[i] = new TreeNode(this);
		child[i]->makeTree();
	}
}

void TreeNode::makeInternal2(short i)
{
	if(level == para->DepthMax - 1) {
		child[i] = new TreeNode(this, data->ClassNum);
	} else {
		child[i] = new TreeNode(this);
		child[i]->makeTree();
	}
}

// 部分木を生成
void TreeNode::makeTree()
{
	for(short i = 0; i < para->ChildNodeNumMax; i++) {
		if(sprig->chrom[i+1] == -1)			// 非終端ノード
			makeInternal(i);
		else if(sprig->chrom[i+1] != -2)	// 終端ノード
			child[i] = new TreeNode(this, sprig->chrom[i+1]);
	}
}

// 部分木をコピー
// modelは非終端ノードでないとだめ
void TreeNode::copyTree(TreeNode* model)
{
	if(model->child) {
		for(short i = 0; i < para->ChildNodeNumMax; i++) {
			if(model->child[i]) {
				if((level == para->DepthMax - 1) && model->child[i]->child) {
					child[i] = new TreeNode(this, data->ClassNum);
				} else {
					child[i] = new TreeNode(this, model->child[i]);
					child[i]->copyTree(model->child[i]);
				}
			}
		}
	}
}

// 次世代生成
void TreeNode::newGeneration()
{
	if(child) {
		attribute = sprig->chrom[0];
		childNum = sprig->childNum;
		for(short i = 0; i < para->ChildNodeNumMax; i++) {
			if(child[i]) {
				if(sprig->chrom[i+1] == -1) {	// 今度は内部節点
					if(child[i]->child) {			// もともと内部
						child[i]->newGeneration();
					} else {						// もともと葉
						delete child[i];
						makeInternal(i);
					}
				} else if(sprig->chrom[i+1] != -2) {	// 今度は葉
					if(child[i]->child) {			// もともと内部
						delete child[i];
						child[i] = new TreeNode(this, sprig->chrom[i+1]);
					} else {						// もともと葉
						child[i]->classNo = sprig->chrom[i+1];
					}
				} else {						// 今度は子なし
					delete child[i];
					child[i] = NULL;
				}
			} else {
				if(sprig->chrom[i+1] == -1)	// 今度は内部節点
					makeInternal(i);
				else if(sprig->chrom[i+1] != -2)	// 今度は葉
					child[i] = new TreeNode(this, sprig->chrom[i+1]);
			}
		}
	}
}

// preorderでn番目のノードを返す
// n : 探したいノードの番号
// return : preorderでn番目のノードへのポインタ
TreeNode* TreeNode::getNode(int n)
{
	int m, node;
	short i;

	if(n == 1) {
		return this;
	} else {
		m = n;
		node = 1;
		for(i = 0; node < m; i++) {
			if(child[i]) {
				m -= node;
				node = child[i]->countNode();
			}
		}
		return child[i-1]->getNode(m);
	}
}

// 木のノード数を数える
int TreeNode::countNode()
{
	int ret = 1;
	if(child) {
		for(short i = 0; i < para->ChildNodeNumMax; i++) {
			if(child[i])
				ret += child[i]->countNode();
		}
	}
	return ret;
}

// ノードのレベルを設定する．
void TreeNode::setLevel()
{
	if(parent)
		level = parent->level + 1;
	else
		level = 0;
	if(child) {
		for(short i = 0; i < para->ChildNodeNumMax; i++) {
			if(child[i])
				child[i]->setLevel();
		}
	}
}

// 木の深さを調べる
short TreeNode::maxLevel()
{
	short max, x, i;

	if(child) {
		for(i = 0, max = -1; i < para->ChildNodeNumMax; i++) {
			if(child[i]) {
				x = child[i]->maxLevel();
				if(max < x)
					max = x;
			}
		}
		return max;
	} else {
		return level;
	}
}

// 木を探索
// data : データ
// return : 正解なら1，不正解なら0
short TreeNode::traverse(float* adata)
{
	short ret, i, j;

	if(child) {		// 非終端ノードの場合
		for(i = 0; !child[i]; i++);
		while(1) {
			for(j = i + 1; (j < para->ChildNodeNumMax) && !child[j]; j++);
			if((j == para->ChildNodeNumMax) ||
				(adata[attribute] < (float)(i+j) * data->gapAttValue[attribute] + data->minAttValue[attribute]))
				break;
			i = j;
		}
		ret = child[i]->traverse(adata);
	} else {		// 終端ノードの場合
		if(classNo == (short)adata[data->AttNum])
			ret = 1;
		else
			ret = 0;
		if(classNo == data->ClassNum)
			ballot[(short)adata[data->AttNum]]++;
	}
	processNum++;
	correctNum += ret;
	return ret;
}

// processNumが0の部分木を削除
void TreeNode::simplify()
{
	short i;

	if(child) {
		for(i = 0; i < para->ChildNodeNumMax; i++) {
			if(child[i] && !child[i]->processNum) {
				delete child[i];
				child[i] = NULL;
				childNum--;
			}
		}
		// 子が減ったら調整
		if(childNum < 2) {
			upNode();
			simplify();
		} else {
			for(i = 0; i < para->ChildNodeNumMax; i++) {
				if(child[i])
					child[i]->simplify();
			}
		}
	}
}

void TreeNode::addPCNum(TreeNode* a)
{
	processNum += a->processNum;
	correctNum += a->correctNum;
	if(child) {
		for(short i = 0; i < para->ChildNodeNumMax; i++) {
			if(child[i])
				child[i]->addPCNum(a->child[i]);
		}
	}
}

// クラス不定の葉ノードに関する処理
// return : クラス不定の葉ノードの正解数
short TreeNode::modify()
{
	short i, ret;

	ret = 0;
	if(child) {		// 非終端ノードの場合
		// クラス不定の葉ノードに関する処理を全ての子ノードについて行なう
		for(i = 0; i < para->ChildNodeNumMax; i++) {
			if(child[i])
				ret += child[i]->modify();
		}
	} else {		// 終端ノードの場合
		if(classNo == data->ClassNum) {
			for(i = 0; i < data->ClassNum; i++) {
				if(ret < ballot[i]) {
					ret = ballot[i];
					classNo = i;
				}
			}
			if(classNo == data->ClassNum)
				classNo = randomInt(data->ClassNum - 1);
		}
	}
	correctNum += ret;
	return ret;
}

// 隣り合う同じ部分木を統合する
void TreeNode::simplify2()
{
	short i, j;

	if(child) {		// 非終端ノードの場合
		for(i = 0; i < para->ChildNodeNumMax; i++) {
			if(child[i])
				child[i]->simplify2();
		}
		for(i = 0; !child[i]; i++);
		while(1) {
			for(j = i + 1; (j < para->ChildNodeNumMax) && !child[j]; j++);
			if(j == para->ChildNodeNumMax)
				break;
			if(child[i]->same(child[j])) {
				child[j]->addPCNum(child[i]);
				delete child[i];
				child[i] = NULL;
				childNum--; 
			}
			i = j;
		}
		if(childNum < 2)
			upNode();
	}
}

// エントロピーを計算
float TreeNode::calcEnt()
{
	float ent = 0.0;
	if(child) {
		for(short i = 0; i < para->ChildNodeNumMax; i++) {
			if(child[i])
				ent += child[i]->calcEnt();
		}
	} else {
		if(correctNum)
			ent = (float)correctNum * (float)log((double)correctNum) / log(2.0);
	}
	return ent;
}

// sprigのfitnessを設定
void TreeNode::sprigEval(float treeFit)
{
	short i;

	fitness = treeFit;
	if(child) {
		if(sprig->fitness > fitness) {
			sprig->fitness = fitness;
			sprig->childNum = childNum;
			sprig->chrom[0] = attribute;
			for(i = 0; i < para->ChildNodeNumMax; i++) {
				if(child[i])
					sprig->chrom[i+1] = child[i]->classNo;
				else
					sprig->chrom[i+1] = -2;
			}
		}
		for(i = 0; i < para->ChildNodeNumMax; i++) {
			if(child[i])
				child[i]->sprigEval(treeFit);
		}
	}
}

// 評価用変数の初期化
void TreeNode::evalInitA()
{
	short i;

	evalInitA2();
	if(child) {
		for(i = 0; i < para->ChildNodeNumMax; i++) {
			if(child[i])
				child[i]->evalInitA();
		}
	} else {
			for(i = 0; i < data->ClassNum; i++)
				ballot[i] = 0;
	}
}

// 評価用変数の初期化
void TreeNode::evalInitA2()
{
	processNum = 0;
	correctNum = 0;
}

// 部分木を比較
bool TreeNode::same(TreeNode* b)
{
	short i;

	if(childNum == b->childNum) {
		if(!childNum) {
			if(classNo == b->classNo)
				return true;
		} else if (attribute == b->attribute) {
			for(i = 0; i < para->ChildNodeNumMax; i++) {
				if(child[i]) {
					if(!b->child[i])
						return false;
					else if(!child[i]->same(b->child[i]))
						return false;
				} else {
					if(b->child[i])
						return false;
				}
			}
			return true;
		}
	}
	return false;
}
