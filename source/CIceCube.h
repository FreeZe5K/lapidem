#pragma once
#include "CSpell.h"

class CIceCube : public CSpell
{
	int m_nRadius;
	bool m_bExploded;
	CEmitter* m_pExplosionEmitter;

public:
	CIceCube();
	~CIceCube();
	
	void Update(float);
	void Render();

	void HandleCollision(float, CBase*);

	void SetExplosionEmitter(CEmitter* pEmitter) { m_pExplosionEmitter = pEmitter; }
	CEmitter* GetExplosionEmitter() { return m_pExplosionEmitter; }

	void CreateExplosion();
	void CreateSparks(float fDirRotation);
};