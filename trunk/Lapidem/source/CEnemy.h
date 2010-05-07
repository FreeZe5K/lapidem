#pragma once
#include "CCharacter.h"

class IAIState;

class CEnemy : public CCharacter
{
	IAIState*  currState;
	float      m_fShotTimer;
	float      m_fWaitTimer;
	int        m_nAttackWho;
	bool	   m_bKnockBack;
	float	   m_fKnockBack;
	bool	   m_bBurning;
	int		   m_nBurnDamage;
	float      m_fBurnTimer;

public:
	CEnemy( EleType, float, float ); 
	~CEnemy();
	void Update( float );
	void HandleCollision( CBase* );
	void SetWait( float _f )          { m_fWaitTimer = _f;}
	void SetKnockBack(float fSpeed) { m_fKnockBack = fSpeed; m_bKnockBack = true;} 
};