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

	/*posx = posx + posy*/;

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
	//float dist2( 1000 );

	//if( two )
	//{
	//	float posx2 = two->GetPosX() - theEnemy->GetPosX();
	//	float posy2 = two->GetPosY() - theEnemy->GetPosY();

	//	posy2 = posy2 * posy2;
	//	posx2 = posx2 * posx2;

	//	posx2 += posy2;

	//	dist2 = sqrt(posx2);
	//}

	//float dist = sqrt( posx );

	//if( dist2 < dist && dist2 < 300 )
	//{
	//	if( dist2 < 100 )
	//	{
	//		if( two->GetPosX( ) > theEnemy->GetPosX( ) )
	//			theEnemy->SetVelX( -150.0f );
	//		else if( two->GetPosX( ) > theEnemy->GetPosX( ) )
	//			theEnemy->SetVelX( 150.0f );
	//	}

	//	theEnemy->SetVelX( 0 );
	//	return 2;
	//}
	//else if( dist < 300 )
	//{
	//	CPlayer* one = CGameplayState::GetInstance( )->GetPlayerOne( );
	//	if( dist < 100 )
	//	{
	//		if( one->GetPosX( ) > theEnemy->GetPosX( ) )
	//			theEnemy->SetVelX( -150.0f );
	//		else if( one->GetPosX( ) > theEnemy->GetPosX( ) )
	//			theEnemy->SetVelX( 150.0f );
	//	} 
	//	return 1;
	//}

	return 0;
}

void AIStateIce::Attack( CCharacter* pTarget, CCharacter* pShooter )
{
	if( pTarget )
	{
		CSpellFactory::GetInstance()->CreateEnemyIce(pShooter, pTarget);
	}
}
