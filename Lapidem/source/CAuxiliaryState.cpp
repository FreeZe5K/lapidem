//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CAuxiliaryState.cpp"
//
//  Author Name :   Mike Hatter
//////////////////////////////////////////////////////////////////////////
#include "CAuxiliaryState.h"

CAuxiliaryState* CAuxiliaryState::GetInstance( )
{
	static CAuxiliaryState instance;
	return &instance;
}

void CAuxiliaryState::Enter( )
{
	m_pD3D          = CSGD_Direct3D::GetInstance();
	m_pTM           = CSGD_TextureManager::GetInstance();
	m_pDS           = CSGD_DirectSound::GetInstance();
	m_pWM           = CSGD_WaveManager::GetInstance();
	m_pDI           = CSGD_DirectInput::GetInstance();

	m_nAttractTimer = 0;
	m_nCreditScroll = 500;


	pFile           = NULL;

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

	if( m_nState == 1 )
	{		
		char _NameBuffer[25];
		int  _ScoreBuffer;
		fopen_s( &pFile, "resources/data/Lapidem_HighscoreTable.bin", "rb" );

		if( pFile == NULL )
			return;

		for( int i = 0; i < 10; i++ )
		{
			fread( &_NameBuffer, sizeof( char ), 25, pFile );
			m_szPlayerNames[i] = _NameBuffer;

			fread( &_ScoreBuffer, sizeof( _ScoreBuffer ), 1, pFile );
			m_nPlayerScores[i] = _ScoreBuffer;
		}

		fclose( pFile );
	}

	m_pWM->Play( m_nSoundID[0], DSBPLAY_LOOPING );
	m_pWM->SetVolume( m_nSoundID[0], CGame::GetInstance( )->GetMusicVolume( ) ); 
	m_pWM->SetVolume( m_nSoundID[1], CGame::GetInstance( )->GetSoundFXVolume( ) ); 
}

bool CAuxiliaryState::Input( )
{
	if( m_nState == 0 )
	{
		if( m_pDI->KeyPressed( DIK_UP ) )
		{
			m_nAttractTimer = 0;
			m_pWM->Play( m_nSoundID[1] );

			if( --m_nChoice < 0 ) 
				m_nChoice = 2;
		}

		if( m_pDI->KeyPressed( DIK_DOWN ) )
		{
			m_nAttractTimer = 0;
			m_pWM->Play( m_nSoundID[1] );

			if( ++m_nChoice > 2 )
				m_nChoice = 0;
		}

		if( m_pDI->KeyDown( DIK_LEFT ) )
		{
			// Music Volume
			if( m_nChoice == 0 )
				CGame::GetInstance( )->SetMusicVolume( 
				CGame::GetInstance( )->GetMusicVolume( ) - 1 );

			// Sound Effect Volume
			else if( m_nChoice == 1 )
				CGame::GetInstance( )->SetSoundFXVolume( 
				CGame::GetInstance( )->GetSoundFXVolume( ) - 1 );
		}

		if( m_pDI->KeyDown( DIK_RIGHT ) )
		{
			// Music Volume
			if( m_nChoice == 0 )
				CGame::GetInstance( )->SetMusicVolume( 
				CGame::GetInstance( )->GetMusicVolume( ) + 1 );

			// Sound Effect Volume
			else if( m_nChoice == 1 )
				CGame::GetInstance( )->SetSoundFXVolume( 
				CGame::GetInstance( )->GetSoundFXVolume( ) + 1 );
		}

		if( m_pDI->KeyPressed( DIK_RETURN ) )
			if( m_nChoice == 2 )
				CGame::GetInstance( )->ChangeState( CMenuState::GetInstance( ) );
	}
	else if( m_nState == 1 ) // High Scores
	{
		// - - - - - - - - - - - - - - - - - -
		// 1. Parse the "Lapidem_HighscoreTable.bin"
		// 2. Display the names and scores to the screen
		// 3. Take a break.
		// - - - - - - - - - - - - - - - - - -
		if( m_pDI->CheckBufferedKeysEx( ) )
			CGame::GetInstance( )->ChangeState( CMenuState::GetInstance( ) );
	}
	else if( m_nState == 2 ) // How To Play
	{
	}
	else if( m_nState == 3 ) // Credits
	{
		if( m_pDI->CheckBufferedKeysEx( ) )
			CGame::GetInstance( )->ChangeState( CMenuState::GetInstance( ) );
	}

	return true;
}

