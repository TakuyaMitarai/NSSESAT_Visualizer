// Sprig.h: Sprig クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "myfunc.h"
#include "Data.h"
#include "TreeNode.h"
class TreeNode;

class Sprig
{
public:
	static Para* para;
	static Data* data;

	Sprig(short att);
	virtual ~Sprig();
	void newGeneration();
	void newGeneration(Sprig* x, Sprig* y, short index);

	short* chrom;			// 染色体
	short childNum;			// 子ノードの数（2～ChildNodeNumMax）
	float fitness;			// 評価値

private:
	short selectNodeKind(short nouse);
	void fix();
	void setMemVar();
	void mutate();
};