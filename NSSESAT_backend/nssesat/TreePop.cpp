// TreePop.cpp: TreePop �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////
#include "TreePop.h"
#include <iostream>
#include <algorithm>

Para* TreePop::para = NULL;
Data* TreePop::data = NULL;

// �R���X�g���N�^�i�V�K�j
TreePop::TreePop()
{
	short i, j;

	genNum = 1;
	bestTree = NULL;
	pop = new Tree* [para->TreePopNum];	
	bestacc.resize(SPLIT_NUM + 1);

	// �e�̂������_���ɐ���
	for(i = 0; i < para->TreePopNum; i++)
		pop[i] = new Tree();
	for(i = 0; i < para->TreePopNum; i++) {
		for(j = 0; j < data->trainDataNum; j++)
			pop[i]->traverse(data->trainData[j]);
		pop[i]->modify();
		pop[i]->calcFit();
	}
	//�p���[�g�t�����g�ƍ��G�����̎Z�o
	Paretofit();

	for(i = 0; i < para->TreePopNum; i++) {
		pop[i]->root->sprigEval(pop[i]->fitness);
	}
}

// �f�X�g���N�^
TreePop::~TreePop()
{
	short i;

	for(i = 0; i < para->TreePopNum; i++)
		delete pop[i];
	delete [] pop;
}

void TreePop::Paretofit()
{
	short i, j, k;
	int cnt;
	int current_cnt = 0;
	int flag;
	int rank = 1;
	int num;
	vector<int> now_s;
	vector<int> next_s;
	vector<int> rank_s;

	for(i = 0; i < para->TreePopNum; i++) {
		next_s.push_back(i); 
	}
	//�p���[�g�t�����g&���G�����L�q
	while(current_cnt < para->TreePopNum) {
		cnt = current_cnt;
		now_s = next_s;
		next_s.clear();
		rank_s.clear();
		for(j = 0; j < para->TreePopNum - cnt; j++) {
			flag = 1;
			for(k = 0; k < para->TreePopNum - cnt; k++) {
				if(pop[now_s[j]]->accuracy >= pop[now_s[k]]->accuracy && pop[now_s[j]]->entropy >= pop[now_s[k]]->entropy && now_s[j] != now_s[k]) {
					if(pop[now_s[j]]->accuracy != pop[now_s[k]]->accuracy || pop[now_s[j]]->entropy != pop[now_s[k]]->entropy) {
						num = now_s[j];
						flag = 0;
					}
				}
				if(flag == 0) {
					next_s.push_back(num);
					break;
				}
			}
			if(flag == 1) {
				pop[now_s[j]]->rankfit = rank;
				rank_s.push_back(now_s[j]);
				current_cnt++;
			}
		}
		//���G�x�̎Z�o
		CrowdedDistance(rank, rank_s);
		rank++;
	}
	//#pragma omp parallel for
	for(i = 0; i < para->TreePopNum; i++) {
		//cout << pop[i]->accuracy << " "  << pop[i]->entropy << " " << pop[i]->fitness << endl;
		pop[i]->fitness = pop[i]->rankfit + 1 / (pop[i]->fitness * 10 + 1);
	}
	//sort(0, para->TreePopNum - 1);
	std::sort(pop, pop+para->TreePopNum, [](Tree* a, Tree*b){
		return a->fitness < b->fitness;
	});

}

//���G�x
void TreePop::CrowdedDistance(int i, vector<int> rank_s)
{
	int j;
	
	for(j = 0; j < rank_s.size(); j++) {
		pop[rank_s[j]]->fitness = 0;
	}
	if(rank_s.size() >= 2) {
		std::sort(rank_s.begin(), rank_s.end(), [this](const int& a, const int& b) {
			return this->pop[a]->accuracy < this->pop[b]->accuracy;
		});
		pop[rank_s[0]]->fitness = 10000;
		pop[rank_s[rank_s.size()-1]]->fitness = 10000;
		for(j = 2; j < rank_s.size() - 1; j++) {
			if(pop[rank_s[rank_s.size()-1]]->accuracy - pop[rank_s[0]]->accuracy != 0) {
				pop[rank_s[j]]->fitness += (pop[rank_s[j+1]]->accuracy - pop[rank_s[j-1]]->accuracy) / (pop[rank_s[rank_s.size()-1]]->accuracy - pop[rank_s[0]]->accuracy);
			}
		}
		std::sort(rank_s.begin(), rank_s.end(), [this](const int& a, const int& b) {
			return this->pop[a]->entropy < this->pop[b]->entropy;
		});
		pop[rank_s[0]]->fitness = 10000;
		pop[rank_s[rank_s.size()-1]]->fitness = 10000;
		for(j = 2; j < rank_s.size() - 1; j++) {
			if(pop[rank_s[rank_s.size()-1]]->entropy - pop[rank_s[0]]->entropy != 0) {
				pop[rank_s[j]]->fitness += (pop[rank_s[j+1]]->entropy - pop[rank_s[j-1]]->entropy) / (pop[rank_s[rank_s.size()-1]]->entropy - pop[rank_s[0]]->entropy);
			}
		}
	}
}

