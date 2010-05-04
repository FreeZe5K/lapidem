//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CFire.h"
//
//  Author Name :   Jonathan Caro
//
//  Purpose     :	Handles the fire objects created by the player and
//					enemies. Handles all 3 tiers of spells.
//////////////////////////////////////////////////////////////////////////
#include "CFire.h"
#include "Wrappers/CSGD_TextureManager.h"
#include "CTerrainBase.h"
#include "CCamera.h"
#include "CPlayer.h"

CFire::CFire( ) : CSpell( )
{
	// - - - - - - - - - - - - - - -
	// TODO ::
	//      Delete this and replace
	//      with particle effects.
	// - - - - - - - - - - - - - - -

	SetType( OBJ_SPELL );
	SetElement( OBJ_FIRE );
}

CFire::~CFire()
{
	if( GetImage( ) >= 0 )
		CSGD_TextureManager::GetInstance( )->UnloadTexture( GetImage( ) );
}

void CFire::Update( float fElapsedTime )
{
	SetLifespan( GetLifespan( ) - fElapsedTime );

	if( GetLifespan( ) < 0 )
		SetActive( false );

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

void CFire::UpdateTier1( float fElapsedTime )
{  CSpell::UpdateTier1( fElapsedTime ); }

void CFire::UpdateTier2( float fElapsedTime )
{ /* NOTHING HERE YET */ }

void CFire::UpdateTier3( float fElapsedTime )
{ /* NOTHING HERE YET */ }

void CFire::Render( )
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

void CFire::RenderTier1( )
{
	if( GetImage( ) != -1 )
		CSGD_TextureManager::GetInstance( )->Draw( GetImage( ), 
		int( GetPosX( ) - CCamera::GetCamera( )->GetXOffset( ) ), 
		int( GetPosY( )- CCamera::GetCamera( )->GetYOffset( ) ) );
}

void CFire::RenderTier2( )
{ /* NOTHING HERE YET */ }

void CFire::RenderTier3( )
{ /* NOTHING HERE YET */ }

void CFire::HandleCollision( CBase* pObject )
{
	if( 1 == GetTier( ) )
	{
		if( pObject->GetType( ) == OBJ_PLAYER && !PlayerShot( ) )
		{
			SetActive( false );
			( ( CPlayer* )pObject )->TakeDamage( GetDamage( ) );
		}
		else if( pObject->GetType( ) == OBJ_ENEMY )
			SetActive( false );
		else if( pObject->GetType( ) == OBJ_SPELL && ( ( CSpell* )pObject )->GetElement( ) != GetElement( ) )
			SetActive( false );
		else if( pObject->GetType( ) == OBJ_TERRA )
			SetActive( false );
	}
	else if( 2 == GetTier( ) )
	{ /* do stuff... like destroy... EVERYTHING */ }
	else if( 3 == GetTier( ) )
	{ /* holy crap everything go splode */ }
}