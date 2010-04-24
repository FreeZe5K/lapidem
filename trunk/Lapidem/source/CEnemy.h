#pragma once
#include "Corona_EventHandler.h"
#include "CCharacter.h"

class IAIState;


class CEnemy : public CCharacter, public CLDevice
{
	IAIState* currState;

public:

	CEnemy(EleType); 
	void Update(float fElapsedTime);


};