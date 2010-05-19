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

	m_nChoice       = 0;
	m_nCreditScroll = 500;

	m_nImageID[0]   = m_pTM->LoadTexture( "resource/graphics/Lapidem_MainMenuBG.png" );
	m_nImageID[1]   = m_pTM->LoadTexture( "resource/graphics/Lapidem_HowToPlay.png" );

	if( m_nState == 1 )
	{
		// - - - - - - - - - - - - - - - -
		// Load high scores!
		// - - - - - - - - - - - - - - - -
	}

	CGameplayState::GetInstance( )->SetPlayerReachedEnd( false );

	m_HS.Load( "resource/data/scores.bin" );

}

bool CAuxiliaryState::Input( )
{
	if( m_nState == 0 ) // Options
	{
		if( m_pDI->KeyPressed( DIK_UP ) || m_pDI->JoystickDPadPressed( 2 ) )
		{
			m_pWM->Play( CGame::GetInstance( )->GetMenuTick( ) );

			if( --m_nChoice < 0 ) 
				m_nChoice = 2;
		}

		if( m_pDI->KeyPressed( DIK_DOWN ) || m_pDI->JoystickDPadPressed( 3 ) || m_pDI->JoystickGetLStickXNormalized() > 0)
		{
			m_pWM->Play( CGame::GetInstance( )->GetMenuTick( ) );

			if( ++m_nChoice > 2 )
				m_nChoice = 0;
		}

		if( m_pDI->KeyDown( DIK_LEFT ) || m_pDI->JoystickDPadDown( 0 ) || m_pDI->JoystickGetLStickXNormalized() < 0 )
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

		if( m_pDI->KeyDown( DIK_RIGHT ) || m_pDI->JoystickDPadDown( 1 ) )
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

		if( m_pDI->KeyPressed( DIK_RETURN ) || m_pDI->JoystickButtonDown( 1 ) )
			if( m_nChoice == 2 )
				CGame::GetInstance( )->ChangeState( CMenuState::GetInstance( ) );
	}
	else if( m_nState == 1 ) // High Scores
	{
		if( m_pDI->CheckBufferedKeysEx( ) || m_pDI->JoystickCheckBufferedButtons() != -1)
			CGame::GetInstance( )->ChangeState( CMenuState::GetInstance( ) );
	}
	else if( m_nState == 2 ) // How To Play
	{
		if( m_pDI->CheckBufferedKeysEx( ) || m_pDI->JoystickCheckBufferedButtons() != -1)
			CGame::GetInstance( )->ChangeState( CMenuState::GetInstance( ) );
	}
	else if( m_nState == 3 ) // Credits
	{
		if( m_pDI->CheckBufferedKeysEx( ) || m_pDI->JoystickCheckBufferedButtons() != -1)
			CGame::GetInstance( )->ChangeState( CMenuState::GetInstance( ) );
	}

	return true;
}

void CAuxiliaryState::Update( float fET )
{
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

		// - - - - - - - - - - - -
		// Music volume(s)
		// - - - - - - - - - - - -
		m_pWM->SetVolume( CGame::GetInstance( )->GetLevelOneMusic( ), 
			CGame::GetInstance( )->GetMusicVolume( ) ); 
		m_pWM->SetVolume( CGame::GetInstance( )->GetLevelTwoMusic( ), 
			CGame::GetInstance( )->GetMusicVolume( ) );
		m_pWM->SetVolume( CGame::GetInstance( )->GetLevelTwoMusic( ), 
			CGame::GetInstance( )->GetMusicVolume( ) );
		m_pWM->SetVolume( CGame::GetInstance( )->GetMainMenuMusic( ),
			CGame::GetInstance( )->GetMusicVolume( ) );
		m_pWM->SetVolume( CGame::GetInstance( )->GetVictoryMusic( ),
			CGame::GetInstance( )->GetMusicVolume( ) );
		m_pWM->SetVolume( CGame::GetInstance( )->GetLostMusic( ), 
			CGame::GetInstance( )->GetMusicVolume( ) );

		// - - - - - - - - - - - -
		// Sound effect volume(s)
		// - - - - - - - - - - - -
		m_pWM->SetVolume( CGame::GetInstance( )->GetMenuTick( ), 
			CGame::GetInstance( )->GetSoundFXVolume( ) ); 
		m_pWM->SetVolume( CGame::GetInstance( )->GetPlayerHitSound( ),
			CGame::GetInstance( )->GetSoundFXVolume( ) );
		m_pWM->SetVolume( CGame::GetInstance( )->GetShotFiredSound( ),
			CGame::GetInstance( )->GetSoundFXVolume( ) );
		m_pWM->SetVolume( CGame::GetInstance( )->GetTileDestroyedSound( ),
			CGame::GetInstance( )->GetSoundFXVolume( ) );
	}
	else if( m_nState == 1 ) // High Scores
	{
	}
	else if( m_nState == 2 ) // How To Play
	{
	}
	else if( m_nState == 3 ) // Credits
	{
		if( --m_nCreditScroll < -490 )
			CGame::GetInstance( )->ChangeState( CMenuState::GetInstance( ) );
	}
}

