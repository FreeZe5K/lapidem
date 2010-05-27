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
	m_nHowToAlpha   = 215;

	m_nImageID[0]   = m_pTM->LoadTexture( "resource/graphics/Lapidem_MainMenuBG.png" );
	m_nImageID[1]   = m_pTM->LoadTexture( "resource/graphics/Lapidem_HowToPlayOne.png" );
	m_nImageID[2]   = m_pTM->LoadTexture( "resource/graphics/Lapidem_PressAnyKey.png" );

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
				m_nChoice = 5;
		}

		if( m_pDI->KeyPressed( DIK_DOWN ) || m_pDI->JoystickDPadPressed( 3 ) || m_pDI->JoystickGetLStickXNormalized() > 0)
		{
			m_pWM->Play( CGame::GetInstance( )->GetMenuTick( ) );

			if( ++m_nChoice > 5 )
				m_nChoice = 0;
		}

		if( m_pDI->KeyPressed( DIK_LEFT ) )
		{
			if( m_nChoice == 2 )
			{ 
				if( 1 == CGameplayState::GetInstance( )->GetDifficulty( ) )
				{ /* DO NOTHING */ }
				else if( 2 == CGameplayState::GetInstance( )->GetDifficulty( ) )
					CGameplayState::GetInstance( )->SetDifficulty( 1 );
				else if( 3 == CGameplayState::GetInstance( )->GetDifficulty( ) )
					CGameplayState::GetInstance( )->SetDifficulty( 2 );
			}
		}

		if( m_pDI->KeyPressed( DIK_RIGHT ) )
		{
			if( m_nChoice == 2 )
			{ 
				if( 1 == CGameplayState::GetInstance( )->GetDifficulty( ) )
				CGameplayState::GetInstance( )->SetDifficulty( 2 );
				else if( 2 == CGameplayState::GetInstance( )->GetDifficulty( ) )
					CGameplayState::GetInstance( )->SetDifficulty( 3 );
				else if( 3 == CGameplayState::GetInstance( )->GetDifficulty( ) )
				{ /* DO NOTHING */ }
			}
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

			// Brightness
			else if( m_nChoice == 3 )
			{ 
				if( --m_nBrightness < 0 )
					m_nBrightness = 0;
			}

			// Windowed
			else if( m_nChoice == 4 )
			{ CGame::GetInstance( )->TriggerFSTrue( ); }
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

			// Brightness
			else if( m_nChoice == 3 )
			{ 
				if( ++m_nBrightness > 100 )
					m_nBrightness = 100;
			}

			// Windowed
			else if( m_nChoice == 4 )
			{ CGame::GetInstance( )->TriggerFSFalse( ); }
		}

		if( m_pDI->KeyPressed( DIK_RETURN ) || m_pDI->JoystickButtonDown( 1 ) )
			if( m_nChoice == 5 )
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

		CGame::GetInstance( )->SetBrightness( m_nBrightness );
	}
	else if( m_nState == 1 )
	{ /* High Scores */ }
	else if( m_nState == 2 )
	{ 
		if( m_nHowToAlpha < 180 )
			m_nHowToAlpha = 255;
		else --m_nHowToAlpha;
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
	{
		m_pTM->Draw( m_nImageID[1], 0, 0, 1.0f, 1.0f, 
		NULL, 0.0f, 0.0f, 0.0f, ARGB( 200, 255, 255, 255 ) );
		
		m_pTM->Draw( m_nImageID[2], 0, 0, 1.0f, 1.0f, 
		NULL, 0.0f, 0.0f, 0.0f, ARGB( m_nHowToAlpha, 255, 255, 255 ) );
	}
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

			CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULTY", 70, 
				200, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			if( 1 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 100, 
					220, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 220, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "ANTHONY", 375, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else if( 2 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 100, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 220, 
					220, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "ANTHONY", 375, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else if( 3 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 100, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 220, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "ANTHONY", 375, 
					220, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}
			
			CGame::GetInstance( )->GetFont( )->Draw( "DARKNESS", 70, 
				250, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetBrightness( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 270, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "WINDOWED MODE", 70, 
				300, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			
			if( 1 == CGame::GetInstance( )->GetFSValue( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "ON", 100, 
					320, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "OFF", 170, 
					320, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else 
			{
				CGame::GetInstance( )->GetFont( )->Draw( "ON", 100, 
					320, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "OFF", 170, 
					320, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}

			CGame::GetInstance( )->GetFont( )->Draw( "DONE", 70, 370, 
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

			CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULTY", 70, 
				200, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			if( 1 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 100, 
					220, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 220, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "ANTHONY", 375, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else if( 2 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 100, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 220, 
					220, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "ANTHONY", 375, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else if( 3 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 100, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 220, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "ANTHONY", 375, 
					220, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}
			
			CGame::GetInstance( )->GetFont( )->Draw( "DARKNESS", 70, 
				250, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetBrightness( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 270, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "WINDOWED MODE", 70, 
				300, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			
			if( 1 == CGame::GetInstance( )->GetFSValue( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "ON", 100, 
					320, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "OFF", 170, 
					320, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else 
			{
				CGame::GetInstance( )->GetFont( )->Draw( "ON", 100, 
					320, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "OFF", 170, 
					320, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}

			CGame::GetInstance( )->GetFont( )->Draw( "DONE", 70, 370, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 2 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 50, 
				216, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
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
			
			CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULTY", 70, 
				200, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			if( 1 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 100, 
					220, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 220, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "ANTHONY", 375, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else if( 2 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 100, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 220, 
					220, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "ANTHONY", 375, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else if( 3 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 100, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 220, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "ANTHONY", 375, 
					220, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}
			
			CGame::GetInstance( )->GetFont( )->Draw( "DARKNESS", 70, 
				250, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetBrightness( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 270, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "WINDOWED MODE", 70, 
				300, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			
			if( 1 == CGame::GetInstance( )->GetFSValue( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "ON", 100, 
					320, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "OFF", 170, 
					320, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else 
			{
				CGame::GetInstance( )->GetFont( )->Draw( "ON", 100, 
					320, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "OFF", 170, 
					320, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}

			CGame::GetInstance( )->GetFont( )->Draw( "DONE", 70, 370, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 3 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 50, 
				266, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
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
			
			CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULTY", 70, 
				200, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			if( 1 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 100, 
					220, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 220, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "ANTHONY", 375, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else if( 2 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 100, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 220, 
					220, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "ANTHONY", 375, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else if( 3 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 100, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 220, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "ANTHONY", 375, 
					220, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}
			
			CGame::GetInstance( )->GetFont( )->Draw( "DARKNESS", 70, 
				250, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetBrightness( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 270, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "WINDOWED MODE", 70, 
				300, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			if( 1 == CGame::GetInstance( )->GetFSValue( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "ON", 100, 
					320, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "OFF", 170, 
					320, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else 
			{
				CGame::GetInstance( )->GetFont( )->Draw( "ON", 100, 
					320, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "OFF", 170, 
					320, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}

			CGame::GetInstance( )->GetFont( )->Draw( "DONE", 70, 370, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 4 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 50, 
				316, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
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
			
			CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULTY", 70, 
				200, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			if( 1 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 100, 
					220, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 220, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "ANTHONY", 375, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else if( 2 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 100, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 220, 
					220, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "ANTHONY", 375, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else if( 3 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 100, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 220, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "ANTHONY", 375, 
					220, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}
			
			CGame::GetInstance( )->GetFont( )->Draw( "DARKNESS", 70, 
				250, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetBrightness( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 270, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "WINDOWED MODE", 70, 
				300, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			
			if( 1 == CGame::GetInstance( )->GetFSValue( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "ON", 100, 
					320, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "OFF", 170, 
					320, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else 
			{
				CGame::GetInstance( )->GetFont( )->Draw( "ON", 100, 
					320, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "OFF", 170, 
					320, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}

			CGame::GetInstance( )->GetFont( )->Draw( "DONE", 70, 370, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 5 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 50, 
				366, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
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
			
			CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULTY", 70, 
				200, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			if( 1 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 100, 
					220, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 220, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "ANTHONY", 375, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else if( 2 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 100, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 220, 
					220, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "ANTHONY", 375, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else if( 3 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 100, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 220, 
					220, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "ANTHONY", 375, 
					220, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}
			
			CGame::GetInstance( )->GetFont( )->Draw( "DARKNESS", 70, 
				250, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetBrightness( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 270, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "WINDOWED MODE", 70, 
				300, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			
			if( 1 == CGame::GetInstance( )->GetFSValue( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "ON", 100, 
					320, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "OFF", 170, 
					320, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else 
			{
				CGame::GetInstance( )->GetFont( )->Draw( "ON", 100, 
					320, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "OFF", 170, 
					320, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}

			CGame::GetInstance( )->GetFont( )->Draw( "DONE", 80, 370, 
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
		int nDifficulty( 0 );
		int nDarkness( 0 );
		int nWindowed( 0 );

		ifs.read( ( char* )&nMusicVol, sizeof( int ) );
		ifs.read( ( char* )&nSFXVol, sizeof( int ) );
		ifs.read( ( char* )&nDifficulty, sizeof( int ) );
		ifs.read( ( char* )&nDarkness, sizeof( int ) );
		ifs.read( ( char* )&nWindowed, sizeof( int ) );

		CGame::GetInstance( )->SetMusicVolume( nMusicVol );
		CGame::GetInstance( )->SetSoundFXVolume( nSFXVol );
	
		m_nBrightness = nDarkness;

		CGameplayState::GetInstance( )->SetDifficulty( nDifficulty );	
		CGame::GetInstance( )->SetBrightness( m_nBrightness );
		CGame::GetInstance( )->SetFSValue( nWindowed );

		if( 1 == nWindowed )
		{
			m_bFullScreenON = true;
			CGame::GetInstance( )->TriggerFSTrue( );
		}
		else if( 2 == nWindowed )
		{
			m_bFullScreenON = false;
			CGame::GetInstance( )->TriggerFSFalse( );
		}
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
		int nDifficulty( CGameplayState::GetInstance( )->GetDifficulty( ) );
		int nDarkness( CGame::GetInstance( )->GetBrightness( ) );
		int nWindowed( CGame::GetInstance( )->GetFSValue( ) );

		ofs.write( ( char* )&nMusicVol, sizeof( int ) );
		ofs.write( ( char* )&nSFXVol, sizeof( int ) );
		ofs.write( ( char* )&nDifficulty, sizeof( int ) );
		ofs.write( ( char* )&nDarkness, sizeof( int ) );
		ofs.write( ( char* )&nWindowed, sizeof( int ) );
	} ofs.close( );

	return true;
}

bool CAuxiliaryState::LoadSlotInfo( const char* _file )
{
	ifstream ifs( _file, ios_base::in | ios_base::binary );

	if( ifs.is_open( ) )
	{
		ifs.read( ( char* )&_tSlotOne._nNumPlayers, sizeof( int ) );
		ifs.read( ( char* )&_tSlotOne._nCurrentLevel, sizeof( int ) );
		ifs.read( ( char* )&_tSlotOne._nPlayerOneHealth, sizeof( int ) );
		ifs.read( ( char* )&_tSlotOne._nPlayerOneScore, sizeof( int ) );
		ifs.read( ( char* )&_tSlotOne._nPlayerTwoHealth, sizeof( int ) );
		ifs.read( ( char* )&_tSlotOne._nPlayerTwoScore, sizeof( int ) );
		
		ifs.read( ( char* )&_tSlotTwo._nNumPlayers, sizeof( int ) );
		ifs.read( ( char* )&_tSlotTwo._nCurrentLevel, sizeof( int ) );
		ifs.read( ( char* )&_tSlotTwo._nPlayerOneHealth, sizeof( int ) );
		ifs.read( ( char* )&_tSlotTwo._nPlayerOneScore, sizeof( int ) );
		ifs.read( ( char* )&_tSlotTwo._nPlayerTwoHealth, sizeof( int ) );
		ifs.read( ( char* )&_tSlotTwo._nPlayerTwoScore, sizeof( int ) );
		
		ifs.read( ( char* )&_tSlotThree._nNumPlayers, sizeof( int ) );
		ifs.read( ( char* )&_tSlotThree._nCurrentLevel, sizeof( int ) );
		ifs.read( ( char* )&_tSlotThree._nPlayerOneHealth, sizeof( int ) );
		ifs.read( ( char* )&_tSlotThree._nPlayerOneScore, sizeof( int ) );
		ifs.read( ( char* )&_tSlotThree._nPlayerTwoHealth, sizeof( int ) );
		ifs.read( ( char* )&_tSlotThree._nPlayerTwoScore, sizeof( int ) );
	} ifs.close( );

	return true;
}

bool CAuxiliaryState::UpdateSlotInfo( const char* _file, int _slot_to_change )
{
	ifstream ifs( _file, ios_base::in | ios_base::binary );

	if( ifs.is_open( ) )
	{
		ifs.read( ( char* )&_tSlotOne._nNumPlayers, sizeof( int ) );
		ifs.read( ( char* )&_tSlotOne._nCurrentLevel, sizeof( int ) );
		ifs.read( ( char* )&_tSlotOne._nPlayerOneHealth, sizeof( int ) );
		ifs.read( ( char* )&_tSlotOne._nPlayerOneScore, sizeof( int ) );
		ifs.read( ( char* )&_tSlotOne._nPlayerTwoHealth, sizeof( int ) );
		ifs.read( ( char* )&_tSlotOne._nPlayerTwoScore, sizeof( int ) );
		
		ifs.read( ( char* )&_tSlotTwo._nNumPlayers, sizeof( int ) );
		ifs.read( ( char* )&_tSlotTwo._nCurrentLevel, sizeof( int ) );
		ifs.read( ( char* )&_tSlotTwo._nPlayerOneHealth, sizeof( int ) );
		ifs.read( ( char* )&_tSlotTwo._nPlayerOneScore, sizeof( int ) );
		ifs.read( ( char* )&_tSlotTwo._nPlayerTwoHealth, sizeof( int ) );
		ifs.read( ( char* )&_tSlotTwo._nPlayerTwoScore, sizeof( int ) );
		
		ifs.read( ( char* )&_tSlotThree._nNumPlayers, sizeof( int ) );
		ifs.read( ( char* )&_tSlotThree._nCurrentLevel, sizeof( int ) );
		ifs.read( ( char* )&_tSlotThree._nPlayerOneHealth, sizeof( int ) );
		ifs.read( ( char* )&_tSlotThree._nPlayerOneScore, sizeof( int ) );
		ifs.read( ( char* )&_tSlotThree._nPlayerTwoHealth, sizeof( int ) );
		ifs.read( ( char* )&_tSlotThree._nPlayerTwoScore, sizeof( int ) );
	} ifs.close( );

	if( _slot_to_change == 1 )
	{
		if( CGameplayState::GetInstance( )->GetTwoPlayerMode( ) )
			_tSlotOne._nNumPlayers       = 2;
		else _tSlotOne._nNumPlayers      = 1;

		_tSlotOne._nCurrentLevel         = CGameplayState::GetInstance( )->GetCurrentLevel( );
		_tSlotOne._nPlayerOneHealth      = CGameplayState::GetInstance( )->GetPlayerOne( )->GetHealth( );
		_tSlotOne._nPlayerOneScore       = CGameplayState::GetInstance( )->GetPlayerOne( )->GetScore( );

		if( _tSlotOne._nNumPlayers == 2 )
		{
			_tSlotOne._nPlayerTwoHealth  = CGameplayState::GetInstance( )->GetPlayerTwo( )->GetHealth( );
			_tSlotOne._nPlayerTwoScore   = CGameplayState::GetInstance( )->GetPlayerTwo( )->GetScore( );
		}
		else 
		{
			_tSlotOne._nPlayerTwoHealth  = 0;
			_tSlotOne._nPlayerTwoScore   = 0;
		}
	}
	else if( _slot_to_change == 2 )
	{		
		if( CGameplayState::GetInstance( )->GetTwoPlayerMode( ) )
			_tSlotTwo._nNumPlayers       = 2;
		else _tSlotTwo._nNumPlayers      = 1;

		_tSlotTwo._nCurrentLevel         = CGameplayState::GetInstance( )->GetCurrentLevel( );
		_tSlotTwo._nPlayerOneHealth      = CGameplayState::GetInstance( )->GetPlayerOne( )->GetHealth( );
		_tSlotTwo._nPlayerOneScore       = CGameplayState::GetInstance( )->GetPlayerOne( )->GetScore( );

		if( _tSlotTwo._nNumPlayers == 2 )
		{
			_tSlotTwo._nPlayerTwoHealth  = CGameplayState::GetInstance( )->GetPlayerTwo( )->GetHealth( );
			_tSlotTwo._nPlayerTwoScore   = CGameplayState::GetInstance( )->GetPlayerTwo( )->GetScore( );
		}
		else 
		{
			_tSlotTwo._nPlayerTwoHealth  = 0;
			_tSlotTwo._nPlayerTwoScore   = 0;
		}
	}
	else if( _slot_to_change == 3 )
	{
		if( CGameplayState::GetInstance( )->GetTwoPlayerMode( ) )
			_tSlotThree._nNumPlayers     = 2;
		else _tSlotThree._nNumPlayers    = 1;

		_tSlotThree._nCurrentLevel       = CGameplayState::GetInstance( )->GetCurrentLevel( );
		_tSlotThree._nPlayerOneHealth    = CGameplayState::GetInstance( )->GetPlayerOne( )->GetHealth( );
		_tSlotThree._nPlayerOneScore     = CGameplayState::GetInstance( )->GetPlayerOne( )->GetScore( );
		
		if( _tSlotThree._nNumPlayers == 2 )
		{
			_tSlotThree._nPlayerTwoHealth = CGameplayState::GetInstance( )->GetPlayerTwo( )->GetHealth( );
			_tSlotThree._nPlayerTwoScore  = CGameplayState::GetInstance( )->GetPlayerTwo( )->GetScore( );
		}
		else 
		{
			_tSlotThree._nPlayerTwoHealth  = 0;
			_tSlotThree._nPlayerTwoScore   = 0;
		}
	}

	ofstream ofs( _file, ios_base::out | ios::trunc | ios_base::binary );

	if( ofs.is_open( ) )
	{
		ofs.write( ( char* )&_tSlotOne._nNumPlayers, sizeof( int ) );
		ofs.write( ( char* )&_tSlotOne._nCurrentLevel, sizeof( int ) );
		ofs.write( ( char* )&_tSlotOne._nPlayerOneHealth, sizeof( int ) );
		ofs.write( ( char* )&_tSlotOne._nPlayerOneScore, sizeof( int ) );
		ofs.write( ( char* )&_tSlotOne._nPlayerTwoHealth, sizeof( int ) );
		ofs.write( ( char* )&_tSlotOne._nPlayerTwoScore, sizeof( int ) );
		
		ofs.write( ( char* )&_tSlotTwo._nNumPlayers, sizeof( int ) );
		ofs.write( ( char* )&_tSlotTwo._nCurrentLevel, sizeof( int ) );
		ofs.write( ( char* )&_tSlotTwo._nPlayerOneHealth, sizeof( int ) );
		ofs.write( ( char* )&_tSlotTwo._nPlayerOneScore, sizeof( int ) );
		ofs.write( ( char* )&_tSlotTwo._nPlayerTwoHealth, sizeof( int ) );
		ofs.write( ( char* )&_tSlotTwo._nPlayerTwoScore, sizeof( int ) );
		
		ofs.write( ( char* )&_tSlotThree._nNumPlayers, sizeof( int ) );
		ofs.write( ( char* )&_tSlotThree._nCurrentLevel, sizeof( int ) );
		ofs.write( ( char* )&_tSlotThree._nPlayerOneHealth, sizeof( int ) );
		ofs.write( ( char* )&_tSlotThree._nPlayerOneScore, sizeof( int ) );
		ofs.write( ( char* )&_tSlotThree._nPlayerTwoHealth, sizeof( int ) );
		ofs.write( ( char* )&_tSlotThree._nPlayerTwoScore, sizeof( int ) );
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

	m_pTM->UnloadTexture( m_nImageID[2] );
	m_pTM->UnloadTexture( m_nImageID[1] );
	m_pTM->UnloadTexture( m_nImageID[0] );
}