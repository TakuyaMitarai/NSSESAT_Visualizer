// Tree.cpp: Tree �ｿｽN�ｿｽ�ｿｽ�ｿｽX�ｿｽﾌイ�ｿｽ�ｿｽ�ｿｽv�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽe�ｿｽ[�ｿｽV�ｿｽ�ｿｽ�ｿｽ�ｿｽ
//
//////////////////////////////////////////////////////////////////////
#include "Tree.h"
#include <iostream>
#include <fstream>
using namespace std;
Para* Tree::para = NULL;
Data* Tree::data = NULL;

// �ｿｽR�ｿｽ�ｿｽ�ｿｽX�ｿｽg�ｿｽ�ｿｽ�ｿｽN�ｿｽ^�ｿｽi�ｿｽV�ｿｽK�ｿｽj
Tree::Tree()
{
	root = new TreeNode(NULL);
	root->makeTree();
	setNodeNum();
	setDepth();
	evalInit2();
}

// �ｿｽR�ｿｽ�ｿｽ�ｿｽX�ｿｽg�ｿｽ�ｿｽ�ｿｽN�ｿｽ^�ｿｽi�ｿｽR�ｿｽs�ｿｽ[�ｿｽj
Tree::Tree(Tree* model)
{
	if(model) {
		root = new TreeNode(NULL, model->root);
		root->copyTree(model->root);
		nodeNum = model->nodeNum;
		depth = model->depth;
		fitness = model->fitness;
		accuracy = model->accuracy;
		processNum = model->processNum;
		correctNum = model->correctNum;
	} else {
		root = NULL;
		nodeNum = 0;
		depth = 0;
		evalInit2();
	}
}

// �ｿｽf�ｿｽX�ｿｽg�ｿｽ�ｿｽ�ｿｽN�ｿｽ^
Tree::~Tree()
{
	if(root)
		delete root;
}

// �ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ辮ｶ�ｿｽ�ｿｽ�ｿｽimodel�ｿｽﾆ難ｿｽ�ｿｽ�ｿｽ�ｿｽﾘゑｿｽ�ｿｽ�ｿｽ�ｿｽj
void Tree::newGeneration(Tree* model)
{
	if(root)
		delete root;
	root = new TreeNode(NULL, model->root);
	root->copyTree(model->root);
	nodeNum = model->nodeNum;
	depth = model->depth;
	root->newGeneration();
	setNodeNum();
	setDepth();
	mutate();
}

// �ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ辮ｶ�ｿｽ�ｿｽ�ｿｽi�ｿｽQ�ｿｽﾂの木ゑｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽj
void Tree::newGeneration(Tree* p1, Tree* p2, int an, int bn)
{
	TreeNode* a;
	TreeNode* b;
	TreeNode* c;
	TreeNode* parent;
	short i;

	if(root)
		delete root;
	root = new TreeNode(NULL, p1->root); //�ｿｽo�ｿｽO
	root->copyTree(p1->root);
	a = getNode(an);
	b = p2->getNode(bn);

	if(a->parent) {
		parent = a->parent;
		for(i = 0; i < para->ChildNodeNumMax; i++) {
			if(parent->child[i] && (parent->child[i] == a))
				break;
		}
		if((parent->level == para->DepthMax - 1) && b->child) {
			parent->child[i] = new TreeNode(parent, data->ClassNum);
		} else {
			parent->child[i] = new TreeNode(parent, b);
			parent->child[i]->copyTree(b);
		}
	} else {
		root = new TreeNode(NULL, b);
		root->copyTree(b);
	}
	delete a;
	setNodeNum();
	setDepth();
	mutate();
}

// �ｿｽﾋ然�ｿｽﾏ茨ｿｽ
void Tree::mutate()
{
	short i;

	if(flip(para->ProbMutate/5)) {
		TreeNode* a = getNode(randomInt(nodeNum - 1) + 1);
		if(!a->parent) {
			delete a;
			root = new TreeNode(NULL);
			root->makeTree();
		} else if(a->child) {
			TreeNode* parent = a->parent;
			for(i = 0; i < para->ChildNodeNumMax; i++) {
				if(parent->child[i] && (parent->child[i] == a))
					break;
			}
			delete a;
			parent->makeInternal(i);
		} else {
			a->classNo = data->ClassNum;
		}
		setNodeNum();
		setDepth();
	}
}

