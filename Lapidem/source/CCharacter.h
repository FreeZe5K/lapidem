#pragma once
#include "CBase.h"
#include "CLDevice.h"
#include "CSpellFactory.h"
class CAnimation;

enum DIRECTION {LEFT, RIGHT, UP, DOWN, LEFT_UP, LEFT_DOWN, RIGHT_UP, RIGHT_DOWN};

class CCharacter : public CBase
{
protected:
	CAnimation* animation;
	bool IsRotated;
	int m_nHealth;
	int currAnimation;
	CSpellFactory* m_pSpells;
	EleType m_SpellType;
	DIRECTION currDirec;

public:
	CCharacter();
	DIRECTION GetDirection() {return currDirec;}
	void Render(void);
	void SetEleType(EleType newType) {m_SpellType = newType;}
	void Update(float fElapsedTime);
};