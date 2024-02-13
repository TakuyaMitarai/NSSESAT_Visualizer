// TreeNode.h: TreeNode クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "SprigPop.h"
class SprigPop;
class Sprig;

class TreeNode
{
public:
	static Para* para;
	static Data* data;
	static SprigPop* sprigPop;

	TreeNode(TreeNode* aParent);
	TreeNode(TreeNode* aParent, short aClassNo);
	TreeNode(TreeNode* aParent, TreeNode* model);
	virtual ~TreeNode();
	void upNode();
	void makeTree();
	void makeInternal(short i);
	void makeInternal2(short i);
	void copyTree(TreeNode* model);
	TreeNode* getNode(int n);
	int countNode();
	void setLevel();
	short maxLevel();
	void newGeneration();
	short traverse(float* adata);
	void simplify();
	void simplify2();
	short modify();
	float calcEnt();
	void sprigEval(float treeFit);
	void evalInitA();
	bool same(TreeNode* b);
	void addPCNum(TreeNode* a);

	Sprig* sprig;
	short childNum;
	short attribute;
	short classNo;
	short level;
	TreeNode* parent;
	TreeNode** child;
	float fitness;		// ノード評価用
	short processNum;
	short correctNum;
	short* ballot;
private:
	void evalInitA2();
};