// preorder�ｿｽ�ｿｽn�ｿｽﾔ目のノ�ｿｽ[�ｿｽh�ｿｽ�ｿｽﾔゑｿｽ
// n : �ｿｽT�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽm�ｿｽ[�ｿｽh�ｿｽﾌ番搾ｿｽ
// return : preorder�ｿｽ�ｿｽn�ｿｽﾔ目のノ�ｿｽ[�ｿｽh�ｿｽﾖのポ�ｿｽC�ｿｽ�ｿｽ�ｿｽ^
TreeNode* Tree::getNode(int n)
{
	return root->getNode(n);
}

// �ｿｽﾘのノ�ｿｽ[�ｿｽh�ｿｽ�ｿｽ�ｿｽ狽�ｿｽ�ｿｽ�ｿｽ
void Tree::setNodeNum()
{
	nodeNum = root->countNode();
}

// �ｿｽﾘの深�ｿｽ�ｿｽ�ｿｽｲべゑｿｽ
void Tree::setDepth()
{
	depth = root->maxLevel();
}

// �ｿｽﾘゑｿｽT�ｿｽ�ｿｽ
// data : �ｿｽf�ｿｽ[�ｿｽ^
void Tree::traverse(float* adata)
{
	correctNum += root->traverse(adata);
	processNum++;
}

// �ｿｽN�ｿｽ�ｿｽ�ｿｽX�ｿｽs�ｿｽ�ｿｽﾌ葉�ｿｽm�ｿｽ[�ｿｽh�ｿｽﾉ関ゑｿｽ�ｿｽ髀茨ｿｽ�ｿｽ
void Tree::modify()
{
	correctNum += root->modify();
	root->simplify();
	root->simplify2();
	setNodeNum();
	setDepth();
}

// �ｿｽﾘの適�ｿｽ�ｿｽ�ｿｽx�ｿｽ�ｿｽ�ｿｽv�ｿｽZ
void Tree::calcFit()
{	
	entropy = root->countNode();
	if(entropy < 2) {
		entropy = 100000;
	}
	/*
	if(correctNum > 1)
		entropy = root->calcEnt() / (float)correctNum / (float)log((double)correctNum) * log(2.0);
	else
		entropy = 1;
	*/
	accuracy = ((float)processNum - (float)correctNum) / (float)processNum;
//	fitness = accuracy * 0.8 +  entropy * 0.2;
	//fitness = accuracy * (entropy * 0.2 + 0.8); // 1 - 0.2(entropy - 1) 
//	fitness = accuracy;
}

// �ｿｽ]�ｿｽ�ｿｽ�ｿｽp�ｿｽﾏ撰ｿｽ�ｿｽﾌ擾ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ
void Tree::evalInit2()
{
	fitness = 0.0;
	rankfit = 0.0;
	accuracy = 0.0;
	entropy = 0.0;
	processNum = 0;
	correctNum = 0;
}

// �ｿｽ]�ｿｽ�ｿｽ�ｿｽp�ｿｽﾏ撰ｿｽ�ｿｽﾌ擾ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ
void Tree::evalInit()
{
	evalInit2();
	root->evalInitA();
}

// subRoot : �ｿｽo�ｿｽﾍゑｿｽ�ｿｽ�ｿｽﾘの搾ｿｽ�ｿｽm�ｿｽ[�ｿｽh�ｿｽﾖのポ�ｿｽC�ｿｽ�ｿｽ�ｿｽ^
void Tree::print(TreeNode* subRoot)
{
	short i, j, k;
	float value;

	if(subRoot->child) {
		cout << subRoot->attribute << endl;
		for(i = 0; !subRoot->child[i]; i++);
		while(1) {
			for(j = i + 1; (j < para->ChildNodeNumMax) && !subRoot->child[j]; j++);
			if(j == para->ChildNodeNumMax)
				break;
			value = (float)(i+j) * data->gapAttValue[subRoot->attribute] + data->minAttValue[subRoot->attribute];
			for(k = 0; k < subRoot->level + 1; k++)
				cout << '\t';
			cout << '<' << value << '>';
			print(subRoot->child[i]);
			i = j;
		}
		for(k = 0; k < subRoot->level + 1; k++)
			cout << '\t';
		cout << "<...>";
		print(subRoot->child[i]);
	} else {
		cout << subRoot->classNo << "*\n";
	}
}

