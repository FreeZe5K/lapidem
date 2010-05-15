#pragma once

#include "CSpell.h"

#define MAX_SPARK 10

class CEmitter;

class CGrenade : public CSpell
{
	int m_nRadius;
	bool m_bExploded;

public:
	CGrenade();
	~CGrenade();

	void Update(float);
	void Render();

	void HandleCollision(float, CBase*);


	void CreateExplosion();
	void CreateSparks(float fDirRotation);
};