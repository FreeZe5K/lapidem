//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CSpell.cpp"
//
//  Author Name :   Jonathan Caro
//
//  Purpose     :   Provides the functionality for the spells to inherit 
//////////////////////////////////////////////////////////////////////////
#include "Wrappers/CSGD_TextureManager.h"
#include "Corona_ObjectManager.h" 
#include "CEmitter.h"
#include "CSpell.h"

CSpell::CSpell( ) : CBase( )
{
	SetType(OBJ_SPELL);
	m_pEmitter = NULL;
}

CSpell::~CSpell()
{ /* NOTHING HERE YET */ }

void CSpell::UpdateTier1( float fElapsedTime )
{	
	if( m_pEmitter && IsActive( ) )
		m_pEmitter->UpdateParticlePos( int( GetPosX( ) ), int( GetPosY( ) ) );

	SetPosX( GetPosX( ) + GetVelX( ) * fElapsedTime );
	SetPosY( GetPosY( ) + GetVelY( ) * fElapsedTime );
}
void CSpell::UpdateTier2( float fElapsedTime )
{
	SetPosX( GetPosX( ) + GetVelX( ) * fElapsedTime );
	SetPosY( GetPosY( ) + GetVelY( ) * fElapsedTime );
}

void CSpell::UpdateTier3( float fElapsedTime )
{
	SetPosX( GetPosX( ) + GetVelX( ) * fElapsedTime );
	SetPosY( GetPosY( ) + GetVelY( ) * fElapsedTime );
}
void CSpell::RenderTier1( )
{
	if( GetImage( ) != -1 )
		CSGD_TextureManager::GetInstance( )->Draw
		( GetImage( ), int( GetPosX( ) ), int( GetPosY( ) ) );
}
void CSpell::RenderTier2( )
{
	if( GetImage( ) != -1 )
		CSGD_TextureManager::GetInstance( )->Draw
		( GetImage( ), int( GetPosX( ) ), int( GetPosY( ) ) );
}
void CSpell::RenderTier3( )
{
	if( GetImage( ) != -1 )
		CSGD_TextureManager::GetInstance( )->Draw
		( GetImage( ), int( GetPosX( ) ), int( GetPosY( ) ) );
}
void CSpell::Update( float fElapsedTime )
{
	SetLifespan( GetLifespan( ) - fElapsedTime);

	if( GetLifespan( ) <= 0 )
		SetActive( false );

	switch( m_nTier )
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

void CSpell::HandleCollision( CBase* pObject )
{
	////////////////////////////////////////////////////////////////
	//TODO: Carona_ObjectManager->GetInstance()->RemoveObject(this);
	////////////////////////////////////////////////////////////////
}

void CSpell::Render( )
{
	switch( m_nTier )
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