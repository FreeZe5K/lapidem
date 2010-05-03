#pragma once
#include "CCharacter.h"

class IAIState;

class CEnemy : public CCharacter
{
	IAIState*  currState;
	float      m_fShotTimer;
	float      m_fWaitTimer;
	int        m_nAttackWho;

public:
	CEnemy( EleType, float, float ); 
	~CEnemy();
	void Update( float );
	void HandleCollision( CBase* );
	void SetWait( float _f )          { m_fWaitTimer = _f;     }
};