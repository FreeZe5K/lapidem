//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CFire.h"
//
//  Author Name :   Jonathan Caro
//
//  Purpose     :	Handles the fire objects created by the player and
//					enemies. Handles all 3 tiers of spells.
//////////////////////////////////////////////////////////////////////////
#include "CFire.h"
#include "stdheaders.h"

#include "CTerrainBase.h"
#include "CEmitter.h"
#include "CPlayer.h"

CFire::CFire( ) : CSpell( )
{
	SetType( OBJ_SPELL );
	SetElement( OBJ_FIRE );

	m_pEmitter = NULL;
	
}

CFire::~CFire()
{
	if( GetImage( ) >= 0 )
		CSGD_TextureManager::GetInstance( )->UnloadTexture( GetImage( ) );
		CParticleManager::GetInstance()->RemoveEmitter(m_pEmitter);
}

void CFire::Update( float fElapsedTime )
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

void CFire::UpdateTier1( float fElapsedTime )
{  CSpell::UpdateTier1( fElapsedTime ); }

void CFire::UpdateTier2( float fElapsedTime )
{  CSpell::UpdateTier1( fElapsedTime ); }

void CFire::UpdateTier3( float fElapsedTime )
{
	if(GetPosX() - 10 < CCamera::GetCamera()->GetXOffset() || GetPosX() + GetWidth() + 10 > CCamera::GetCamera()->GetWidth())
		SetVelX(-GetVelX());

	if(GetPosY() + GetHeight() > CCamera::GetCamera()->GetHeight())
		this->SetActive(false);

	CSpell::UpdateTier1(fElapsedTime);
}

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

void CFire::HandleCollision(float fElapsedTime,  CBase* pObject )
{
	if(pObject->GetType() < 0)
		return;

	CSpell::HandleCollision(fElapsedTime, pObject);

	if( 1 == GetTier( ) )
	{
		if( pObject->GetType( ) == OBJ_PLAYER && !PlayerShot( ) )
		{
			SetActive( false );
		}
		else if( pObject->GetType( ) == OBJ_ENEMY && PlayerShot( ))
		{
			SetActive( false );
			CSpellFactory::GetInstance()->AddFireXP(2);
		}
		else if( pObject->GetType( ) == OBJ_SPELL && ( ( CSpell* )pObject )->GetElement( ) != GetElement( ) 
			&& ((CSpell*)pObject)->GetTier() !=3)
			SetActive( false );
		else if( pObject->GetType( ) == OBJ_TERRA )
		{
			SetActive( false );
			CSpellFactory::GetInstance()->AddFireXP(1);
		}

	}
	else if( 2 == GetTier( ) )
	{ 
	
		if( pObject->GetType() == OBJ_ENEMY )
			CSpellFactory::GetInstance()->AddFireXP(5);
		else if( pObject->GetType( ) == OBJ_TERRA )
			CBase::MoveOutOf(pObject, fElapsedTime);
	}
	else if( 3 == GetTier( ) )
	{ /* holy crap everything go splode */ }
}