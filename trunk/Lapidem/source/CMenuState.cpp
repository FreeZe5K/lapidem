//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CMenuState.cpp"
//
//  Author Name :   Mike Hatter
//
//  Purpose     :   
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

	m_nImageID[0]   = m_pTM->LoadTexture( "resource/graphics/Lapidem_MainMenuBG.png" );
	m_nImageID[1]   = m_pTM->LoadTexture( "resource/graphics/SGD_MenuCursor.png" );

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

	m_pWM->Play( m_nSoundID[0], DSBPLAY_LOOPING );
	m_pWM->SetVolume( m_nSoundID[0], CGame::GetInstance( )->GetMusicVolume( ) ); 
	m_pWM->SetVolume( m_nSoundID[1], CGame::GetInstance( )->GetSoundFXVolume( ) ); 
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
		{ } 

		else if( m_nChoice == 1 ) // Options
		{ }

		else if( m_nChoice == 2 ) // High Scores
		{ }

		else if( m_nChoice == 3 ) // How To Play
		{ }

		else if( m_nChoice == 4 ) // Credits
		{ } 

		else if( m_nChoice == 5 ) // Exit
			PostQuitMessage( 0 );
	}

	return true;
}

void CMenuState::Update( )
{
	// TODO:: If one second has passed . . .
	++m_nAttractTimer;

	// TODO:: If 10 seconds have passed without input . . .
	if( m_nAttractTimer >= 10 )
	{ 
		// TODO:: Set timer to zero and switch to attract mode
	}
}

void CMenuState::Render( )
{
	m_pTM->Draw( m_nImageID[0], 0, 0, 1.0f, 1.0f, 
		NULL, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB( 200, 255, 255, 255 ) );

	// - - - - - - - - - - - - - - - -
	// Place holder until a bitmap
	// font class is implemented.
	// - - - - - - - - - - - - - - - -
	m_pD3D->DrawText( "OPTION 1", 70, 80 );
	m_pD3D->DrawText( "OPTION 2", 70, 100 );
	m_pD3D->DrawText( "OPTION 3", 70, 120 );
	m_pD3D->DrawText( "OPTION 4", 70, 140 );
	m_pD3D->DrawText( "OPTION 5", 70, 160 );
	m_pD3D->DrawText( "OPTION 5", 70, 180 );
	// - - - - - - - - - - - - - - - -

	if( m_nChoice == 0 )
		m_pTM->Draw( m_nImageID[1], 50, 80 );
	else if( m_nChoice == 1 )
		m_pTM->Draw( m_nImageID[1], 50, 100 );
	else if( m_nChoice == 2 )
		m_pTM->Draw( m_nImageID[1], 50, 120 );
	else if( m_nChoice == 3 )
		m_pTM->Draw( m_nImageID[1], 50, 140 );
	else if( m_nChoice == 4 )
		m_pTM->Draw( m_nImageID[1], 50, 160 );
	else if( m_nChoice == 5 )
		m_pTM->Draw( m_nImageID[1], 50, 180 );
}

void CMenuState::Exit( )
{
	m_pWM->UnloadWave( m_nSoundID[1] );
	m_pWM->UnloadWave( m_nSoundID[0] );
	m_pTM->UnloadTexture( m_nImageID[1] );
	m_pTM->UnloadTexture( m_nImageID[0] );
}