#pragma once
#include "IAIState.h"

class AIStateEarth : public IAIState
{
	int InitX;
	int InitY;
public:
	bool Update(float, CEnemy*);
	void Attack(CCharacter*, CCharacter*);
	void SetInitPos(int x, int y) {InitX = x, InitY = y;}
};