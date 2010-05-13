#pragma once

#include "IAIState.h"
#include "CEnemy.h"

class AISisBoss : public IAIState
{
	bool m_bHasJumped;
	int m_nCurrentElement;

public:
	AISisBoss();
	~AISisBoss();

	int Update(float, CEnemy*);
	void Attack(CCharacter*, CCharacter*);	

};