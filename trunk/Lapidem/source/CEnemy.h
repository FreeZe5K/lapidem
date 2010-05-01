#pragma once
#include "CCharacter.h"

class IAIState;


class CEnemy : public CCharacter
{
	IAIState* currState;
	float m_fShotTimer;
	float m_fWaitTimer;
	int m_nAttackWho;

public:
~CEnemy();
	CEnemy(EleType, float, float); 
	void Update(float fElapsedTime);
	void HandleCollision(CBase*);
	void SetWait(float fWait) { m_fWaitTimer = fWait;}


};