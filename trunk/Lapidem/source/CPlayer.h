#pragma once
#include "CCharacter.h"

class CPlayer : public CCharacter
{
	bool m_bIsJumping;
	float m_fJumpTimer;
	float m_fFireTimer;


public:
	CPlayer() {m_fFireTimer = 0.0; m_nHealth = 200; m_bIsJumping = false; currAnimation = 0; m_SpellType = OBJ_FIRE; currDirec = RIGHT; m_nType = OBJ_PLAYER; m_fJumpTimer = 0.0f;}
	void Update(float fElapsedTime);
	void Attack(int nTier);
	void Jump();
	void HandleCollision(CBase *);

};