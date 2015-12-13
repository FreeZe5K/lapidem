#pragma once
#include "IAIState.h"

class CLevel;
class CBase;

class AIStateFire : public IAIState
{
private:
	float fireTimer;

public:
	AIStateFire()  {fireTimer = 2.0f;}
	int Update( float, CEnemy* );
	void Attack( CCharacter*, CCharacter*);
	CBase* CheckPassable(CLevel* pLevel, CBase* pObject, float fElapsedTime);
};
