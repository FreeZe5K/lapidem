#pragma once
#include "IAIState.h"

class AIStateEarth : public IAIState
{


public:
	bool Update(float, CEnemy*) {return true;}
	void Attack(CCharacter*) {};


};