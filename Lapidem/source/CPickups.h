#pragma once
#include "CBase.h"


class CPickup : public CBase
{
	int EleType;
	float m_fLifeTimer;
public:
	CPickup() { m_fLifeTimer = 15.f; }
	void SetEleType(int Eletype) {this->EleType = Eletype;}
	void Render();
	void Update(float);
	int	 GetEleType()			  {return EleType;}

};