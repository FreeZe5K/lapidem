#include "AIStateEarth.h"
#include "CEnemy.h"
#include "CGameplayState.h"

int AIStateEarth::Update( float fElapsedTime, CEnemy* theEnemy )
{
	if( theEnemy->GetPosX( ) <= InitX )
		theEnemy->SetVelX( 100.f );
	else if( theEnemy->GetPosX( ) >= InitX + 200 )
		theEnemy->SetVelX( -100.f );
	
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
		} return 1;
	} return 0;
}

void AIStateEarth::Attack( CCharacter* pTarget, CCharacter* pShooter )
{
	if( pTarget )
	{
		if( pTarget == CGameplayState::GetInstance( )->GetPlayerOne( ) )
			CSpellFactory::GetInstance( )->CreateEarth( pShooter, 1 );
		else CSpellFactory::GetInstance( )->CreateEarth( pShooter, 2 );
	}
}