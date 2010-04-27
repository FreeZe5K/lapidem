//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CMenuState.cpp"
//
//  Author Name :   Mike Hatter
//////////////////////////////////////////////////////////////////////////
#include "CMenuState.h"

CMenuState* CMenuState::GetInstance( )
{
	static CMenuState instance;
	return &instance;
}

void CMenuState::Enter( )
{
	m_pD3D          = CSGD_Direct3D::GetInstance();
	m_pTM           = CSGD_TextureManager::GetInstance();
	m_pDS           = CSGD_DirectSound::GetInstance();
	m_pWM           = CSGD_WaveManager::GetInstance();
	m_pDI           = CSGD_DirectInput::GetInstance();

	m_nChoice       = 0;
	m_nAttractTimer = 0;

	m_nImageID      = m_pTM->LoadTexture( "resource/graphics/Lapidem_MainMenuBG.png" );

	m_nSoundID[0]   = m_pWM->LoadWave( "resource/audio/Lapidem_MainMenuMusic.wav" );
	m_nSoundID[1]   = m_pWM->LoadWave( "resource/audio/Lapidem_MainMenuTick.wav" );

	// - - - - - - - - - - - - - - - -
	// Don't touch these. 
	// They'll be implemented soon.
	// - - - - - - - - - - - - - - - -
	m_nScrollSpeedOne     = -128;
	m_nScrollSpeedTwo     = -128;
	m_nScrollSpeedThree   = -128;
	m_nScrollSpeedFour    = -128;
	// - - - - - - - - - - - - - - - -

	m_pWM->SetVolume( m_nSoundID[0], CGame::GetInstance( )->GetMusicVolume( ) ); 
	m_pWM->SetVolume( m_nSoundID[1], CGame::GetInstance( )->GetSoundFXVolume( ) ); 
	m_pWM->Play( m_nSoundID[0], DSBPLAY_LOOPING );
}

bool CMenuState::Input( )
{
	if( m_pDI->KeyPressed( DIK_UP ) )
	{
		m_nAttractTimer = 0;
		m_pWM->Play( m_nSoundID[1] );

		if( --m_nChoice < 0 ) 
			m_nChoice = 5;
	}

	if( m_pDI->KeyPressed( DIK_DOWN ) )
	{
		m_nAttractTimer = 0;
		m_pWM->Play( m_nSoundID[1] );

		if( ++m_nChoice > 5 )
			m_nChoice = 0;
	}

	if( m_pDI->KeyPressed( DIK_RETURN ) )
	{
		if( m_nChoice == 0 )      // Play
			CGame::GetInstance( )->ChangeState( CGameplayState::GetInstance( ) );

		else if( m_nChoice == 1 ) // Options
		{ 
			CAuxiliaryState::GetInstance( )->SetMenuState( 0 );
			CGame::GetInstance( )->ChangeState( CAuxiliaryState::GetInstance( ) );
		}

		else if( m_nChoice == 2 ) // High Scores
		{ 
			CAuxiliaryState::GetInstance( )->SetMenuState( 1 );
			CGame::GetInstance( )->ChangeState( CAuxiliaryState::GetInstance( ) );
		}

		else if( m_nChoice == 3 ) // How To Play
		{ 
			CAuxiliaryState::GetInstance( )->SetMenuState( 2 );
			CGame::GetInstance( )->ChangeState( CAuxiliaryState::GetInstance( ) );
		}

		else if( m_nChoice == 4 ) // Credits
		{ 
			CAuxiliaryState::GetInstance( )->SetMenuState( 3 );
			CGame::GetInstance( )->ChangeState( CAuxiliaryState::GetInstance( ) );
		} 

		else if( m_nChoice == 5 ) // Exit
			PostQuitMessage( 0 );
	}

	return true;
}

void CMenuState::Update( float fET )
{
	// TODO:: If 10 seconds have passed without input . . .
	if( ++m_nAttractTimer >= 10 )
	{ 
		// TODO:: Set timer to zero and switch to attract mode
	}
}

void CMenuState::Render( )
{
	m_pTM->Draw( m_nImageID, 0, 0, 1.0f, 1.0f, 
		NULL, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB( 200, 255, 255, 255 ) );

	if( m_nChoice == 0 )
	{		
		CGame::GetInstance( )->GetFont( )->Draw( ">", 50, 96, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "PLAY", 80, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "OPTIONS", 70, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "HIGH SCORES", 70, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "HOW TO PLAY", 70, 160, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "CREDITS", 70, 180, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "EXIT", 70, 200, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	}
	else if( m_nChoice == 1 )
	{
		CGame::GetInstance( )->GetFont( )->Draw( ">", 50, 116, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "PLAY", 70, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "OPTIONS", 80, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "HIGH SCORES", 70, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "HOW TO PLAY", 70, 160, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "CREDITS", 70, 180, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "EXIT", 70, 200, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	}
	else if( m_nChoice == 2 )
	{
		CGame::GetInstance( )->GetFont( )->Draw( ">", 50, 136, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "PLAY", 70, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "OPTIONS", 70, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "HIGH SCORES", 80, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "HOW TO PLAY", 70, 160, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "CREDITS", 70, 180, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "EXIT", 70, 200, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	}
	else if( m_nChoice == 3 )
	{
		CGame::GetInstance( )->GetFont( )->Draw( ">", 50, 156, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "PLAY", 70, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "OPTIONS", 70, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "HIGH SCORES", 70, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "HOW TO PLAY", 80, 160, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "CREDITS", 70, 180, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "EXIT", 70, 200, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	}
	else if( m_nChoice == 4 )
	{
		CGame::GetInstance( )->GetFont( )->Draw( ">", 50, 176, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "PLAY", 70, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "OPTIONS", 70, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "HIGH SCORES", 70, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "HOW TO PLAY", 70, 160, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "CREDITS", 80, 180, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "EXIT", 70, 200, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	}
	else if( m_nChoice == 5 )
	{
		CGame::GetInstance( )->GetFont( )->Draw( ">", 50, 196, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "PLAY", 70, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "OPTIONS", 70, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "HIGH SCORES", 70, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "HOW TO PLAY", 70, 160, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "CREDITS", 70, 180, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "EXIT", 80, 200, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	}
	// - - - - - - - - - - - - - - - -
}

void CMenuState::Exit( )
{
	m_pWM->UnloadWave( m_nSoundID[1] );
	m_pWM->UnloadWave( m_nSoundID[0] );
	m_pTM->UnloadTexture( m_nImageID );
}