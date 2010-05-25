#include "CLoading.h"

CLoading* CLoading::GetInstance( )
{
	static CLoading instance;
	return &instance;
}

void CLoading::Enter( )
{	
	m_pTM                = CSGD_TextureManager::GetInstance( );

	m_nProposedProgress  = 0;
	m_nCurrentProgress   = 0;

	m_nBackgroundID      = m_pTM->LoadTexture( "resource/graphics/Lapidem_LoadingScreen.png" );
	m_nProgressID        = m_pTM->LoadTexture( "resource/graphics/Lapidem_ProgressBar.png" );
}

bool CLoading::Input( )
{ return true; }

void CLoading::Update( float fDT )
{
	if( m_nCurrentProgress > 639 )
	{
		m_nProposedProgress  = 0;
		m_nCurrentProgress   = 0;
		CGame::GetInstance( )->PopState( );
	}
	else
	{
		if( m_nCurrentProgress < m_nProposedProgress )
			m_nCurrentProgress = m_nCurrentProgress + 5;
	}
}

void CLoading::Render( )
{
	m_pTM->Draw( m_nBackgroundID, 0, 0 );
	m_pTM->Draw( m_nProgressID, m_nCurrentProgress - 640, 0, 1, 1, 0, 0, 0, 0, D3DCOLOR_XRGB(50, 75, 255) );
}

void CLoading::Exit( )
{
	m_pTM->UnloadTexture( m_nProgressID );
	m_pTM->UnloadTexture( m_nBackgroundID );
}

void CLoading::SetProgress( int _prop )
{ m_nProposedProgress = _prop; }