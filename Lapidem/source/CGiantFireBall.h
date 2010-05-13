#pragma once

#include "CSpell.h"

class CGiantFireBall : public CSpell
{
	float m_fRadiusIncrease;
public:
	CGiantFireBall();
	~CGiantFireBall();

	void SetRadiusIncrease(float fIncreaseAmt) { m_fRadiusIncrease = fIncreaseAmt; }
	float GetRadiusIncrease() { return m_fRadiusIncrease; }

	void Update(float);
	void Render();
};