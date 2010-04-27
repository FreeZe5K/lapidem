#include "AIStateEarth.h"
#include "CEnemy.h"

bool AIStateEarth::Update(float fElapsedTime, CEnemy* theEnemy)
{
	if(theEnemy->GetPosX() <= InitX)
		theEnemy->SetVelX(100.f);
	else if(theEnemy->GetPosX() >= InitX + 200)
		theEnemy->SetVelX(-100.f);

	return false;
}

void AIStateEarth::Attack(CCharacter* pTarget)
{




}