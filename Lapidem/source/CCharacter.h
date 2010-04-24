#pragma once
#include "CBase.h"
#include "CLDevice.h"
#include "CSpellFactory.h"

enum DIRECTION {LEFT, RIGHT, UP, DOWN, LEFT_UP, LEFT_DOWN, RIGHT_UP, RIGHT_DOWN};

class CCharacter : public CBase
{
	


	//TODO add Animation *

protected:
	int m_nHealth;
	int currAnimation;
	CSpellFactory* m_pSpells;
	EleType m_SpellType;
	DIRECTION currDirec;

public:
	CCharacter() {m_pSpells = CSpellFactory::GetInstance();}
	DIRECTION GetDirection() {return currDirec;}
	

};