#include "CEnemy.h"
#include "IAIState.h"
#include "AIStateEarth.h"

CEnemy::CEnemy(EleType ElementToBe)
{
	m_nType = OBJ_ENEMY;
	switch(ElementToBe)
	{
	case OBJ_EARTH:
		currState = new AIStateEarth();
		break;
	case OBJ_FIRE:
		break;
	case OBJ_ICE:
		break;
	case OBJ_WIND:
		break;
	}
}

void CEnemy::Update(float fElapsedTime)
{
	//TODO replace NULL with a target.
	//Possible Solution: Replace the Update call's
	//Return type with a CCharacter *, which would be the target...

	if(currState->Update(fElapsedTime, this))
		currState->Attack(NULL);

}