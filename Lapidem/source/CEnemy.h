#pragma once
#include "CCharacter.h"

class IAIState;


class CEnemy : public CCharacter
{
	IAIState* currState;

public:

	CEnemy(EleType); 
	void Update(float fElapsedTime);


};