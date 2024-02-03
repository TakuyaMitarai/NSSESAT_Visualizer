// Exp.h: Exp クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <time.h>
#include "TreePop.h"

class Exp  
{
public:
	Exp(char* dataFname, const char* paraFname);
	virtual ~Exp();
	void initialize(short n);
	void training();
	void test();

	Data* data;
	Para* para;
	SprigPop* sprigPop;
	TreePop* treePop;
	clock_t start, finish;
	float mktime;
	int bestGen;

private:
	void newGeneration();
	void log();

	float bestFit;
};