void CAuxiliaryState::Render( )
{
	if( m_nState == 2 )
		m_pTM->Draw( m_nImageID[1], 0, 0, 1.0f, 1.0f, 
		NULL, 0.0f, 0.0f, 0.0f, ARGB( 200, 255, 255, 255 ) );
	else
		m_pTM->Draw( m_nImageID[0], 0, 0, 1.0f, 1.0f, 
		NULL, 0.0f, 0.0f, 0.0f, ARGB( 200, 255, 255, 255 ) );

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

		m_HS.Render();

		CGame::GetInstance( )->GetFont( )->Draw( "PRESS ANY KEY TO CONTINUE", 45, 
			450, 0.7f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	}
	else if( m_nState == 2 ) // How To Play
	{
		CGame::GetInstance( )->GetFont( )->Draw( "PRESS ANY KEY TO CONTINUE", 45, 
			450, 0.7f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	}
	else if( m_nState == 3 ) // Credits
	{
		CGame::GetInstance( )->GetFont( )->Draw( "EXECUTIVE PRODUCERS", 30, 
			m_nCreditScroll, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "DUSTIN CLINGMAN", 80, 
			m_nCreditScroll + 40, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "JOHN O'LESKE", 80, 
			m_nCreditScroll + 70, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

		CGame::GetInstance( )->GetFont( )->Draw( "ASSOCIATE PRODUCER", 30, 
			m_nCreditScroll + 120, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "SEAN HATHAWAY", 80, 
			m_nCreditScroll + 160, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

		CGame::GetInstance( )->GetFont( )->Draw( "MUSIC", 30, 
			m_nCreditScroll + 210, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "MICHAL JELONEK", 80, 
			m_nCreditScroll + 250, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

		CGame::GetInstance( )->GetFont( )->Draw( "PROGRAMMERS", 30, 
			m_nCreditScroll + 300, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "PABLO LEON", 80, 
			m_nCreditScroll + 340, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "XIAODONG WENG", 80, 
			m_nCreditScroll + 370, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "MIKE HATTER", 80, 
			m_nCreditScroll + 400, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "JONATHAN CARO", 80, 
			m_nCreditScroll + 430, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( "SAMUEL MATHIS", 80, 
			m_nCreditScroll + 460, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	}
}

bool CAuxiliaryState::LoadConfig( const char* _file )
{
	ifstream ifs( _file, ios_base::in | ios_base::binary );

	if( ifs.is_open( ) )
	{
		int nMusicVol( 0 );
		int nSFXVol( 0 );

		ifs.read( ( char* )&nMusicVol, sizeof( int ) );
		ifs.read( ( char* )&nSFXVol, sizeof( int ) );

		CGame::GetInstance( )->SetMusicVolume( nMusicVol );
		CGame::GetInstance( )->SetSoundFXVolume( nSFXVol );
	} ifs.close( );

	return true;
}

bool CAuxiliaryState::SaveConfig( const char* _file )
{
	ofstream ofs( _file, ios_base::out | ios::trunc | ios_base::binary );

	if( ofs.is_open( ) )
	{ 
		int nMusicVol( CGame::GetInstance( )->GetMusicVolume( ) );
		int nSFXVol( CGame::GetInstance( )->GetSoundFXVolume( ) );

		ofs.write( ( char* )&nMusicVol, sizeof( int ) );
		ofs.write( ( char* )&nSFXVol, sizeof( int ) );
	} ofs.close( );

	return true;
}

void CAuxiliaryState::Exit( )
{
	SaveConfig( "resource/data/Lapidem_Config.dat" );

	if( m_pWM->IsWavePlaying( CGame::GetInstance( )->GetVictoryMusic( ) ) )
	{
		m_pWM->Stop( CGame::GetInstance( )->GetVictoryMusic( ) );
		m_pWM->Reset( CGame::GetInstance( )->GetVictoryMusic( ) );
	}
	else if( m_pWM->IsWavePlaying( CGame::GetInstance( )->GetLostMusic( ) ) )
	{
		m_pWM->Stop( CGame::GetInstance( )->GetLostMusic( ) );
		m_pWM->Reset( CGame::GetInstance( )->GetLostMusic( ) );
	}

	m_pTM->UnloadTexture( m_nImageID[1] );
	m_pTM->UnloadTexture( m_nImageID[0] );
}