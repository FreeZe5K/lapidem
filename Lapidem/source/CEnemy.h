#pragma once
#include "CCharacter.h"

class IAIState;


class CEnemy : public CCharacter
{
	IAIState* currState;
	float m_fShotTimer;

public:
~CEnemy();
	CEnemy(EleType, float, float); 
	void Update(float fElapsedTime);
	void HandleCollision(CBase*);


};