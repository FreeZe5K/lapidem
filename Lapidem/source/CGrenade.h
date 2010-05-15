#pragma once

#include "CSpell.h"

#define MAX_SPARK 10

class CEmitter;

class CGrenade : public CSpell
{
	int m_nRadius;
	bool m_bExploded;
	CEmitter* m_pExplosionEmitter;

public:
	CGrenade();
	~CGrenade();

	void Update(float);
	void Render();

	void HandleCollision(float, CBase*);

	void SetExplosionEmitter(CEmitter* pEmitter) { m_pExplosionEmitter = pEmitter; }
	CEmitter* GetExplosionEmitter() { return m_pExplosionEmitter; }

	void CreateExplosion();
	void CreateSparks(float fDirRotation);
};