#pragma once
#include "CCharacter.h"

class CPlayer : public CCharacter
{
	bool m_bIsJumping;
	float m_fJumpTimer;

	float m_fFireTimer;


public:
	CPlayer();
	void Update(float fElapsedTime);
	void Attack(int nTier);
	void Jump();
	void HandleCollision(CBase *);

};