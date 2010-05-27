//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CAttractState.cpp"
//
//  Author Name :   Mike Hatter
//
//  Purpose     :   
//////////////////////////////////////////////////////////////////////////
#include "CAttractState.h"

CAttractState* CAttractState::GetInstance( )
{
	static CAttractState instance;
	return &instance;
}

void CAttractState::Enter( )
{
	m_pD3D               = CSGD_Direct3D::GetInstance( );
	m_pTM                = CSGD_TextureManager::GetInstance( );
	m_pDS                = CSGD_DirectSound::GetInstance( );
	m_pWM                = CSGD_WaveManager::GetInstance( );
	m_pDI                = CSGD_DirectInput::GetInstance( );

	m_nSSImageID[0]      = m_pTM->LoadTexture( "resource/graphics/attract/one.png" );
	m_nSSImageID[1]      = m_pTM->LoadTexture( "resource/graphics/attract/two.png" );
	m_nSSImageID[2]      = m_pTM->LoadTexture( "resource/graphics/attract/three.png" );
	m_nSSImageID[3]      = m_pTM->LoadTexture( "resource/graphics/attract/four.png" );
	m_nSSImageID[4]      = m_pTM->LoadTexture( "resource/graphics/attract/five.png" );

	m_bAllowedToPlay     = true;
	m_nCurrentPosition   = 0;
}

bool CAttractState::Input( )
{
	if( m_pDI->CheckBufferedKeysEx( ) )
	{
		CMenuState::GetInstance( )->ResetAttractTimer( );
		CGame::GetInstance( )->PopState( );
	}

	if( m_pDI->KeyPressed( DIK_UP ) || 
		m_pDI->KeyPressed( DIK_DOWN ) || 
		m_pDI->KeyPressed( DIK_LEFT ) || 
		m_pDI->KeyPressed( DIK_RIGHT ) )
	{
		CMenuState::GetInstance( )->ResetAttractTimer( );
		CGame::GetInstance( )->PopState( );
	}

	if( m_pDI->JoystickCheckBufferedButtons( ) != -1 )
	{
		CMenuState::GetInstance( )->ResetAttractTimer( );
		CGame::GetInstance( )->PopState( );
	}

	return true;
}

void CAttractState::Update( float fET )
{
	if( m_bAllowedToPlay )
	{
		m_nCurrentPosition  = 0;
		m_bAllowedToPlay    = false;
	}

	if( m_bSecond )
	{
		m_nTimer = m_nTimer + 1;
		m_bSecond = false;
	}

	if( m_nTimer > 3 )
	{
		m_nTimer = 0;

		if( ++m_nCurrentPosition > 4 )
			m_nCurrentPosition = 0;
	}
}

void CAttractState::Render( )
{
	m_pTM->Draw( m_nSSImageID[m_nCurrentPosition], 
		0, 0, 1.0f, 1.0f, NULL, 0.0f, 0.0f, 0.0f, 
		D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
}

void CAttractState::Exit( )
{ 
	for( int _uload = 0; _uload < 5; _uload++ )
		m_pTM->UnloadTexture( m_nSSImageID[_uload] );

	CMenuState::GetInstance( )->ResetAttractTimer( );
}