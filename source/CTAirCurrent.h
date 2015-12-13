#pragma once
#include "CTerrainBase.h"
#include "CCharacter.h"
#include "CEmitter.h"

class CTAirCurrent : public CTerrainBase
{
	DIRECTION shotDirect;
	DIRECTION old_Direct;

	int m_nDirecionDeg;
	float m_fDelay;
	CEmitter*	m_emitter;

public:
	CTAirCurrent();
	~CTAirCurrent();
	void SetDirection(int DirectionInDegrees);
	void Update(float fElapsedTime);
	void HandleCollision(CBase*);
	DIRECTION GetDirection() {return shotDirect;}

	void SetTransform(float _f) { m_fTransformTimer = _f; }

};