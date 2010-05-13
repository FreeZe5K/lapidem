#pragma once

#include "IAIState.h"

class CLevel;
class CBase;

class AIStateIce : public IAIState
{
private:


public:
	AIStateIce()  {}
	int Update( float, CEnemy* );
	void Attack( CCharacter*, CCharacter*);
};
