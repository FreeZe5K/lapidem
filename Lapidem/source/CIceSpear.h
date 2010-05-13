#pragma once

#include "CSpell.h"


class CIceSpear : public CSpell
{
	int m_nRadius;
	bool m_bExploded;
	CEmitter* m_pExplosionEmitter;

public:
	CIceSpear();
	~CIceSpear();

	void HandleCollision(CBase*);
	
	void Update(float);
	void Render();

	void SetExplosionEmitter(CEmitter* pEmitter) { m_pExplosionEmitter = pEmitter; }
	CEmitter* GetExplosionEmitter() { return m_pExplosionEmitter; }
};