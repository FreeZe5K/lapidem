//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CIce.h"
//
//  Author Name :   Jonathan Caro
//
//  Purpose     :	Handles the Ice objects created by the player and
//					enemies. Handles all 3 tiers of spells.
//////////////////////////////////////////////////////////////////////////
#include "CIce.h"
#include "Wrappers/CSGD_TextureManager.h"
#include "CCamera.h"
#include "CSpellFactory.h"
#include "CPlayer.h"

CIce::CIce( ) : CSpell( )
{
	SetImage(CSGD_TextureManager::GetInstance()->LoadTexture
		( "resource/graphics/ice.bmp", D3DCOLOR_XRGB( 0, 0, 0 ) ) );
}

CIce::~CIce( )
{
	if( GetImage( ) >= 0 )
		CSGD_TextureManager::GetInstance( )->UnloadTexture( GetImage( ) );
}

void CIce::Update( float fElapsedTime )
{
	SetLifespan( GetLifespan( ) - fElapsedTime );

	if( GetLifespan( ) < 0)
	{ /* SetActive(false); */ }

	switch( GetTier( ) )
	{
	case 1:
		{
			UpdateTier1( fElapsedTime );
			break;
		}
	case 2:
		{
			UpdateTier2( fElapsedTime );
			break;
		}
	case 3:
		{
			UpdateTier3( fElapsedTime );
			break;
		}
	}
}

void CIce::UpdateTier1( float fElapsedTime )
{
	SetPosX( GetPosX( ) + GetVelX( ) * fElapsedTime );
	SetPosY( GetPosY( ) + GetVelY( ) * fElapsedTime );	
}

void CIce::UpdateTier2( float fElapsedTime )
{ /* NOTHING HERE YET */ }

void CIce::UpdateTier3( float fElapsedTime)
{ /* NOTHING HERE YET */ }

void CIce::Render( )
{
	switch( GetTier( ) )
	{
	case 1:
		{
			RenderTier1( );
			break;
		}
	case 2:
		{
			RenderTier2( );
			break;
		}
	case 3:
		{
			RenderTier3( );
			break;
		}
	}
}

void CIce::RenderTier1( )
{
	if( GetImage( ) != -1 )
	{
		CSGD_TextureManager::GetInstance( )->Draw( GetImage( ), 
			int( GetPosX( ) - CCamera::GetCamera( )->GetXOffset( ) ), 
			int( GetPosY( )- CCamera::GetCamera( )->GetYOffset( ) ) );
	}
}

void CIce::RenderTier2( )
{ /* NOTHING HERE YET */ }

void CIce::RenderTier3( )
{ /* NOTHING HERE YET */ }

void CIce::HandleCollision( CBase* pObject )
{
	if( 1 == GetTier() )
	{
		if( pObject->GetType( ) == OBJ_TERRA )
		{
			SetActive( false );
			CSpellFactory::GetInstance()->AddIceXP(2);
		}
		else if( pObject->GetType( ) == OBJ_PLAYER && !PlayerShot( ) )
		{
			SetActive( false );
			( ( CPlayer* )pObject )->TakeDamage( GetDamage( ) );
		}
		else if( pObject->GetType( ) == OBJ_ENEMY )
		{
			SetActive( false );
			CSpellFactory::GetInstance()->AddIceXP(2);
		}
		else if( pObject->GetType( ) == OBJ_SPELL && ( ( CSpell* )pObject )->GetElement( ) != GetElement( ) )
			SetActive( false );
	}
	else if( 2 == GetTier( ) )
	{ /* do stuff... like destroy... EVERYTHING */ }
	else if( 3 == GetTier( ) )
	{ /* holy crap everything go splode */ }
}