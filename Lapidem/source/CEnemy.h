#pragma once
#include "CCharacter.h"

class IAIState;

struct Frame;
class CFlock;
class CEnemy : public CCharacter
{
private:
	IAIState*  currState;
	float      m_fShotTimer;
	float      m_fWaitTimer;
	float	   m_fKnockBack;
	float      m_fBurnTimer;
	float      m_fFreezeTimer;
	float	   m_fChangeAnimationTimer;
	float      m_fFrozenSpeed;
	int        m_nAttackWho;
	int		   m_nBurnDamage;
	bool	   m_bKnockBack;
	bool	   m_bBurning;
	bool       m_bIsFrozen;
	int		   m_nAnimation;
	float	   m_fScale;
	Frame*     LastFrame;
	float      m_fLastPosition;

public:


	CEnemy( EleType, float, float, int boss = 0, CFlock* Flock = NULL ); 
	~CEnemy();
	CAnimation* GetAnimation(void) {return animation;}
	void Render(void);
	RECT GetCollisionRect(float fElapsedTime);
	void Update( float );
	void HandleCollision(float, CBase* );
	void SetWait( float _f )          { m_fWaitTimer = _f;}
	IAIState* GetState(void) {return currState;}
	void SetKnockBack(float fSpeed) { m_fKnockBack = fSpeed; m_bKnockBack = true;} 
};