#pragma once
#include "CBase.h"

class CEmitter;


class CPickup : public CBase
{
	int EleType;
	float m_fLifeTimer;

	CEmitter* m_pEmitter;

public:
	CPickup() { m_fLifeTimer = 15.f; }
	~CPickup();
	void SetEleType(int Eletype) {this->EleType = Eletype;}
	void Render();
	void Update(float);
	int	 GetEleType()			  {return EleType;}

	void SetEmitter( CEmitter* pEmitter ) { m_pEmitter = pEmitter; }
	CEmitter* GetEmitter() { return m_pEmitter; }

};