void CAuxiliaryState::Update( )
{
	// - - - - - - - - - - - - - - - - - -
	// If 10 seconds have passed 
	// without input . . .
	// - - - - - - - - - - - - - - - - - -
	if( ++m_nAttractTimer >= 10 )
	{ 
		// - - - - - - - - - - - - - - - - - -
		// Set timer to zero 
		// and switch to attract mode.
		// - - - - - - - - - - - - - - - - - -
	}

	if( m_nState == 0 ) // Options
	{
		if( CGame::GetInstance( )->GetMusicVolume( ) > 100 )
			CGame::GetInstance( )->SetMusicVolume( 100 );
		else if( CGame::GetInstance( )->GetMusicVolume( ) < 0 )
			CGame::GetInstance( )->SetMusicVolume( 0 );

		if( CGame::GetInstance( )->GetSoundFXVolume( ) > 100 )
			CGame::GetInstance( )->SetSoundFXVolume( 100 );
		else if( CGame::GetInstance( )->GetSoundFXVolume( ) < 0 )
			CGame::GetInstance( )->SetSoundFXVolume( 0 );

		m_pWM->SetVolume( m_nSoundID[0], CGame::GetInstance( )->GetMusicVolume( ) ); 
		m_pWM->SetVolume( m_nSoundID[1], CGame::GetInstance( )->GetSoundFXVolume( ) ); 
	}
	else if( m_nState == 1 ) // High Scores
	{
	}
	else if( m_nState == 2 ) // How To Play
	{
	}
	else if( m_nState == 3 ) // Credits
	{
		if( --m_nCreditScroll < -150 )
			CGame::GetInstance( )->ChangeState( CMenuState::GetInstance( ) );
	}
}

void CAuxiliaryState::Render( )
{
	m_pTM->Draw( m_nImageID, 0, 0, 1.0f, 1.0f, 
		NULL, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB( 200, 255, 255, 255 ) );

	// - - - - - - - - - - - - - - - - - -
	// Fix the bitmap fonts to be kerned.
	// - - - - - - - - - - - - - - - - - -
	if( m_nState == 0 ) // Options
	{
		char cBuffer[64];

		if( m_nChoice == 0 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 50, 
				116, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "MUSIC VOL", 70, 
				100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetMusicVolume( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 120, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "SOUND FX VOL", 70, 
				150, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetSoundFXVolume( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 170, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "DONE", 70, 210, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 1 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 50, 
				166, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "MUSIC VOL", 70, 
				100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetMusicVolume( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 120, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "SOUND FX VOL", 70, 
				150, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetSoundFXVolume( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 170, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "DONE", 70, 210, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 2 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 50, 
				206, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "MUSIC VOL", 70, 
				100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetMusicVolume( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 120, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "SOUND FX VOL", 70, 
				150, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetSoundFXVolume( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 170, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "DONE", 80, 210, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
	}
	else if( m_nState == 1 ) // High Scores
	{
		int _ScoreHeight( 100 );
		char _ScoreBuffer[16];

		for( int i = 0; i < 10; i++ )
		{
			CGame::GetInstance( )->GetFont( )->Draw( m_szPlayerNames[i].c_str(), 30, 
				_ScoreHeight, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			sprintf_s( _ScoreBuffer, "%i", m_nPlayerScores[i] );
			CGame::GetInstance( )->GetFont( )->Draw( _ScoreBuffer, 150, 
				_ScoreHeight, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			_ScoreHeight = _ScoreHeight + 30;
		}
		CGame::GetInstance( )->GetFont( )->Draw( "PRESS ANY KEY TO CONTINUE...", 50, 
			450, 0.7f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	}
	else if( m_nState == 2 ) // How To Play
	{

	}
	else if( m_nState == 3 ) // Credits
	{
		CGame::GetInstance( )->GetFont( )->Draw( "PROGRAMMERS", 30, 
			m_nCreditScroll, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "MIKE HATTER", 80, 
			m_nCreditScroll + 40, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "PABLO LEON", 80, 
			m_nCreditScroll + 70, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "JONATHAN CARO", 80, 
			m_nCreditScroll + 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "SAMUEL MATHIS", 80, 
			m_nCreditScroll + 130, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	}
}

void CAuxiliaryState::Exit( )
{
	m_nChoice = 0;
	m_pWM->UnloadWave( m_nSoundID[1] );
	m_pWM->UnloadWave( m_nSoundID[0] );
	m_pTM->UnloadTexture( m_nImageID );
}