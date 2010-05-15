#include "AIStateIce.h"
#include "CEnemy.h"
#include "CGameplayState.h"
#include "Corona_EventHandler.h"
#include "CTerrainBase.h"

int AIStateIce::Update( float fElapsedTime, CEnemy* theEnemy )
{
	int nXCentre, nYCentre;
	CLevel* pLevel = CGameplayState::GetInstance()->GetLevel();

	int nDistanceToNext = theEnemy->GetVelX() > 0 ? (theEnemy->GetWidth() + (pLevel->GetTileWidth()>>1)) : -(pLevel->GetTileWidth()>>1);

	nXCentre = (int)(theEnemy->GetPosX() + nDistanceToNext + theEnemy->GetVelX() * fElapsedTime);
	nYCentre = (int)(theEnemy->GetPosY()) + theEnemy->GetHeight() - (pLevel->GetTileHeight()>>1);

	CTerrainBase* pTerrain = (CTerrainBase*)pLevel->GetTile(nXCentre, nYCentre);

	if(pTerrain)
	{
		int Possibility = 0;

		if(pTerrain->GetTypeTerrain() == T_EMPTY)
			Possibility |= 1;

		pTerrain = (CTerrainBase*)pLevel->GetTile(nXCentre, nYCentre - pLevel->GetTileHeight());

		if(pTerrain->GetTypeTerrain() == T_EMPTY)
			Possibility |= 2;

		pTerrain = (CTerrainBase*)pLevel->GetTile(nXCentre, nYCentre - pLevel->GetTileHeight() * 2);

		if(pTerrain->GetTypeTerrain() == T_EMPTY)
			Possibility |= 4;

		if( !Possibility || !(2 & Possibility) )
			theEnemy->SetVelX(-theEnemy->GetVelX());

		else if( !(1 & Possibility) && 4 & Possibility )
		{
			theEnemy->SetPosX(pTerrain->GetPosX() + ((int)theEnemy->GetWidth() >> 1));
			theEnemy->SetPosY(pTerrain->GetPosY());
		}
	}
	else
	{
		theEnemy->SetActive(false);
		Corona_EventHandler::GetInstance()->SendEvent("EnemyDied", (void*)theEnemy);
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
		return 2;
	else if( dist < 300 )
		return 1;

	return 0;
}

void AIStateIce::Attack( CCharacter* pTarget, CCharacter* pShooter )
{
	if( pTarget )
	{
		CSpellFactory::GetInstance()->CreateEnemyIce(pShooter, pTarget);
	}
}
