#include "AIDocBoss.h"
#include "CEnemy.h"
#include "CPlayer.h"
#include "Corona_ObjectManager.h"
#include "CGameplayState.h"



AIDocBoss::AIDocBoss()
{
	m_bIsShielded = true;
	m_fShieldTimer = 0.0f;
	m_fSpawnEnemyTimer = 0.0f;
	m_nMaxHealth = 500;

}

int AIDocBoss::Update(float fElapsedTime, CEnemy *thaBoss)
{
	CLevel* pLevel = CGameplayState::GetInstance()->GetLevel();

	thaBoss->SetVelX(0.0f);

	if(pLevel->NextLevelOpen())
	{
		m_bIsShielded = false;
		m_fShieldTimer = 10.0f;
	}


	if(m_bIsShielded)
		thaBoss->SetType(OBJ_SHIELD);
	else
	{
		thaBoss->SetType(OBJ_NONE);
		m_fShieldTimer -= fElapsedTime;
		
		if(m_fShieldTimer <= 0.0f)
		{
			pLevel->ResetSwitches();
			m_bIsShielded = true;
		}
	}

	m_fSpawnEnemyTimer -= fElapsedTime;
	
	if(m_fSpawnEnemyTimer > 0)
		return 0;

	float posx, posy;
	posx = CGameplayState::GetInstance( )->GetPlayerOne( )->GetPosX( );
	posy = CGameplayState::GetInstance( )->GetPlayerOne( )->GetPosY( );

	posx = posx - thaBoss->GetPosX( );
	posy = posy - thaBoss->GetPosY( );

	posx = posx * posx;
	posy = posy * posy;

	posx = posx + posy;

	CPlayer* two = CGameplayState::GetInstance( )->GetPlayerTwo( );
	float dist2( 1000 );

	if( two )
	{
		float posx2 = two->GetPosX() - thaBoss->GetPosX();
		float posy2 = two->GetPosY() - thaBoss->GetPosY();

		posy2 = posy2 * posy2;
		posx2 = posx2 * posx2;

		posx2 += posy2;

		dist2 = sqrt(posx2);
	}

	float dist = sqrt( posx );

	if( dist2 < dist && dist2 < 300 )
		return 2;
	else if( dist < 300 )
		return 1;

	return 0;

}

void AIDocBoss::Attack(CCharacter* pTarget, CCharacter* pShooter)
{

	if(m_fSpawnEnemyTimer <= 0)
	{
		m_fSpawnEnemyTimer = 10.0f * pShooter->GetHealth() / m_nMaxHealth;

		CEnemy* spawnd = new CEnemy(EleType(rand() % 4), pTarget->GetPosX() + pTarget->GetVelX() * 5, pTarget->GetPosY());

		Corona_ObjectManager::GetInstance()->AddObject(spawnd);

		spawnd->Release();
	}
}