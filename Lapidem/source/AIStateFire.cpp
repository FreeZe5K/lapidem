#include "AIStateFire.h"
#include "CEnemy.h"
#include "CGameplayState.h"
#include "CTerrainBase.h"

int AIStateFire::Update( float fElapsedTime, CEnemy* theEnemy )
{

	fireTimer -= fElapsedTime;

	int nBottomX, nBottomY;
	CLevel* pLevel = CGameplayState::GetInstance()->GetLevel();

	int nDistanceToNext = theEnemy->GetVelX() > 0 ? (theEnemy->GetWidth() + (pLevel->GetTileWidth()>>1)) : -(pLevel->GetTileWidth()>>1);

	nBottomX = (int)(theEnemy->GetPosX() + nDistanceToNext + theEnemy->GetVelX() * fElapsedTime);
	nBottomY = (int)(theEnemy->GetPosY()) + theEnemy->GetHeight() + (pLevel->GetTileHeight()>>1);

	CTerrainBase* pTerrain = (CTerrainBase*)pLevel->GetTile(nBottomX, nBottomY);

	if(pTerrain)
	{
		if(pTerrain->GetTypeTerrain() == T_EMPTY)		// empty tile
			theEnemy->SetVelX(-theEnemy->GetVelX());	// turning back
			
	}

	float posx, posy;
	posx = CGameplayState::GetInstance( )->GetPlayerOne( )->GetPosX( );
	posy = CGameplayState::GetInstance( )->GetPlayerOne( )->GetPosY( );

	posx = posx - theEnemy->GetPosX( );
	posy = posy - theEnemy->GetPosY( );

	posx = posx * posx;
	posy = posy * posy;

	posx = posx + posy;

	CPlayer* two = CGameplayState::GetInstance( )->GetPlayerTwo( );
	float dist2( 1000 );

	if( two )
	{
		float posx2 = two->GetPosX() - theEnemy->GetPosX();
		float posy2 = two->GetPosY() - theEnemy->GetPosY();

		posy2 = posy2 * posy2;
		posx2 = posx2 * posx2;

		posx2 += posy2;

		dist2 = sqrt(posx2);
	}

	float dist = sqrt( posx );

	if( dist2 < dist && dist2 < 300 )
	{
		if( dist2 < 100 )
		{
			if( two->GetPosX( ) > theEnemy->GetPosX( ) )
				theEnemy->SetVelX( -150.0f );
			else if( two->GetPosX( ) > theEnemy->GetPosX( ) )
				theEnemy->SetVelX( 150.0f );
		}

		theEnemy->SetVelX( 0 );
		return 2;
	}
	else if( dist < 300 )
	{
		CPlayer* one = CGameplayState::GetInstance( )->GetPlayerOne( );
		if( dist < 100 )
		{
			if( one->GetPosX( ) > theEnemy->GetPosX( ) )
				theEnemy->SetVelX( -150.0f );
			else if( one->GetPosX( ) > theEnemy->GetPosX( ) )
				theEnemy->SetVelX( 150.0f );
		} 
		return 1;
	}
		pTerrain = (CTerrainBase*)CheckPassable(pLevel, theEnemy, fElapsedTime);
		if(pTerrain && fireTimer <= 0.0f)
		{
			CSpellFactory::GetInstance()->CreateEnemyFire(theEnemy, pTerrain);
			fireTimer = 1.0f;
		}


	return 0;
}

void AIStateFire::Attack( CCharacter* pTarget, CCharacter* pShooter )
{
	if( pTarget && fireTimer <= 0.0f)
	{
		fireTimer = 2.0f;
		CSpellFactory::GetInstance()->CreateEnemyFire(pShooter, pTarget);
	}
}


CBase* AIStateFire::CheckPassable(CLevel* pLevel, CBase* pObject, float fElapsedTime)
{
	int nTilesToCheck = pObject->GetHeight() / pLevel->GetTileHeight();

	int nDistanceToNext = pObject->GetVelX() > 0 ? (pObject->GetWidth() + (pLevel->GetTileWidth()>>1)) : -(pLevel->GetTileWidth()>>1);

	int nPosX = (int)(pObject->GetPosX() + nDistanceToNext + pObject->GetVelX() * fElapsedTime);
	int nPosY = (int)pObject->GetPosY();
	CTerrainBase* pTerrain;

	for(int i = 0; i < nTilesToCheck; ++i)
	{
		pTerrain = (CTerrainBase*)pLevel->GetTile(nPosX, nPosY);

		if(pTerrain && pTerrain->GetType() != OBJ_EVENT)
		{
			if(pTerrain->GetTypeTerrain() != T_EMPTY)
				return pTerrain;
		}
		nPosY += pLevel->GetTileHeight();
	}
	return NULL;
}