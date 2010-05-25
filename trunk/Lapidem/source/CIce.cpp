//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CIce.h"
//
//  Author Name :   Jonathan Caro
//
//  Purpose     :	Handles the Ice objects created by the player and
//					enemies. Handles all 3 tiers of spells.
//////////////////////////////////////////////////////////////////////////
#include "CIce.h"
#include "stdheaders.h"

#include "CEmitter.h"
#include "CPlayer.h"

CIce::CIce( ) : CSpell( )
{
}

CIce::~CIce( )
{
	if( GetImage( ) >= 0 )
		CSGD_TextureManager::GetInstance( )->UnloadTexture( GetImage( ) );
	CParticleManager::GetInstance()->RemoveEmitter(m_pEmitter);
}

void CIce::Update( float fElapsedTime )
{
	SetLifespan( GetLifespan( ) - fElapsedTime );

	if( GetLifespan( ) < 0 )
		SetActive( false );

	if(m_pEmitter)
	{
		m_pEmitter->SetPosX(GetPosX());
		m_pEmitter->SetPosY(GetPosY());
	}

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
{ UpdateTier1( fElapsedTime ); }

void CIce::UpdateTier3( float fElapsedTime)
{ CSpell::UpdateTier3( fElapsedTime ); }

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
{ 
	if( GetImage( ) != -1 )
	{
		CSGD_TextureManager::GetInstance( )->Draw( GetImage( ), 
			int( GetPosX( ) - CCamera::GetCamera( )->GetXOffset( ) ), 
			int( GetPosY( )- CCamera::GetCamera( )->GetYOffset( ) ), 0.3f, 0.3f );
	}
}

void CIce::RenderTier3( )
{ /* NOTHING HERE YET */ }

void CIce::HandleCollision(float fElapsedTime, CBase* pObject )
{
	if(pObject->GetType() < 0)
		return;

	CSpell::HandleCollision(fElapsedTime, pObject);

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
			CSpellFactory::GetInstance( )->AddIceXP( 2 );
		}
		else if( pObject->GetType( ) == OBJ_SPELL && ( ( CSpell* )pObject )->GetElement( ) != GetElement( ) 
			&& ( ( CSpell* )pObject )->GetTier( ) != 3 )
			SetActive( false );
	}
	else if( 2 == GetTier( ) )
	{
		/*if( pObject->GetType( ) == OBJ_PLAYER ) 
			pObject->MoveOutOf( this, fElapsedTime ); */
	}
	else if( 3 == GetTier( ) )
	{ /* holy crap everything go splode */ }
}