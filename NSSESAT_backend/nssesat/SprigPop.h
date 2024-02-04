// SprigPop.h: SprigPop クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Sprig.h"
class Sprig;

class SprigPop
{
public:
	static Para* para;
	static Data* data;

	SprigPop();
	virtual ~SprigPop();
	void newGeneration();
	void sort(short lb, short ub);
	
	Sprig** pop;
};