void Tree::printDot(TreeNode* subRoot, ofstream& outputfile2) {
	short i, j, k;
	int num = 0; //隕ｪ逡ｪ蜿ｷ
	itr = 0; //譛譁ｰ繝弱�ｼ繝臥分蜿ｷ
	//level縲∬ｦｪ逡ｪ蜿ｷ縲》his逡ｪ蜿ｷ
	child_root.clear();
	root_info.clear();

	child_root.push_back(subRoot);
	root_info.emplace_back();
	root_info[0].push_back(0);
	root_info[0].push_back(-1);
	root_info[0].push_back(0);
	itr++;

	printret(subRoot, num);
	generate_dot(outputfile2);
}
void Tree::generate_dot(ofstream& outputfile2) 
{
	int i;

	outputfile2 << "digraph G {" << endl;
	outputfile2 << "	node [shape=box];" << endl;
	outputfile2 << "	node [style=filled, fillcolor=white];" << endl;
	outputfile2 << endl;

	// 繝ｬ繝吶Ν縺ｮ譛螟ｧ蛟､繧呈ｱゅａ繧�
	double max_level = 0;
	// for(i = 0; i < root_info.size(); i++){

	// }
	for (const auto& info : root_info) {
		max_level = std::max(max_level, info[0]);
	}

	// 繝ｫ繝ｼ繝医ヮ繝ｼ繝会ｼ�0繝ｬ繝吶Ν�ｼ�
	write_label(child_root[0], 0, outputfile2);
	outputfile2 << endl;

	// 蜷�繝ｬ繝吶Ν縺ｧ蜃ｦ逅�
	for (double level = 1; level <= max_level; ++level) {
		for (size_t i = 0; i < child_root.size(); ++i) {
			if (root_info[i][0] == level) {
				write_label(child_root[i], root_info[i][2], outputfile2);
			}
		}
		outputfile2 << endl;
		for (size_t i = 0; i < child_root.size(); ++i) {
			if (root_info[i][0] == level) {
				write_branch(root_info[i], outputfile2);
			}
		}
		outputfile2 << endl;
	}

	outputfile2 << "}" << endl;
}

void Tree::printret(TreeNode* subRoot, int num) 
{
	short i, j, k;
	int num2;
	double value;

	if(subRoot->child) {
		for(i = 0; !subRoot->child[i]; i++);
		while(1) {
			for(j = i + 1; (j < para->ChildNodeNumMax) && !subRoot->child[j]; j++);
			if(j == para->ChildNodeNumMax)
				break;
			value = (double)(i+j) * data->gapAttValue[subRoot->attribute] + data->minAttValue[subRoot->attribute];
			child_root.push_back(subRoot->child[i]);
			root_info.emplace_back();
			root_info[root_info.size()-1].push_back(subRoot->child[i]->level);
			root_info[root_info.size()-1].push_back(num);
			root_info[root_info.size()-1].push_back(itr);
			root_info[root_info.size()-1].push_back(value);
			num2 = itr;
			itr++;
			printret(subRoot->child[i], num2);
			i = j;
		}
		child_root.push_back(subRoot->child[i]);
		root_info.emplace_back();
		root_info[root_info.size()-1].push_back(subRoot->child[i]->level);
		root_info[root_info.size()-1].push_back(num);
		root_info[root_info.size()-1].push_back(itr);
		root_info[root_info.size()-1].push_back(-1000000);
		num2 = itr;
		itr++;
		printret(subRoot->child[i], num2);
	}
}
void Tree::write_label(TreeNode* subRoot, int num, ofstream& outputfile2) 
{
	if(subRoot->child) {
		outputfile2 << "	" << num << " [label=\"" << subRoot->attribute << "\"];"<< endl;
	} else {
		outputfile2 << "	" << num << " [label=\"class=" << subRoot->classNo << "\"];"<< endl;
	}
}
void Tree::write_branch(vector<double> rinfo, ofstream& outputfile2)
{
	if(rinfo[3] != -1000000)
		outputfile2 << "	" << rinfo[1] << " -> " << rinfo[2] << " [label=\"< " << rinfo[3] << "\"];" << endl;
	else
		outputfile2 << "	" << rinfo[1] << " -> " << rinfo[2] << " [label=\"...\"];" << endl;
}