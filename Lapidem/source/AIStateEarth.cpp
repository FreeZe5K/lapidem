#include "AIStateEarth.h"
#include "CEnemy.h"
#include "CGameplayState.h"

bool AIStateEarth::Update(float fElapsedTime, CEnemy* theEnemy)
{
	if(theEnemy->GetPosX() <= InitX)
		theEnemy->SetVelX(100.f);
	else if(theEnemy->GetPosX() >= InitX + 200)
		theEnemy->SetVelX(-100.f);
	
	float posx, posy;
	posx = CGameplayState::GetInstance()->GetPlayerOne()->GetPosX();
	posy = CGameplayState::GetInstance()->GetPlayerOne()->GetPosY();

	posx = posx - theEnemy->GetPosX();
	posy = posy - theEnemy->GetPosY();

	posx = posx*posx;
	posy = posy*posy;

	posx += posy;
	if(sqrt(posx) < 300)
	{
		return true;
	}

	return false;
}

void AIStateEarth::Attack(CCharacter* pTarget, CCharacter* pShooter)
{
	if(pTarget)
	{
		CSpellFactory::GetInstance()->CreateEarth(pShooter,1);
	}



}