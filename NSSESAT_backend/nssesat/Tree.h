// Tree.h: Tree ?N???X??C???^?[?t?F?C?X
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "TreeNode.h"
#include <vector>
using namespace std;

class Tree
{
public:
	static Para* para;
	static Data* data;

	Tree();
	Tree(Tree* model);
	virtual ~Tree();
	void newGeneration(Tree* model);
	void newGeneration(Tree* p1, Tree* p2, int an, int bn);
	TreeNode* getNode(int i);
	void modify();
	void calcFit();
	void traverse(float* adata);
	void evalInit();
	void mutate();
	void setNodeNum();
	void setDepth();
	void print(TreeNode* subRoot);
	void printDot(TreeNode* subRoot, ofstream& outputfile2);
	void write_label(TreeNode* subRoot, int num, ofstream& outputfile2);
	void printret(TreeNode* subRoot, int num);
	void write_branch(vector<double> rinfo, ofstream& outputfile2);
	void generate_dot(ofstream& outputfile2);

	TreeNode *root;		// ?????m?[?h???w???|?C???^
	int nodeNum;		// ?m?[?h??
	short depth;		// ???[??
	double fitness;		// ?]???l
	double rankfit;
	double entropy;
	double accuracy;		// ?P????????
	short processNum;	// ?????????P??????
	short correctNum;	// ?????????P????????
	vector<TreeNode*> child_root;
	vector<vector<double>> root_info;
	int itr;

private:
	void evalInit2();
};