#pragma once
#include "CCharacter.h"

class CPlayer : public CCharacter
{
	


public:
	CPlayer() {m_nHealth = 200; currAnimation = 0; m_SpellType = OBJ_FIRE; currDirec = RIGHT; m_nType = OBJ_PLAYER;}
	void Update(float fElapsedTime);
	void Attack(int nTier);

};