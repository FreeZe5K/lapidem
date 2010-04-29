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
	EleType GetEleType() {return m_SpellType;}
	void SetEleType(EleType newType) {if(newType > OBJ_EARTH) newType = OBJ_FIRE; else if(newType < OBJ_FIRE) newType = OBJ_EARTH; m_SpellType = newType;}
	void Update(float fElapsedTime);
	void SetHealth(int nHealth) {m_nHealth = nHealth;}
	int GetHealth(void) {return m_nHealth;}
	void TakeDamage(int nDamage) {m_nHealth -= nDamage;}

};