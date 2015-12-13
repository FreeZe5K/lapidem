#pragma once
#include "CFire.h"

class CGiantFireBall : public CFire
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