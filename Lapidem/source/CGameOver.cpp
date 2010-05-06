#include "CGameOver.h"

CGameOver* CGameOver::GetInstance( )
{
	static CGameOver instance;
	return &instance;
}

void CGameOver::Enter( )
{
	m_pD3D        = CSGD_Direct3D::GetInstance( );
	m_pTM         = CSGD_TextureManager::GetInstance( );
	m_pDS         = CSGD_DirectSound::GetInstance( );
	m_pWM         = CSGD_WaveManager::GetInstance( );
	m_pDI         = CSGD_DirectInput::GetInstance( );

	m_nSoundID[0] = m_pWM->LoadWave( "resource/audio/Lapidem_PlayerDied.wav" );
	m_nSoundID[1] = m_pWM->LoadWave( "resource/audio/Lapidem_PlayerWon.wav" );

	m_nImageID[0] = m_pTM->LoadTexture
		( "resource/graphics/Lapidem_PlayerDied.png" );
	m_nImageID[1] = m_pTM->LoadTexture
		( "resource/graphics/Lapidem_PlayerWon.png" );
}

bool CGameOver::Input( )
{
	return true;
}

void CGameOver::Update( float fDT )
{
}

void CGameOver::Render( )
{
	if( 1 == m_nState ) // Player lost
	{
		if( 0 == m_nCondition ) // Timer ran out
		{
		}
		else if( 1 == m_nCondition ) // Health ran out
		{
		}
	}
	else if( 2 == m_nState ) // Player won
	{
		if( 0 == m_nCondition ) // New high score
		{
		}
		else if( 1 == m_nCondition ) // No new high score :(
		{
		}
	}
}

void CGameOver::Exit( )
{
	m_pWM->UnloadWave( m_nSoundID[1] );
	m_pWM->UnloadWave( m_nSoundID[0] );

	m_pTM->UnloadTexture( m_nImageID[1] );
	m_pTM->UnloadTexture( m_nImageID[0] );
}