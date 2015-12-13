#include "AIStateFire.h"
#include "CEnemy.h"
#include "CGameplayState.h"
#include "CTerrainBase.h"

int AIStateFire::Update( float fElapsedTime, CEnemy* theEnemy )
{
	fireTimer -= fElapsedTime;

	CLevel* pLevel = CGameplayState::GetInstance()->GetLevel();

	CTerrainBase* pTerrain = NULL;

	float posx, posy;
	posx = CGameplayState::GetInstance( )->GetPlayerOne( )->GetPosX( );
	posy = CGameplayState::GetInstance( )->GetPlayerOne( )->GetPosY( );

	posx = posx - theEnemy->GetPosX( );
	posy = posy - theEnemy->GetPosY( );

	CPlayer* two = CGameplayState::GetInstance( )->GetPlayerTwo( );

	if(two)
	{
		float posx2, posy2;
		posx2 = two->GetPosX() - theEnemy->GetPosX();
		posy2 = two->GetPosY() - theEnemy->GetPosY();

		if (posx2 < posx)
			if(posx2 < 200 && posx2 > -200)
				if(posy2 < 150 && posy2 > -150)
					return 2;
	}

	if(posx < 200 && posx > -200)
		if(posy < 150 && posy > -150)
			return 1;

	pTerrain = (CTerrainBase*)CheckPassable(pLevel, theEnemy, fElapsedTime);
	if(pTerrain && fireTimer <= 0.0f)
	{
		if(pTerrain->GetTypeTerrain() == T_BOUNDARY)
			theEnemy->SetVelX( -theEnemy->GetVelX() );
		else
		{
			CSpellFactory::GetInstance()->CreateEnemyFire(theEnemy, pTerrain);
			fireTimer = 1.0f;
		}
	}


	return 0;
}

void AIStateFire::Attack( CCharacter* pTarget, CCharacter* pShooter )
{
	if( pTarget )
	{
		CSpellFactory::GetInstance()->CreateEnemyFire(pShooter, pTarget);
	}
}

CBase* AIStateFire::CheckPassable(CLevel* pLevel, CBase* pObject, float fElapsedTime)
{
	CTerrainBase* pTerrain = NULL;

	if(pObject->GetVelX() < 0)
		pTerrain = ( CTerrainBase* )pLevel->GetTile( int( pObject->GetPosX( ) - 16 ), 
		 int( pObject->GetPosY( ) + pObject->GetHeight( ) * 0.5f + pObject->GetVelY( ) * fElapsedTime ) );
	else
		pTerrain = ( CTerrainBase* )pLevel->GetTile( int( pObject->GetPosX( ) + pObject->GetWidth() + 
		16 + pObject->GetVelX( ) * fElapsedTime ), int( pObject->GetPosY( ) + pObject->GetHeight( ) * 0.5f + 
		pObject->GetVelY( ) * fElapsedTime ) );

	if(pTerrain && pTerrain->GetType() != OBJ_EVENT)
	{
		if(pTerrain->GetTypeTerrain() != T_EMPTY)
			return pTerrain;
	}

	return NULL;
}