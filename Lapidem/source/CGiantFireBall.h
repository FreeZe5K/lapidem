#pragma once

#include "CSpell.h"

class CGiantFireBall : public CSpell
{
	float m_fRadiusIncrease;
public:
	CGiantFireBall();
	~CGiantFireBall();

	void Update(float);
	void Render();
};