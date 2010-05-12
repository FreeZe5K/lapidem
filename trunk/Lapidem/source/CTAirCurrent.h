#pragma once
#include "CTerrainBase.h"
#include "CCharacter.h"

class CTAirCurrent : public CTerrainBase
{
	DIRECTION shotDirect;
	DIRECTION old_Direct;
public:
	void SetDirection(int DirectionInDegrees);
	void Update(float fElapsedTime);
	void HandleCollision(CBase*);
	DIRECTION GetDirection() {return shotDirect;}

};