#pragma once
#include "CBase.h"
#include "CLDevice.h"

class CSpellFactory;

class CCharacter : public CBase
{
	int m_nHealth;
	CSpellFactory* m_pSpells;
	EleType m_SpellType;


	//TODO add Animation *
	//Override Render to use the Aniamtions.
	//Override the Update to update the Animation.

public:

	

};