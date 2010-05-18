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

	m_nImageID[0] = m_pTM->LoadTexture( "resource/graphics/Lapidem_YouDied.png" );
	m_nImageID[1] = m_pTM->LoadTexture( "resource/graphics/Lapidem_YouWon.png" );

	m_pWM->SetVolume( CGame::GetInstance( )->GetLostMusic( ), 
		CGame::GetInstance( )->GetMusicVolume( ) );
	m_pWM->SetVolume( CGame::GetInstance( )->GetVictoryMusic( ), 
		CGame::GetInstance( )->GetSoundFXVolume( ) );

	if( 1 == m_nState )
		m_pWM->Play( CGame::GetInstance( )->GetLostMusic( ) );
	else if( 2 == m_nState )
		m_pWM->Play( CGame::GetInstance( )->GetVictoryMusic( ) );
}

bool CGameOver::Input( )
{
	if( 1 == m_nState )
	{
		if( m_pDI->CheckBufferedKeysEx( ) || m_pDI->JoystickCheckBufferedButtons() != -1)
		{
			// Set the state to high scores
			CAuxiliaryState::GetInstance( )->SetMenuState( 1 );
			CGame::GetInstance( )->ChangeState( CAuxiliaryState::GetInstance( ) );
		}
	}
	else if( 2 == m_nState )
	{
		if( m_pDI->CheckBufferedKeysEx( ) || m_pDI->JoystickCheckBufferedButtons() != -1)
		{
			// Set the state to high scores
			CAuxiliaryState::GetInstance( )->SetMenuState( 1 );
			CGame::GetInstance( )->ChangeState( CAuxiliaryState::GetInstance( ) );
		}
	}

	return true;
}

void CGameOver::Update( float fDT )
{
}

void CGameOver::Render( )
{
	if( 1 == m_nState ) // Player lost
	{
		m_pTM->Draw( m_nImageID[0], 0, 0 );

		if( 0 == m_nCondition ) // Timer ran out
		{
		}
		else if( 1 == m_nCondition ) // Health ran out
		{
		}
	}
	else if( 2 == m_nState ) // Player won
	{
		m_pTM->Draw( m_nImageID[1], 0, 0 );

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
	m_pTM->UnloadTexture( m_nImageID[1] );
	m_pTM->UnloadTexture( m_nImageID[0] );
}