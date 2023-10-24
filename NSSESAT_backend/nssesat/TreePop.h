// TreePop.h: TreePop クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Tree.h"
#include <vector>
using namespace std;

class TreePop
{
public:
	static Para* para;
	static Data* data;
	int genNum;

	TreePop();
	virtual ~TreePop();
	void newGeneration();
	void Paretofit();
	void CrowdedDistance(int i, vector<int> rank_s);

	Tree* bestTree;
	Tree** pop;
};