// �����㐶��
void TreePop::newGeneration()
{
	int i;
	int TOURNAMENT_SIZE = 10;

	genNum++;
	// #pragma omp parallel for
	vector<vector<Tree*>> nodepop(SPLIT_NUM+1);
	for(i = 0; i < para->TreePopNum; i++) {
		int iter = int(pop[i]->entropy/NODE_SPLIT);
		if(SPLIT_NUM <= iter)iter = SPLIT_NUM; 
		nodepop[iter-1].push_back(pop[i]);
	}
	for(int k = 0; k < SPLIT_NUM+1; k++) {
		for(i = PRESERVE_NUM; i < nodepop[k].size(); i++) {
			int j, a, b, index1, index2, tmp1, tmp2, fit1;
			int an, bn;
			do {
				tmp1 = randomInt(para->TreePopNum - 1);
				fit1 = pop[tmp1]->fitness;
				for(j = 0; j < TOURNAMENT_SIZE; j++) {
					a = randomInt(para->TreePopNum - 1);
					if(fit1 > pop[a]->fitness) {
						fit1 = pop[a]->fitness;
						tmp1 = a;
					}
				}
			} while(pop[tmp1] == nodepop[k][i]);
			do {
				tmp2 = randomInt(para->TreePopNum - 1);
				fit1 = pop[tmp2]->fitness;
				for(j = 0; j < TOURNAMENT_SIZE; j++) {
					a = randomInt(para->TreePopNum - 1);
					if(fit1 > pop[a]->fitness) {
						fit1 = pop[a]->fitness;
						tmp2 = a;
					}
				}
			} while (tmp1 == tmp2 || pop[tmp2] == nodepop[k][i]);
			index1 = randomInt(pop[tmp1]->nodeNum - 1) + 1;
			index2 = randomInt(pop[tmp2]->nodeNum - 1) + 1;
			nodepop[k][i]->newGeneration(pop[tmp1], pop[tmp2], index1, index2);
			nodepop[k][i]->evalInit();
			for(j = 0; j < data->trainDataNum; j++)
				nodepop[k][i]->traverse(data->trainData[j]);
			nodepop[k][i]->modify();
			nodepop[k][i]->calcFit();
			// nodepop[k][i]->evalInit();
			// for(j = 0; j < data->validationDataNum; j++)
			// 	nodepop[k][i]->traverse(data->validationData[j]);
			// nodepop[k][i]->calcFit();
			if(int(nodepop[k][i]->entropy) < SPLIT_NUM  && int(nodepop[k][i]->entropy) > 0) {
				if(bestacc[int(nodepop[k][i]->entropy)].empty()) {
					bestacc[int(nodepop[k][i]->entropy)].push_back(new Tree(nodepop[k][i]));
				} else if(bestacc[int(nodepop[k][i]->entropy)][bestacc[int(nodepop[k][i]->entropy)].size()-1]->accuracy > nodepop[k][i]->accuracy) {
					bestacc[int(nodepop[k][i]->entropy)].push_back(new Tree(nodepop[k][i]));
				}
			}
			// nodepop[k][i]->evalInit();
			// for(j = 0; j < data->trainDataNum; j++)
			// 	nodepop[k][i]->traverse(data->trainData[j]);
			// nodepop[k][i]->calcFit();
		}
	}
	nodepop.clear();
	//�p���[�g�t�����g�ƍ��G�����̎Z�o
	Paretofit();
	for(i = 0; i < para->TreePopNum; i++) {
		pop[i]->root->sprigEval(pop[i]->fitness);
	}
}
