//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CPauseMenuState.cpp"
//
//  Author Name :   Mike Hatter
//
//  Purpose     :   
//////////////////////////////////////////////////////////////////////////
#include "CPauseMenuState.h"
#include "CLevel.h"
#include "CGameplayState.h"
#include "CTerrainBase.h"

CPauseMenuState* CPauseMenuState::GetInstance( )
{
	static CPauseMenuState instance;
	return &instance;
}

void CPauseMenuState::Enter( )
{
	m_pD3D          = CSGD_Direct3D::GetInstance();
	m_pTM           = CSGD_TextureManager::GetInstance();
	m_pDS           = CSGD_DirectSound::GetInstance();
	m_pWM           = CSGD_WaveManager::GetInstance();
	m_pDI           = CSGD_DirectInput::GetInstance();

	m_nChoice       = 0;
	m_nImageID      = m_pTM->LoadTexture( "resource/graphics/Lapidem_PauseMenu.png");
}

bool CPauseMenuState::Input( )
{
	if( 0 == m_nState )
	{
		if( m_pDI->KeyPressed( DIK_UP ) || m_pDI->JoystickDPadPressed(2) )
		{
			if( --m_nChoice < 0 ) 
				m_nChoice = 4;
		}

		if( m_pDI->KeyPressed( DIK_DOWN ) || m_pDI->JoystickDPadPressed(3) )
		{
			if( ++m_nChoice > 4 )
				m_nChoice = 0;
		}

		if( m_pDI->KeyPressed( DIK_P ) || m_pDI->JoystickButtonPressed(9))
		{
			CGame::GetInstance( )->PopState( );
			CGame::GetInstance( )->SetPaused( false );
		}

		if( m_pDI->KeyPressed( DIK_RETURN ) || m_pDI->JoystickButtonPressed(1))
		{
			if( m_nChoice == 0 ) // Resume Game
			{
				CGame::GetInstance( )->SetPaused( false );
				CGame::GetInstance( )->PopState( );
			}
			else if( m_nChoice == 1 ) // Options menu
			{
				m_nChoice  = 0;
				m_nState   = 1;
			}
			else if( m_nChoice == 2 ) // Save and quit
			{
				m_nChoice  = 0;
				m_nState   = 2;
			}
			else if( m_nChoice == 3 ) // Save and quit
			{
				m_nChoice  = 0;
				m_nState   = 3;
			}
			else // Main menu (or other...)
			{
				CGame::GetInstance( )->SetPaused( false );
				CGame::GetInstance( )->ChangeState( CMenuState::GetInstance( ) );
			}
		}
	}
	else if( 1 == m_nState ) // Options
	{
		if(m_pDI->KeyPressed( DIK_UP ) || m_pDI->JoystickDPadPressed( 2 ) )
		{
			if( --m_nChoice < 0 ) 
				m_nChoice = 5;
		}

		if(m_pDI->KeyPressed( DIK_DOWN ) || m_pDI->JoystickDPadPressed( 3 ) )
		{
			if( ++m_nChoice > 5 )
				m_nChoice = 0;
		}
		
		if( m_pDI->KeyPressed( DIK_LEFT ) )
		{
			if( 2 == m_nChoice )
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
			if( 2 == m_nChoice )
			{ 
				if( 1 == CGameplayState::GetInstance( )->GetDifficulty( ) )
				CGameplayState::GetInstance( )->SetDifficulty( 2 );
				else if( 2 == CGameplayState::GetInstance( )->GetDifficulty( ) )
					CGameplayState::GetInstance( )->SetDifficulty( 3 );
				else if( 3 == CGameplayState::GetInstance( )->GetDifficulty( ) )
				{ /* DO NOTHING */ }
			}
		}

		if(m_pDI->KeyDown( DIK_LEFT ) || m_pDI->JoystickDPadDown( 0 ) )
		{
			// Music
			if( m_nChoice == 0 )
			{
				if( CGame::GetInstance( )->GetMusicVolume( ) - 1 < 0 )
					CGame::GetInstance( )->SetMusicVolume( 0 );
				else CGame::GetInstance( )->SetMusicVolume( 
				CGame::GetInstance( )->GetMusicVolume( ) - 1 );
			}

			// Sound FX
			else if( m_nChoice == 1 )
			{
				if( CGame::GetInstance( )->GetSoundFXVolume( ) - 1 < 0 )
					CGame::GetInstance( )->SetSoundFXVolume( 0 );
				else CGame::GetInstance( )->SetSoundFXVolume( 
				CGame::GetInstance( )->GetSoundFXVolume( ) - 1 );
			}

			// Brightness
			else if( m_nChoice == 3 )
			{
				if( CAuxiliaryState::GetInstance( )->GetBrightness( ) - 1 < 0 )
					CAuxiliaryState::GetInstance( )->ToggleBrightness( 0 );
				else CAuxiliaryState::GetInstance( )->ToggleBrightness( 
					CAuxiliaryState::GetInstance( )->GetBrightness( ) - 1 );
			}

			// Windowed
			else if( m_nChoice == 4 )
			{ CGame::GetInstance( )->TriggerFSTrue( ); }
		}

		if( m_pDI->KeyDown( DIK_RIGHT ) || m_pDI->JoystickDPadDown(1) )
		{
			// Music
			if( m_nChoice == 0 )
			{
				if( CGame::GetInstance( )->GetMusicVolume( ) + 1 > 100 )
					CGame::GetInstance( )->SetMusicVolume( 100 );
				else CGame::GetInstance( )->SetMusicVolume( 
				CGame::GetInstance( )->GetMusicVolume( ) + 1 );
			}

			// Sound FX
			else if( m_nChoice == 1 )
			{
				if( CGame::GetInstance( )->GetSoundFXVolume( ) + 1 > 100 )
					CGame::GetInstance( )->SetSoundFXVolume( 100 );
				else CGame::GetInstance( )->SetSoundFXVolume( 
				CGame::GetInstance( )->GetSoundFXVolume( ) + 1 );
			}

			// Brightness
			else if( m_nChoice == 3 )
			{ 
				if( CAuxiliaryState::GetInstance( )->GetBrightness( ) + 1 > 100 )
					CAuxiliaryState::GetInstance( )->ToggleBrightness( 100 );
				else CAuxiliaryState::GetInstance( )->ToggleBrightness( 
					CAuxiliaryState::GetInstance( )->GetBrightness( ) + 1 );
			}

			// Windowed
			else if( m_nChoice == 4 )
			{ CGame::GetInstance( )->TriggerFSFalse( ); }
		}

		if( m_pDI->KeyPressed( DIK_RETURN ) || m_pDI->JoystickButtonPressed(1) )
		{
			if( m_nChoice == 5 )
			{
				CAuxiliaryState::GetInstance( )->SaveConfig
					( "resource/data/Lapidem_Config.dat" );
				m_nState   = 0;
				m_nChoice  = 0;
			}
		}
		
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

		CGame::GetInstance( )->SetBrightness( CAuxiliaryState::GetInstance( )->GetBrightness( ) );
	}
	else if( 2 == m_nState ) // Save game
	{
		if(m_pDI->KeyPressed( DIK_UP ) || m_pDI->JoystickDPadPressed( 2 ) )
		{
			if( --m_nChoice < 0 ) 
				m_nChoice = 3;
		}

		if(m_pDI->KeyPressed( DIK_DOWN ) || m_pDI->JoystickDPadPressed( 3 ) )
		{
			if( ++m_nChoice > 3 )
				m_nChoice = 0;
		}

		if( m_pDI->KeyPressed( DIK_RETURN ) || m_pDI->JoystickButtonPressed( 1 ) )
		{
			if( m_nChoice == 0 ) // Save slot 1
			{
				SaveGame( 1 );
				CGame::GetInstance( )->SetPaused( false );
				m_nState = 0;
				CGame::GetInstance( )->ChangeState( CMenuState::GetInstance( ) );
			}
			else if( m_nChoice == 1 ) // Save slot 2
			{
				SaveGame( 2 );
				CGame::GetInstance( )->SetPaused( false );
				m_nState = 0;
				CGame::GetInstance( )->ChangeState( CMenuState::GetInstance( ) );
			}
			else if( m_nChoice == 2 ) // Save slot 3
			{
				SaveGame( 3 );
				CGame::GetInstance( )->SetPaused( false );
				m_nState = 0;
				CGame::GetInstance( )->ChangeState( CMenuState::GetInstance( ) );
			} 
			else 
			{
				m_nState   = 0; // Back
				m_nChoice  = 0;
			}
		}
	}
	else if( 3 == m_nState ) // Load game
	{
		if(m_pDI->KeyPressed( DIK_UP ) || m_pDI->JoystickDPadPressed( 2 ) )
		{
			if( --m_nChoice < 0 ) 
				m_nChoice = 3;
		}

		if(m_pDI->KeyPressed( DIK_DOWN ) || m_pDI->JoystickDPadPressed( 3 ) )
		{
			if( ++m_nChoice > 3 )
				m_nChoice = 0;
		}

		if( m_pDI->KeyPressed( DIK_RETURN ) || m_pDI->JoystickButtonPressed( 1 ) )
		{
			if( m_nChoice == 0 ) // Load slot 1
			{
				CGame::GetInstance( )->PushState( CLoading::GetInstance( ) );
				CLoading::GetInstance( )->SetProgress( 0 );

				CGameplayState::GetInstance( )->GetLevel( )->Clear( );
				CLoading::GetInstance( )->SetProgress( 50 );

				CMenuState::GetInstance( )->Load( 1 );
				CLoading::GetInstance( )->SetProgress( 100 );
				
				CGameplayState::GetInstance( )->GetLevel( )->GetEntryPoint( )->
					SetPosX( float( CMenuState::GetInstance( )->GetSlotOne( ).nPositionX ) );
				CGameplayState::GetInstance( )->GetPlayerOne( )->SetPosX( 
					float( CMenuState::GetInstance( )->GetSlotOne( ).nPositionX ) );
				CGameplayState::GetInstance( )->GetLevel( )->GetEntryPoint( )->
					SetPosY( float( CMenuState::GetInstance( )->GetSlotOne( ).nPositionY ) );
				CGameplayState::GetInstance( )->GetPlayerOne( )->SetPosY( 
					float( CMenuState::GetInstance( )->GetSlotOne( ).nPositionY ) );

				CGameplayState::GetInstance( )->GetPlayerOne( )->setWindEnergy( 
					CMenuState::GetInstance( )->GetSlotOne( ).manaP1.nAir );
				CGameplayState::GetInstance( )->GetPlayerOne( )->setFireEnergy( 
					CMenuState::GetInstance( )->GetSlotOne( ).manaP1.nFire );
				CGameplayState::GetInstance( )->GetPlayerOne( )->setEarthEnergy( 
					CMenuState::GetInstance( )->GetSlotOne( ).manaP1.nEarth );
				CGameplayState::GetInstance( )->GetPlayerOne( )->setIceEnergy( 
					CMenuState::GetInstance( )->GetSlotOne( ).manaP1.nIce );

				CGameplayState::GetInstance( )->GetPlayerOne( )->SetHealth( 
					CMenuState::GetInstance( )->GetSlotOne( ).nPlayerOneHealth );

				CLoading::GetInstance( )->SetProgress( 350 );

				if( 2 == CMenuState::GetInstance( )->GetSlotOne( ).nPlayerCount )
				{
					CGameplayState::GetInstance( )->GetPlayerTwo( )->SetHealth( 
						CMenuState::GetInstance( )->GetSlotOne( ).nPlayerTwoHealth );

					CGameplayState::GetInstance( )->GetPlayerTwo( )->SetPosX( 
						float( CMenuState::GetInstance( )->GetSlotOne( ).nPlayerTwoPosX ) );
					CGameplayState::GetInstance( )->GetPlayerTwo( )->SetPosY( 
						float( CMenuState::GetInstance( )->GetSlotOne( ).nPlayerTwoPosY ) );

					CGameplayState::GetInstance( )->GetPlayerTwo( )->setWindEnergy( 
						CMenuState::GetInstance( )->GetSlotOne( ).manaP2.nAir );
					CGameplayState::GetInstance( )->GetPlayerTwo( )->setFireEnergy( 
						CMenuState::GetInstance( )->GetSlotOne( ).manaP2.nFire );
					CGameplayState::GetInstance( )->GetPlayerTwo( )->setEarthEnergy( 
						CMenuState::GetInstance( )->GetSlotOne( ).manaP2.nEarth );
					CGameplayState::GetInstance( )->GetPlayerTwo( )->setIceEnergy( 
						CMenuState::GetInstance( )->GetSlotOne( ).manaP2.nIce );
				}
				CLoading::GetInstance( )->SetProgress( 500 );

				CGameplayState::GetInstance( )->SetPlayerOneScore( 
					CMenuState::GetInstance( )->GetSlotOne( ).nPlayerOneScore );
				CGameplayState::GetInstance( )->SetPlayerOneScore( 
					CMenuState::GetInstance( )->GetSlotOne( ).nPlayerTwoScore );

				CGame::GetInstance( )->SetPaused( false );
				CLoading::GetInstance( )->SetProgress( 640 );
			}
			else if( m_nChoice == 1 ) // Load slot 2
			{
				CGame::GetInstance( )->PushState( CLoading::GetInstance( ) );
				CLoading::GetInstance( )->SetProgress( 0 );

				CGameplayState::GetInstance( )->GetLevel( )->Clear( );
				CLoading::GetInstance( )->SetProgress( 50 );

				CMenuState::GetInstance( )->Load( 2 );
				CLoading::GetInstance( )->SetProgress( 100 );

				CGameplayState::GetInstance( )->GetLevel( )->GetEntryPoint( )->SetPosX( 
					float( CMenuState::GetInstance( )->GetSlotTwo( ).nPositionX ) );
				CGameplayState::GetInstance( )->GetPlayerOne( )->SetPosX( 
					float( CMenuState::GetInstance( )->GetSlotTwo( ).nPositionX ) );
				CGameplayState::GetInstance( )->GetLevel( )->GetEntryPoint( )->SetPosY( 
					float( CMenuState::GetInstance( )->GetSlotTwo( ).nPositionY ) );
				CGameplayState::GetInstance( )->GetPlayerOne( )->SetPosY( 
					float( CMenuState::GetInstance( )->GetSlotTwo( ).nPositionY ) );

				CGameplayState::GetInstance( )->GetPlayerOne( )->setWindEnergy( 
					CMenuState::GetInstance( )->GetSlotTwo( ).manaP1.nAir );
				CGameplayState::GetInstance( )->GetPlayerOne( )->setFireEnergy( 
					CMenuState::GetInstance( )->GetSlotTwo( ).manaP1.nFire );
				CGameplayState::GetInstance( )->GetPlayerOne( )->setEarthEnergy( 
					CMenuState::GetInstance( )->GetSlotTwo( ).manaP1.nEarth );
				CGameplayState::GetInstance( )->GetPlayerOne( )->setIceEnergy( 
					CMenuState::GetInstance( )->GetSlotTwo( ).manaP1.nIce );

				CGameplayState::GetInstance( )->GetPlayerOne( )->SetHealth( 
					CMenuState::GetInstance( )->GetSlotTwo( ).nPlayerOneHealth );

				if( 2 == CMenuState::GetInstance( )->GetSlotTwo( ).nPlayerCount )
				{
					CGameplayState::GetInstance( )->GetPlayerTwo( )->SetHealth( 
						CMenuState::GetInstance( )->GetSlotTwo( ).nPlayerTwoHealth );

					CGameplayState::GetInstance( )->GetPlayerTwo( )->SetPosX( 
						float( CMenuState::GetInstance( )->GetSlotTwo( ).nPlayerTwoPosX ) );
					CGameplayState::GetInstance( )->GetPlayerTwo( )->SetPosY( 
						float( CMenuState::GetInstance( )->GetSlotTwo( ).nPlayerTwoPosY ) );

					CGameplayState::GetInstance( )->GetPlayerTwo( )->setWindEnergy( 
						CMenuState::GetInstance( )->GetSlotTwo( ).manaP2.nAir );
					CGameplayState::GetInstance( )->GetPlayerTwo( )->setFireEnergy( 
						CMenuState::GetInstance( )->GetSlotTwo( ).manaP2.nFire );
					CGameplayState::GetInstance( )->GetPlayerTwo( )->setEarthEnergy( 
						CMenuState::GetInstance( )->GetSlotTwo( ).manaP2.nEarth );
					CGameplayState::GetInstance( )->GetPlayerTwo( )->setIceEnergy( 
						CMenuState::GetInstance( )->GetSlotTwo( ).manaP2.nIce );
				}

				CGameplayState::GetInstance( )->SetPlayerOneScore( 
					CMenuState::GetInstance( )->GetSlotTwo( ).nPlayerOneScore );
				CGameplayState::GetInstance( )->SetPlayerTwoScore( 
					CMenuState::GetInstance( )->GetSlotTwo( ).nPlayerTwoScore );

				CGame::GetInstance( )->SetPaused( false );
				CLoading::GetInstance( )->SetProgress( 640 );
			}
			else if( m_nChoice == 2 ) // Load slot 3
			{
				CGame::GetInstance( )->PushState( CLoading::GetInstance( ) );
				CLoading::GetInstance( )->SetProgress( 0 );

				CGameplayState::GetInstance( )->GetLevel( )->Clear( );
				CLoading::GetInstance( )->SetProgress( 50 );

				CMenuState::GetInstance( )->Load( 3 );
				CLoading::GetInstance( )->SetProgress( 100 );

				CGameplayState::GetInstance( )->GetLevel( )->GetEntryPoint( )->SetPosX( 
					float( CMenuState::GetInstance( )->GetSlotThree( ).nPositionX ) );
				CGameplayState::GetInstance( )->GetPlayerOne( )->SetPosX( 
					float( CMenuState::GetInstance( )->GetSlotThree( ).nPositionX ) );
				CGameplayState::GetInstance( )->GetLevel( )->GetEntryPoint( )->SetPosY( 
					float( CMenuState::GetInstance( )->GetSlotThree( ).nPositionY ) );
				CGameplayState::GetInstance( )->GetPlayerOne( )->SetPosY( 
					float( CMenuState::GetInstance( )->GetSlotThree( ).nPositionY ) );

				CGameplayState::GetInstance( )->GetPlayerOne( )->setWindEnergy( 
					CMenuState::GetInstance( )->GetSlotThree( ).manaP2.nAir );
				CGameplayState::GetInstance( )->GetPlayerOne( )->setFireEnergy( 
					CMenuState::GetInstance( )->GetSlotThree( ).manaP2.nFire );
				CGameplayState::GetInstance( )->GetPlayerOne( )->setEarthEnergy( 
					CMenuState::GetInstance( )->GetSlotThree( ).manaP2.nEarth );
				CGameplayState::GetInstance( )->GetPlayerOne( )->setIceEnergy( 
					CMenuState::GetInstance( )->GetSlotThree( ).manaP2.nIce );

				CGameplayState::GetInstance( )->GetPlayerOne( )->SetHealth( 
					CMenuState::GetInstance( )->GetSlotThree( ).nPlayerOneHealth );

				if( 2 == CMenuState::GetInstance( )->GetSlotThree( ).nPlayerCount )
				{
					CGameplayState::GetInstance( )->GetPlayerTwo( )->SetHealth( 
						CMenuState::GetInstance( )->GetSlotThree( ).nPlayerTwoHealth );

					CGameplayState::GetInstance( )->GetPlayerTwo( )->SetPosX( float( 
						CMenuState::GetInstance( )->GetSlotThree( ).nPlayerTwoPosX ) );
					CGameplayState::GetInstance( )->GetPlayerTwo( )->SetPosY( float( 
						CMenuState::GetInstance( )->GetSlotThree( ).nPlayerTwoPosY ) );

					CGameplayState::GetInstance( )->GetPlayerTwo( )->setWindEnergy( 
						CMenuState::GetInstance( )->GetSlotThree( ).manaP2.nAir );
					CGameplayState::GetInstance( )->GetPlayerTwo( )->setFireEnergy( 
						CMenuState::GetInstance( )->GetSlotThree( ).manaP2.nFire );
					CGameplayState::GetInstance( )->GetPlayerTwo( )->setEarthEnergy( 
						CMenuState::GetInstance( )->GetSlotThree( ).manaP2.nEarth );
					CGameplayState::GetInstance( )->GetPlayerTwo( )->setIceEnergy( 
						CMenuState::GetInstance( )->GetSlotThree( ).manaP2.nIce );
				}

				CGameplayState::GetInstance( )->SetPlayerOneScore( 
					CMenuState::GetInstance( )->GetSlotThree( ).nPlayerOneScore );
				CGameplayState::GetInstance( )->SetPlayerTwoScore( 
					CMenuState::GetInstance( )->GetSlotThree( ).nPlayerTwoScore );

				CGame::GetInstance( )->SetPaused( false );
				CLoading::GetInstance( )->SetProgress( 640 );
			} 
			else 
			{
				m_nState   = 0; // Back
				m_nChoice  = 0;
			} 
		}
	}

	return true;
}

void CPauseMenuState::Update( float fET )
{ /* NOTHING HERE WILL BE CALLED */ }

void CPauseMenuState::Render( )
{
	m_pTM->Draw( m_nImageID, 0, 0, 1.0f );

	// - - - - - - - - - - - - - - - -
	// Paused menu
	// - - - - - - - - - - - - - - - -
	if( m_nState == 0 )
	{
		if( m_nChoice == 0 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 250, 196, 
				1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "RESUME", 280, 200, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "OPTIONS", 270, 220, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SAVE AND QUIT", 270, 240, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "LOAD SLOT", 270, 260, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "RAGE QUIT", 270, 280, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 1 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 250, 216, 
				1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "RESUME", 270, 200, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "OPTIONS", 280, 220, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SAVE AND QUIT", 270, 240, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "LOAD SLOT", 270, 260, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "RAGE QUIT", 270, 280, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 2 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 250, 236, 
				1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "RESUME", 270, 200, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "OPTIONS", 270, 220, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SAVE AND QUIT", 280, 240, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "LOAD SLOT", 270, 260, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "RAGE QUIT", 270, 280, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 3 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 250, 256, 
				1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "RESUME", 270, 200, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "OPTIONS", 270, 220, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SAVE AND QUIT", 270, 240, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "LOAD SLOT", 280, 260, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "RAGE QUIT", 270, 280, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 4 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 250, 276, 
				1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "RESUME", 270, 200, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "OPTIONS", 270, 220, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SAVE AND QUIT", 270, 240, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "LOAD SLOT", 270, 260, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "RAGE QUIT", 280, 280, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
	}
	// - - - - - - - - - - - - - - - -
	// Options
	// - - - - - - - - - - - - - - - -
	else if( m_nState == 1 )
	{
		char cBuffer[64];

		if( m_nChoice == 0 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 50, 
				146, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "MUSIC VOL", 70, 
				130, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetMusicVolume( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 150, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "SOUND FX VOL", 70, 
				180, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetSoundFXVolume( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 200, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULTY", 70, 
				230, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			if( 1 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "EASY", 100, 
					250, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 220, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 375, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else if( 2 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "EASY", 100, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 220, 
					250, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 375, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else if( 3 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "EASY", 100, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 220, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 375, 
					250, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}
			
			CGame::GetInstance( )->GetFont( )->Draw( "DARKNESS", 70, 
				280, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetBrightness( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 300, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "WINDOWED MODE", 70, 
				330, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			
			if( 1 == CGame::GetInstance( )->GetFSValue( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "ON", 100, 
					350, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "OFF", 170, 
					350, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else 
			{
				CGame::GetInstance( )->GetFont( )->Draw( "ON", 100, 
					350, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "OFF", 170, 
					350, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}

			CGame::GetInstance( )->GetFont( )->Draw( "DONE", 70, 400, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 1 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 50, 
				196, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "MUSIC VOL", 70, 
				130, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetMusicVolume( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 150, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "SOUND FX VOL", 70, 
				180, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetSoundFXVolume( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 200, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULTY", 70, 
				230, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			if( 1 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "EASY", 100, 
					250, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 220, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 375, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else if( 2 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "EASY", 100, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 220, 
					250, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 375, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else if( 3 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "EASY", 100, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 220, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 375, 
					250, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}
			
			CGame::GetInstance( )->GetFont( )->Draw( "DARKNESS", 70, 
				280, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetBrightness( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 300, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "WINDOWED MODE", 70, 
				330, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			
			if( 1 == CGame::GetInstance( )->GetFSValue( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "ON", 100, 
					350, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "OFF", 170, 
					350, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else 
			{
				CGame::GetInstance( )->GetFont( )->Draw( "ON", 100, 
					350, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "OFF", 170, 
					350, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}

			CGame::GetInstance( )->GetFont( )->Draw( "DONE", 70, 400, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 2 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 50, 
				246, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "MUSIC VOL", 70, 
				130, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetMusicVolume( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 150, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "SOUND FX VOL", 70, 
				180, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetSoundFXVolume( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 200, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			
			CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULTY", 70, 
				230, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			if( 1 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "EASY", 100, 
					250, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 220, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 375, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else if( 2 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "EASY", 100, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 220, 
					250, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 375, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else if( 3 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "EASY", 100, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 220, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 375, 
					250, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}
			
			CGame::GetInstance( )->GetFont( )->Draw( "DARKNESS", 70, 
				280, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetBrightness( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 300, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "WINDOWED MODE", 70, 
				330, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			
			if( 1 == CGame::GetInstance( )->GetFSValue( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "ON", 100, 
					350, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "OFF", 170, 
					350, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else 
			{
				CGame::GetInstance( )->GetFont( )->Draw( "ON", 100, 
					350, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "OFF", 170, 
					350, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}

			CGame::GetInstance( )->GetFont( )->Draw( "DONE", 70, 400, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 3 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 50, 
				296, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "MUSIC VOL", 70, 
				130, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetMusicVolume( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 150, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "SOUND FX VOL", 70, 
				180, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetSoundFXVolume( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 200, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			
			CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULTY", 70, 
				230, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			if( 1 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "EASY", 100, 
					250, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 220, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 375, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else if( 2 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "EASY", 100, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 220, 
					250, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 375, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else if( 3 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "EASY", 100, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 220, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 375, 
					250, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}
			
			CGame::GetInstance( )->GetFont( )->Draw( "DARKNESS", 70, 
				280, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetBrightness( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 300, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "WINDOWED MODE", 70, 
				330, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			if( 1 == CGame::GetInstance( )->GetFSValue( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "ON", 100, 
					350, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "OFF", 170, 
					350, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else 
			{
				CGame::GetInstance( )->GetFont( )->Draw( "ON", 100, 
					350, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "OFF", 170, 
					350, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}

			CGame::GetInstance( )->GetFont( )->Draw( "DONE", 70, 400, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 4 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 50, 
				346, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "MUSIC VOL", 70, 
				130, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetMusicVolume( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 150, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "SOUND FX VOL", 70, 
				180, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetSoundFXVolume( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 200, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			
			CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULTY", 70, 
				230, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			if( 1 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "EASY", 100, 
					280, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 220, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 375, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else if( 2 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "EASY", 100, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 220, 
					250, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 375, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else if( 3 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "EASY", 100, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 220, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 375, 
					250, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}
			
			CGame::GetInstance( )->GetFont( )->Draw( "DARKNESS", 70, 
				280, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetBrightness( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 300, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "WINDOWED MODE", 70, 
				330, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			
			if( 1 == CGame::GetInstance( )->GetFSValue( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "ON", 100, 
					350, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "OFF", 170, 
					350, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else 
			{
				CGame::GetInstance( )->GetFont( )->Draw( "ON", 100, 
					350, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "OFF", 170, 
					350, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}

			CGame::GetInstance( )->GetFont( )->Draw( "DONE", 70, 400, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 5 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 50, 
				396, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "MUSIC VOL", 70, 
				130, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetMusicVolume( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 150, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "SOUND FX VOL", 70, 
				180, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetSoundFXVolume( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 200, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			
			CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULTY", 70, 
				230, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			if( 1 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "EASY", 100, 
					250, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 220, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 375, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else if( 2 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "EASY", 100, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 220, 
					250, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 375, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else if( 3 == CGameplayState::GetInstance( )->GetDifficulty( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "EASY", 100, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "NORMAL", 220, 
					250, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "DIFFICULT", 375, 
					250, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}
			
			CGame::GetInstance( )->GetFont( )->Draw( "DARKNESS", 70, 
				280, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetBrightness( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 100, 300, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "WINDOWED MODE", 70, 
				330, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			
			if( 1 == CGame::GetInstance( )->GetFSValue( ) )
			{
				CGame::GetInstance( )->GetFont( )->Draw( "ON", 100, 
					350, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "OFF", 170, 
					350, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
			}
			else 
			{
				CGame::GetInstance( )->GetFont( )->Draw( "ON", 100, 
					350, 0.8f, D3DCOLOR_ARGB( 120, 255, 255, 255 ) );
				CGame::GetInstance( )->GetFont( )->Draw( "OFF", 170, 
					350, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}

			CGame::GetInstance( )->GetFont( )->Draw( "DONE", 80, 400, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
	}
	// - - - - - - - - - - - - - - - -
	// Save slots
	// - - - - - - - - - - - - - - - -
	else if( m_nState == 2 ) 
	{
		if( m_nChoice == 0 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 250, 196, 
				1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 1", 280, 200, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 2", 270, 220, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 3", 270, 240, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "BACK", 270, 260, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 1 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 250, 216, 
				1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 1", 270, 200, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 2", 280, 220, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 3", 270, 240, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "BACK", 270, 260, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 2 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 250, 236, 
				1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 1", 270, 200, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 2", 270, 220, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 3", 280, 240, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "BACK", 270, 260, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 3 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 250, 256, 
				1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 1", 270, 200, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 2", 270, 220, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 3", 270, 240, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "BACK", 280, 260, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
	}
	// - - - - - - - - - - - - - - - -
	// Load slots
	// - - - - - - - - - - - - - - - -
	else if( m_nState == 3 ) 
	{
		if( m_nChoice == 0 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 250, 196, 
				1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 1", 280, 200, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 2", 270, 220, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 3", 270, 240, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "BACK", 270, 260, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 1 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 250, 216, 
				1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 1", 270, 200, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 2", 280, 220, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 3", 270, 240, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "BACK", 270, 260, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 2 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 250, 236, 
				1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 1", 270, 200, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 2", 270, 220, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 3", 280, 240, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "BACK", 270, 260, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 3 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 250, 256, 
				1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 1", 270, 200, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 2", 270, 220, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 3", 270, 240, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "BACK", 280, 260, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
	}
}

void CPauseMenuState::Exit( )
{
	m_pTM->UnloadTexture( m_nImageID );
}

bool CPauseMenuState::SaveGame( int _nSlot )
{
	// - - - - - - - - - - - - - -
	// Read in slot 1 / 2 / 3 info.
	// - - - - - - - - - - - - - -
	char _saveSlot[128] = "resource/data/Lapidem_GameSaves.bin";
	ifstream fin( _saveSlot, std::ios_base::in | std::ios_base::binary );

	CLevel* pLevel = CGameplayState::GetInstance()->GetLevel();
	int nDataChunkSize;

	if( fin.is_open( ) )
	{
		// - - - - - - - - - - - - - - 
		// Slot 1
		// - - - - - - - - - - - - - - 
		fin.read( ( char* )&_tSlotOne._nPlayerCount, sizeof( int ) );
		fin.read( ( char* )&_tSlotOne._nPositionX, sizeof( int ) );
		fin.read( ( char* )&_tSlotOne._nPositionY, sizeof( int ) );

		fin.read( ( char* )&_tSlotOne._manaP1._nFire, sizeof( int ) );
		fin.read( ( char* )&_tSlotOne._manaP1._nEarth, sizeof( int ) );
		fin.read( ( char* )&_tSlotOne._manaP1._nIce, sizeof( int ) );
		fin.read( ( char* )&_tSlotOne._manaP1._nAir, sizeof( int ) );

		fin.read( ( char* )&_tSlotOne._spellP1._nFireXP, sizeof( int ) );
		fin.read( ( char* )&_tSlotOne._spellP1._nFireLevel, sizeof( int ) );
		fin.read( ( char* )&_tSlotOne._spellP1._nEarthXP, sizeof( int ) );
		fin.read( ( char* )&_tSlotOne._spellP1._nEarthLevel, sizeof( int ) );
		fin.read( ( char* )&_tSlotOne._spellP1._nIceXP, sizeof( int ) );
		fin.read( ( char* )&_tSlotOne._spellP1._nIceLevel, sizeof( int ) );
		fin.read( ( char* )&_tSlotOne._spellP1._nAirXP, sizeof( int ) );
		fin.read( ( char* )&_tSlotOne._spellP1._nAirLevel, sizeof( int ) );

		fin.read( ( char* )&_tSlotOne._nPlayerOneHealth, sizeof( int ) );

		if( 2 == _tSlotOne._nPlayerCount )
		{
			fin.read( ( char* )&_tSlotOne._nPlayerTwoHealth, sizeof( int ) );

			fin.read( ( char* )&_tSlotOne._nPlayerTwoPosX, sizeof( int ) );
			fin.read( ( char* )&_tSlotOne._nPlayerTwoPosY, sizeof( int ) );

			fin.read( ( char* )&_tSlotOne._manaP2._nFire, sizeof( int ) );
			fin.read( ( char* )&_tSlotOne._manaP2._nEarth, sizeof( int ) );
			fin.read( ( char* )&_tSlotOne._manaP2._nIce, sizeof( int ) );
			fin.read( ( char* )&_tSlotOne._manaP2._nAir, sizeof( int ) );

			fin.read( ( char* )&_tSlotOne._spellP2._nFireXP, sizeof( int ) );
			fin.read( ( char* )&_tSlotOne._spellP2._nFireLevel, sizeof( int ) );
			fin.read( ( char* )&_tSlotOne._spellP2._nEarthXP, sizeof( int ) );
			fin.read( ( char* )&_tSlotOne._spellP2._nEarthLevel, sizeof( int ) );
			fin.read( ( char* )&_tSlotOne._spellP2._nIceXP, sizeof( int ) );
			fin.read( ( char* )&_tSlotOne._spellP2._nIceLevel, sizeof( int ) );
			fin.read( ( char* )&_tSlotOne._spellP2._nAirXP, sizeof( int ) );
			fin.read( ( char* )&_tSlotOne._spellP2._nAirLevel, sizeof( int ) );
		}

		fin.read( ( char* )&_tSlotOne._nPlayerOneScore, sizeof( int ) );
		fin.read( ( char* )&_tSlotOne._nPlayerTwoScore, sizeof( int ) );

		fin.read( ( char* )&nDataChunkSize, sizeof( int ) );  // return size that can skip the whole next part

		if( _nSlot == 1 )                                     // if it's slot one, skip loading from file cuz it will be changed any way
			fin.seekg( nDataChunkSize, ios_base::cur );
		else pLevel->LoadLevelToMemory( &fin, m_tLevelInfo[0].szFileName, 
			&m_tLevelInfo[0].pTerrainTiles, &m_tLevelInfo[0].pEventTiles, &m_tLevelInfo[0].pSwitches );

		// - - - - - - - - - - - - - - 
		// Slot 2
		// - - - - - - - - - - - - - - 
		fin.read( ( char* )&_tSlotTwo._nPlayerCount, sizeof( int ) );
		fin.read( ( char* )&_tSlotTwo._nPositionX, sizeof( int ) );
		fin.read( ( char* )&_tSlotTwo._nPositionY, sizeof( int ) );

		fin.read( ( char* )&_tSlotTwo._manaP1._nFire, sizeof( int ) );
		fin.read( ( char* )&_tSlotTwo._manaP1._nEarth, sizeof( int ) );
		fin.read( ( char* )&_tSlotTwo._manaP1._nIce, sizeof( int ) );
		fin.read( ( char* )&_tSlotTwo._manaP1._nAir, sizeof( int ) );

		fin.read( ( char* )&_tSlotTwo._spellP1._nFireXP, sizeof( int ) );
		fin.read( ( char* )&_tSlotTwo._spellP1._nFireLevel, sizeof( int ) );
		fin.read( ( char* )&_tSlotTwo._spellP1._nEarthXP, sizeof( int ) );
		fin.read( ( char* )&_tSlotTwo._spellP1._nEarthLevel, sizeof( int ) );
		fin.read( ( char* )&_tSlotTwo._spellP1._nIceXP, sizeof( int ) );
		fin.read( ( char* )&_tSlotTwo._spellP1._nIceLevel, sizeof( int ) );
		fin.read( ( char* )&_tSlotTwo._spellP1._nAirXP, sizeof( int ) );
		fin.read( ( char* )&_tSlotTwo._spellP1._nAirLevel, sizeof( int ) );

		fin.read( ( char* )&_tSlotTwo._nPlayerOneHealth, sizeof( int ) );

		if( 2 == _tSlotTwo._nPlayerCount )
		{
			fin.read( ( char* )&_tSlotTwo._nPlayerTwoHealth, sizeof( int ) );

			fin.read( ( char* )&_tSlotTwo._nPlayerTwoPosX, sizeof( int ) );
			fin.read( ( char* )&_tSlotTwo._nPlayerTwoPosY, sizeof( int ) );

			fin.read( ( char* )&_tSlotTwo._manaP2._nFire, sizeof( int ) );
			fin.read( ( char* )&_tSlotTwo._manaP2._nEarth, sizeof( int ) );
			fin.read( ( char* )&_tSlotTwo._manaP2._nIce, sizeof( int ) );
			fin.read( ( char* )&_tSlotTwo._manaP2._nAir, sizeof( int ) );

			fin.read( ( char* )&_tSlotTwo._spellP2._nFireXP, sizeof( int ) );
			fin.read( ( char* )&_tSlotTwo._spellP2._nFireLevel, sizeof( int ) );
			fin.read( ( char* )&_tSlotTwo._spellP2._nEarthXP, sizeof( int ) );
			fin.read( ( char* )&_tSlotTwo._spellP2._nEarthLevel, sizeof( int ) );
			fin.read( ( char* )&_tSlotTwo._spellP2._nIceXP, sizeof( int ) );
			fin.read( ( char* )&_tSlotTwo._spellP2._nIceLevel, sizeof( int ) );
			fin.read( ( char* )&_tSlotTwo._spellP2._nAirXP, sizeof( int ) );
			fin.read( ( char* )&_tSlotTwo._spellP2._nAirLevel, sizeof( int ) );
		}

		fin.read( ( char* )&_tSlotTwo._nPlayerOneScore, sizeof( int ) );
		fin.read( ( char* )&_tSlotTwo._nPlayerTwoScore, sizeof( int ) );

		fin.read( ( char* )&nDataChunkSize, sizeof( int ) );  // return size that can skip the whole next part

		if( _nSlot == 2 )                                     // if it's slot two, skip loading from file cuz it will be changed any way
			fin.seekg( nDataChunkSize, ios_base::cur );
		else pLevel->LoadLevelToMemory( &fin, m_tLevelInfo[1].szFileName, 
			&m_tLevelInfo[1].pTerrainTiles, &m_tLevelInfo[1].pEventTiles, &m_tLevelInfo[1].pSwitches );

		// - - - - - - - - - - - - - - 
		// Slot 3
		// - - - - - - - - - - - - - - 
		fin.read( ( char* )&_tSlotThree._nPlayerCount, sizeof( int ) );
		fin.read( ( char* )&_tSlotThree._nPositionX, sizeof( int ) );
		fin.read( ( char* )&_tSlotThree._nPositionY, sizeof( int ) );

		fin.read( ( char* )&_tSlotThree._manaP1._nFire, sizeof( int ) );
		fin.read( ( char* )&_tSlotThree._manaP1._nEarth, sizeof( int ) );
		fin.read( ( char* )&_tSlotThree._manaP1._nIce, sizeof( int ) );
		fin.read( ( char* )&_tSlotThree._manaP1._nAir, sizeof( int ) );

		fin.read( ( char* )&_tSlotThree._spellP1._nFireXP, sizeof( int ) );
		fin.read( ( char* )&_tSlotThree._spellP1._nFireLevel, sizeof( int ) );
		fin.read( ( char* )&_tSlotThree._spellP1._nEarthXP, sizeof( int ) );
		fin.read( ( char* )&_tSlotThree._spellP1._nEarthLevel, sizeof( int ) );
		fin.read( ( char* )&_tSlotThree._spellP1._nIceXP, sizeof( int ) );
		fin.read( ( char* )&_tSlotThree._spellP1._nIceLevel, sizeof( int ) );
		fin.read( ( char* )&_tSlotThree._spellP1._nAirXP, sizeof( int ) );
		fin.read( ( char* )&_tSlotThree._spellP1._nAirLevel, sizeof( int ) );

		fin.read( ( char* )&_tSlotThree._nPlayerOneHealth, sizeof( int ) );

		if( 2 == _tSlotThree._nPlayerCount )
		{
			fin.read( ( char* )&_tSlotThree._nPlayerTwoHealth, sizeof( int ) );

			fin.read( ( char* )&_tSlotThree._nPlayerTwoPosX, sizeof( int ) );
			fin.read( ( char* )&_tSlotThree._nPlayerTwoPosY, sizeof( int ) );

			fin.read( ( char* )&_tSlotThree._manaP2._nFire, sizeof( int ) );
			fin.read( ( char* )&_tSlotThree._manaP2._nEarth, sizeof( int ) );
			fin.read( ( char* )&_tSlotThree._manaP2._nIce, sizeof( int ) );
			fin.read( ( char* )&_tSlotThree._manaP2._nAir, sizeof( int ) );

			fin.read( ( char* )&_tSlotThree._spellP2._nFireXP, sizeof( int ) );
			fin.read( ( char* )&_tSlotThree._spellP2._nFireLevel, sizeof( int ) );
			fin.read( ( char* )&_tSlotThree._spellP2._nEarthXP, sizeof( int ) );
			fin.read( ( char* )&_tSlotThree._spellP2._nEarthLevel, sizeof( int ) );
			fin.read( ( char* )&_tSlotThree._spellP2._nIceXP, sizeof( int ) );
			fin.read( ( char* )&_tSlotThree._spellP2._nIceLevel, sizeof( int ) );
			fin.read( ( char* )&_tSlotThree._spellP2._nAirXP, sizeof( int ) );
			fin.read( ( char* )&_tSlotThree._spellP2._nAirLevel, sizeof( int ) );
		}

		fin.read( ( char* )&_tSlotThree._nPlayerOneScore, sizeof( int ) );
		fin.read( ( char* )&_tSlotThree._nPlayerTwoScore, sizeof( int ) );

		fin.read( ( char* )&nDataChunkSize, sizeof( int ) ); // return size that can skip the whole next part

		if( _nSlot == 3 )                                    // if it's slot three, skip loading from file cuz it will be changed any way
			fin.seekg( nDataChunkSize, ios_base::cur );
		else pLevel->LoadLevelToMemory( &fin, m_tLevelInfo[2].szFileName, 
			&m_tLevelInfo[2].pTerrainTiles, &m_tLevelInfo[2].pEventTiles, &m_tLevelInfo[2].pSwitches );
	} fin.close( );

	// - - - - - - - - - - - - - -
	// Change the current slot info.
	// - - - - - - - - - - - - - -
	if( 1 == _nSlot )
	{
		if( 1 == CMenuState::GetInstance( )->GetPlayerCount( ) )
			_tSlotOne._nPlayerCount = 1;
		else _tSlotOne._nPlayerCount = 2;

		_tSlotOne._nPositionX         = int( CGameplayState::GetInstance( )->GetPlayerOne( )->GetPosX( ) );
		_tSlotOne._nPositionY         = int( CGameplayState::GetInstance( )->GetPlayerOne( )->GetPosY( ) );

		_tSlotOne._nPlayerOneScore    = CGameplayState::GetInstance( )->GetPlayerOne( )->GetScore( );
		_tSlotOne._nPlayerOneHealth   = CGameplayState::GetInstance( )->GetPlayerOne( )->GetHealth( );

		_tSlotOne._spellP1._nAirLevel     = CSpellFactory::GetInstance( )->GetWindLevel( );
		_tSlotOne._spellP1._nAirXP        = CSpellFactory::GetInstance( )->GetWindXP( );
		_tSlotOne._spellP1._nEarthLevel   = CSpellFactory::GetInstance( )->GetEarthLevel( );
		_tSlotOne._spellP1._nEarthXP      = CSpellFactory::GetInstance( )->GetEarthXP( );
		_tSlotOne._spellP1._nFireLevel    = CSpellFactory::GetInstance( )->GetFireLevel( );
		_tSlotOne._spellP1._nFireXP       = CSpellFactory::GetInstance( )->GetFireXP( );
		_tSlotOne._spellP1._nIceLevel     = CSpellFactory::GetInstance( )->GetIceLevel( );
		_tSlotOne._spellP1._nIceXP        = CSpellFactory::GetInstance( )->GetIceXP( );

		if( 2 == _tSlotOne._nPlayerCount )
		{
			_tSlotOne._nPlayerTwoScore    = CGameplayState::GetInstance( )->GetPlayerTwo( )->GetScore( );
			_tSlotOne._nPlayerTwoHealth   = CGameplayState::GetInstance( )->GetPlayerTwo( )->GetHealth( );

			_tSlotOne._nPlayerTwoPosX     = int( CGameplayState::GetInstance( )->GetPlayerTwo( )->GetPosX( ) );
			_tSlotOne._nPlayerTwoPosY     = int( CGameplayState::GetInstance( )->GetPlayerTwo( )->GetPosY( ) );

			_tSlotOne._manaP2._nFire      = CGameplayState::GetInstance( )->GetPlayerTwo( )->GetFireEnergy( );
			_tSlotOne._manaP2._nEarth     = CGameplayState::GetInstance( )->GetPlayerTwo( )->GetEarthEnergy( );
			_tSlotOne._manaP2._nIce       = CGameplayState::GetInstance( )->GetPlayerTwo( )->GetIceEnergy( );
			_tSlotOne._manaP2._nAir       = CGameplayState::GetInstance( )->GetPlayerTwo( )->GetWindEnergy( );

			_tSlotOne._spellP2._nAirLevel     = CSpellFactory::GetInstance( )->GetWindLevel( );
			_tSlotOne._spellP2._nAirXP        = CSpellFactory::GetInstance( )->GetWindXP( );
			_tSlotOne._spellP2._nEarthLevel   = CSpellFactory::GetInstance( )->GetEarthLevel( );
			_tSlotOne._spellP2._nEarthXP      = CSpellFactory::GetInstance( )->GetEarthXP( );
			_tSlotOne._spellP2._nFireLevel    = CSpellFactory::GetInstance( )->GetFireLevel( );
			_tSlotOne._spellP2._nFireXP       = CSpellFactory::GetInstance( )->GetFireXP( );
			_tSlotOne._spellP2._nIceLevel     = CSpellFactory::GetInstance( )->GetIceLevel( );
			_tSlotOne._spellP2._nIceXP        = CSpellFactory::GetInstance( )->GetIceXP( );
		}

		_tSlotOne._manaP1._nFire      = CGameplayState::GetInstance( )->GetPlayerOne( )->GetFireEnergy( );
		_tSlotOne._manaP1._nEarth     = CGameplayState::GetInstance( )->GetPlayerOne( )->GetEarthEnergy( );
		_tSlotOne._manaP1._nIce       = CGameplayState::GetInstance( )->GetPlayerOne( )->GetIceEnergy( );
		_tSlotOne._manaP1._nAir       = CGameplayState::GetInstance( )->GetPlayerOne( )->GetWindEnergy( );

		CAuxiliaryState::GetInstance( )->UpdateSlotInfo( "resource/data/Lapidem_SlotInfo.bin", 1 );
	}
	else if( 2 == _nSlot )
	{
		if( 1 == CMenuState::GetInstance( )->GetPlayerCount( ) )
			_tSlotTwo._nPlayerCount = 1;
		else _tSlotTwo._nPlayerCount = 2;

		_tSlotTwo._nPositionX         = int( CGameplayState::GetInstance( )->GetPlayerOne( )->GetPosX( ) );
		_tSlotTwo._nPositionY         = int( CGameplayState::GetInstance( )->GetPlayerOne( )->GetPosY( ) );

		_tSlotTwo._nPlayerOneScore    = CGameplayState::GetInstance( )->GetPlayerOne( )->GetScore( );
		_tSlotTwo._nPlayerOneHealth   = CGameplayState::GetInstance( )->GetPlayerOne( )->GetHealth( );

		_tSlotTwo._spellP1._nAirLevel     = CSpellFactory::GetInstance( )->GetWindLevel( );
		_tSlotTwo._spellP1._nAirXP        = CSpellFactory::GetInstance( )->GetWindXP( );
		_tSlotTwo._spellP1._nEarthLevel   = CSpellFactory::GetInstance( )->GetEarthLevel( );
		_tSlotTwo._spellP1._nEarthXP      = CSpellFactory::GetInstance( )->GetEarthXP( );
		_tSlotTwo._spellP1._nFireLevel    = CSpellFactory::GetInstance( )->GetFireLevel( );
		_tSlotTwo._spellP1._nFireXP       = CSpellFactory::GetInstance( )->GetFireXP( );
		_tSlotTwo._spellP1._nIceLevel     = CSpellFactory::GetInstance( )->GetIceLevel( );
		_tSlotTwo._spellP1._nIceXP        = CSpellFactory::GetInstance( )->GetIceXP( );

		if( 2 == _tSlotTwo._nPlayerCount )
		{
			_tSlotTwo._nPlayerTwoScore    = CGameplayState::GetInstance( )->GetPlayerTwo( )->GetScore( );
			_tSlotTwo._nPlayerTwoHealth   = CGameplayState::GetInstance( )->GetPlayerTwo( )->GetHealth( );

			_tSlotTwo._nPlayerTwoPosX     = int( CGameplayState::GetInstance( )->GetPlayerTwo( )->GetPosX( ) );
			_tSlotTwo._nPlayerTwoPosY     = int( CGameplayState::GetInstance( )->GetPlayerTwo( )->GetPosY( ) );

			_tSlotTwo._manaP2._nFire      = CGameplayState::GetInstance( )->GetPlayerTwo( )->GetFireEnergy( );
			_tSlotTwo._manaP2._nEarth     = CGameplayState::GetInstance( )->GetPlayerTwo( )->GetEarthEnergy( );
			_tSlotTwo._manaP2._nIce       = CGameplayState::GetInstance( )->GetPlayerTwo( )->GetIceEnergy( );
			_tSlotTwo._manaP2._nAir       = CGameplayState::GetInstance( )->GetPlayerTwo( )->GetWindEnergy( );

			_tSlotTwo._spellP2._nAirLevel     = CSpellFactory::GetInstance( )->GetWindLevel( );
			_tSlotTwo._spellP2._nAirXP        = CSpellFactory::GetInstance( )->GetWindXP( );
			_tSlotTwo._spellP2._nEarthLevel   = CSpellFactory::GetInstance( )->GetEarthLevel( );
			_tSlotTwo._spellP2._nEarthXP      = CSpellFactory::GetInstance( )->GetEarthXP( );
			_tSlotTwo._spellP2._nFireLevel    = CSpellFactory::GetInstance( )->GetFireLevel( );
			_tSlotTwo._spellP2._nFireXP       = CSpellFactory::GetInstance( )->GetFireXP( );
			_tSlotTwo._spellP2._nIceLevel     = CSpellFactory::GetInstance( )->GetIceLevel( );
			_tSlotTwo._spellP2._nIceXP        = CSpellFactory::GetInstance( )->GetIceXP( );
		}

		_tSlotTwo._manaP1._nFire      = CGameplayState::GetInstance( )->GetPlayerOne( )->GetFireEnergy( );
		_tSlotTwo._manaP1._nEarth     = CGameplayState::GetInstance( )->GetPlayerOne( )->GetEarthEnergy( );
		_tSlotTwo._manaP1._nIce       = CGameplayState::GetInstance( )->GetPlayerOne( )->GetIceEnergy( );
		_tSlotTwo._manaP1._nAir       = CGameplayState::GetInstance( )->GetPlayerOne( )->GetWindEnergy( );

		CAuxiliaryState::GetInstance( )->UpdateSlotInfo( "resource/data/Lapidem_SlotInfo.bin", 2 );
	}
	else if( 3 == _nSlot )
	{
		if( 1 == CMenuState::GetInstance( )->GetPlayerCount( ) )
			_tSlotThree._nPlayerCount = 1;
		else _tSlotThree._nPlayerCount = 2;

		_tSlotThree._nPositionX         = int( CGameplayState::GetInstance( )->GetPlayerOne( )->GetPosX( ) );
		_tSlotThree._nPositionY         = int( CGameplayState::GetInstance( )->GetPlayerOne( )->GetPosY( ) );

		_tSlotThree._nPlayerOneScore    = CGameplayState::GetInstance( )->GetPlayerOne( )->GetScore( );
		_tSlotThree._nPlayerOneHealth   = CGameplayState::GetInstance( )->GetPlayerOne( )->GetHealth( );

		_tSlotThree._spellP1._nAirLevel     = CSpellFactory::GetInstance( )->GetWindLevel( );
		_tSlotThree._spellP1._nAirXP        = CSpellFactory::GetInstance( )->GetWindXP( );
		_tSlotThree._spellP1._nEarthLevel   = CSpellFactory::GetInstance( )->GetEarthLevel( );
		_tSlotThree._spellP1._nEarthXP      = CSpellFactory::GetInstance( )->GetEarthXP( );
		_tSlotThree._spellP1._nFireLevel    = CSpellFactory::GetInstance( )->GetFireLevel( );
		_tSlotThree._spellP1._nFireXP       = CSpellFactory::GetInstance( )->GetFireXP( );
		_tSlotThree._spellP1._nIceLevel     = CSpellFactory::GetInstance( )->GetIceLevel( );
		_tSlotThree._spellP1._nIceXP        = CSpellFactory::GetInstance( )->GetIceXP( );

		if( 2 == _tSlotThree._nPlayerCount )
		{
			_tSlotThree._nPlayerTwoScore    = CGameplayState::GetInstance( )->GetPlayerTwo( )->GetScore( );
			_tSlotThree._nPlayerTwoHealth   = CGameplayState::GetInstance( )->GetPlayerTwo( )->GetHealth( );

			_tSlotThree._nPlayerTwoPosX     = int( CGameplayState::GetInstance( )->GetPlayerTwo( )->GetPosX( ) );
			_tSlotThree._nPlayerTwoPosY     = int( CGameplayState::GetInstance( )->GetPlayerTwo( )->GetPosY( ) );

			_tSlotThree._manaP2._nFire      = CGameplayState::GetInstance( )->GetPlayerTwo( )->GetFireEnergy( );
			_tSlotThree._manaP2._nEarth     = CGameplayState::GetInstance( )->GetPlayerTwo( )->GetEarthEnergy( );
			_tSlotThree._manaP2._nIce       = CGameplayState::GetInstance( )->GetPlayerTwo( )->GetIceEnergy( );
			_tSlotThree._manaP2._nAir       = CGameplayState::GetInstance( )->GetPlayerTwo( )->GetWindEnergy( );

			_tSlotThree._spellP2._nAirLevel     = CSpellFactory::GetInstance( )->GetWindLevel( );
			_tSlotThree._spellP2._nAirXP        = CSpellFactory::GetInstance( )->GetWindXP( );
			_tSlotThree._spellP2._nEarthLevel   = CSpellFactory::GetInstance( )->GetEarthLevel( );
			_tSlotThree._spellP2._nEarthXP      = CSpellFactory::GetInstance( )->GetEarthXP( );
			_tSlotThree._spellP2._nFireLevel    = CSpellFactory::GetInstance( )->GetFireLevel( );
			_tSlotThree._spellP2._nFireXP       = CSpellFactory::GetInstance( )->GetFireXP( );
			_tSlotThree._spellP2._nIceLevel     = CSpellFactory::GetInstance( )->GetIceLevel( );
			_tSlotThree._spellP2._nIceXP        = CSpellFactory::GetInstance( )->GetIceXP( );
		}

		_tSlotThree._manaP1._nFire      = CGameplayState::GetInstance( )->GetPlayerOne( )->GetFireEnergy( );
		_tSlotThree._manaP1._nEarth     = CGameplayState::GetInstance( )->GetPlayerOne( )->GetEarthEnergy( );
		_tSlotThree._manaP1._nIce       = CGameplayState::GetInstance( )->GetPlayerOne( )->GetIceEnergy( );
		_tSlotThree._manaP1._nAir       = CGameplayState::GetInstance( )->GetPlayerOne( )->GetWindEnergy( );

		CAuxiliaryState::GetInstance( )->UpdateSlotInfo( "resource/data/Lapidem_SlotInfo.bin", 3 );
	}

	// - - - - - - - - - - - - - -
	// Save all slots.
	// - - - - - - - - - - - - - -
	ofstream fout( _saveSlot, std::ios_base::out | std::ios::trunc | std::ios::binary );

	// - - - - - - - - - - - - - - 
	// Slot 1
	// - - - - - - - - - - - - - - 
	fout.write( ( char* )&_tSlotOne._nPlayerCount, sizeof( int ) );
	fout.write( ( char* )&_tSlotOne._nPositionX, sizeof( int ) );
	fout.write( ( char* )&_tSlotOne._nPositionY, sizeof( int ) );

	fout.write( ( char* )&_tSlotOne._manaP1._nFire, sizeof( int ) );
	fout.write( ( char* )&_tSlotOne._manaP1._nEarth, sizeof( int ) );
	fout.write( ( char* )&_tSlotOne._manaP1._nIce, sizeof( int ) );
	fout.write( ( char* )&_tSlotOne._manaP1._nAir, sizeof( int ) );

	fout.write( ( char* )&_tSlotOne._spellP1._nFireXP, sizeof( int ) );
	fout.write( ( char* )&_tSlotOne._spellP1._nFireLevel, sizeof( int ) );
	fout.write( ( char* )&_tSlotOne._spellP1._nEarthXP, sizeof( int ) );
	fout.write( ( char* )&_tSlotOne._spellP1._nEarthLevel, sizeof( int ) );
	fout.write( ( char* )&_tSlotOne._spellP1._nIceXP, sizeof( int ) );
	fout.write( ( char* )&_tSlotOne._spellP1._nIceLevel, sizeof( int ) );
	fout.write( ( char* )&_tSlotOne._spellP1._nAirXP, sizeof( int ) );
	fout.write( ( char* )&_tSlotOne._spellP1._nAirLevel, sizeof( int ) );

	fout.write( ( char* )&_tSlotOne._nPlayerOneHealth, sizeof( int ) );

	if( 2 == _tSlotOne._nPlayerCount )
	{
		fout.write( ( char* )&_tSlotOne._nPlayerTwoHealth, sizeof( int ) );

		fout.write( ( char* )&_tSlotOne._nPlayerTwoPosX, sizeof( int ) );
		fout.write( ( char* )&_tSlotOne._nPlayerTwoPosY, sizeof( int ) );

		fout.write( ( char* )&_tSlotOne._manaP2._nFire, sizeof( int ) );
		fout.write( ( char* )&_tSlotOne._manaP2._nEarth, sizeof( int ) );
		fout.write( ( char* )&_tSlotOne._manaP2._nIce, sizeof( int ) );
		fout.write( ( char* )&_tSlotOne._manaP2._nAir, sizeof( int ) );

		fout.write( ( char* )&_tSlotOne._spellP2._nFireXP, sizeof( int ) );
		fout.write( ( char* )&_tSlotOne._spellP2._nFireLevel, sizeof( int ) );
		fout.write( ( char* )&_tSlotOne._spellP2._nEarthXP, sizeof( int ) );
		fout.write( ( char* )&_tSlotOne._spellP2._nEarthLevel, sizeof( int ) );
		fout.write( ( char* )&_tSlotOne._spellP2._nIceXP, sizeof( int ) );
		fout.write( ( char* )&_tSlotOne._spellP2._nIceLevel, sizeof( int ) );
		fout.write( ( char* )&_tSlotOne._spellP2._nAirXP, sizeof( int ) );
		fout.write( ( char* )&_tSlotOne._spellP2._nAirLevel, sizeof( int ) );
	}

	fout.write( ( char* )&_tSlotOne._nPlayerOneScore, sizeof( int ) );
	fout.write( ( char* )&_tSlotOne._nPlayerTwoScore, sizeof( int ) );

	if( _nSlot == 1 )
		pLevel->SaveCurrLevelState( &fout );
	else pLevel->SaveLevelFromMemory( &fout, m_tLevelInfo[0].szFileName, 
		&m_tLevelInfo[0].pTerrainTiles, &m_tLevelInfo[0].pEventTiles, &m_tLevelInfo[0].pSwitches );

	// - - - - - - - - - - - - - - 
	// Slot 2
	// - - - - - - - - - - - - - - 
	fout.write( ( char* )&_tSlotTwo._nPlayerCount, sizeof( int ) );
	fout.write( ( char* )&_tSlotTwo._nPositionX, sizeof( int ) );
	fout.write( ( char* )&_tSlotTwo._nPositionY, sizeof( int ) );

	fout.write( ( char* )&_tSlotTwo._manaP1._nFire, sizeof( int ) );
	fout.write( ( char* )&_tSlotTwo._manaP1._nEarth, sizeof( int ) );
	fout.write( ( char* )&_tSlotTwo._manaP1._nIce, sizeof( int ) );
	fout.write( ( char* )&_tSlotTwo._manaP1._nAir, sizeof( int ) );

	fout.write( ( char* )&_tSlotTwo._spellP1._nFireXP, sizeof( int ) );
	fout.write( ( char* )&_tSlotTwo._spellP1._nFireLevel, sizeof( int ) );
	fout.write( ( char* )&_tSlotTwo._spellP1._nEarthXP, sizeof( int ) );
	fout.write( ( char* )&_tSlotTwo._spellP1._nEarthLevel, sizeof( int ) );
	fout.write( ( char* )&_tSlotTwo._spellP1._nIceXP, sizeof( int ) );
	fout.write( ( char* )&_tSlotTwo._spellP1._nIceLevel, sizeof( int ) );
	fout.write( ( char* )&_tSlotTwo._spellP1._nAirXP, sizeof( int ) );
	fout.write( ( char* )&_tSlotTwo._spellP1._nAirLevel, sizeof( int ) );

	fout.write( ( char* )&_tSlotTwo._nPlayerOneHealth, sizeof( int ) );

	if( 2 == _tSlotTwo._nPlayerCount )
	{
		fout.write( ( char* )&_tSlotTwo._nPlayerTwoHealth, sizeof( int ) );

		fout.write( ( char* )&_tSlotTwo._nPlayerTwoPosX, sizeof( int ) );
		fout.write( ( char* )&_tSlotTwo._nPlayerTwoPosY, sizeof( int ) );

		fout.write( ( char* )&_tSlotTwo._manaP2._nFire, sizeof( int ) );
		fout.write( ( char* )&_tSlotTwo._manaP2._nEarth, sizeof( int ) );
		fout.write( ( char* )&_tSlotTwo._manaP2._nIce, sizeof( int ) );
		fout.write( ( char* )&_tSlotTwo._manaP2._nAir, sizeof( int ) );

		fout.write( ( char* )&_tSlotTwo._spellP2._nFireXP, sizeof( int ) );
		fout.write( ( char* )&_tSlotTwo._spellP2._nFireLevel, sizeof( int ) );
		fout.write( ( char* )&_tSlotTwo._spellP2._nEarthXP, sizeof( int ) );
		fout.write( ( char* )&_tSlotTwo._spellP2._nEarthLevel, sizeof( int ) );
		fout.write( ( char* )&_tSlotTwo._spellP2._nIceXP, sizeof( int ) );
		fout.write( ( char* )&_tSlotTwo._spellP2._nIceLevel, sizeof( int ) );
		fout.write( ( char* )&_tSlotTwo._spellP2._nAirXP, sizeof( int ) );
		fout.write( ( char* )&_tSlotTwo._spellP2._nAirLevel, sizeof( int ) );
	}

	fout.write( ( char* )&_tSlotTwo._nPlayerOneScore, sizeof( int ) );
	fout.write( ( char* )&_tSlotTwo._nPlayerTwoScore, sizeof( int ) );

	if( _nSlot == 2 )
		pLevel->SaveCurrLevelState( &fout );
	else pLevel->SaveLevelFromMemory( &fout, m_tLevelInfo[1].szFileName, 
		&m_tLevelInfo[1].pTerrainTiles, &m_tLevelInfo[1].pEventTiles, &m_tLevelInfo[1].pSwitches );

	// - - - - - - - - - - - - - - 
	// Slot 3 
	// - - - - - - - - - - - - - - 
	fout.write( ( char* )&_tSlotThree._nPlayerCount, sizeof( int ) );
	fout.write( ( char* )&_tSlotThree._nPositionX, sizeof( int ) );
	fout.write( ( char* )&_tSlotThree._nPositionY, sizeof( int ) );

	fout.write( ( char* )&_tSlotThree._manaP1._nFire, sizeof( int ) );
	fout.write( ( char* )&_tSlotThree._manaP1._nEarth, sizeof( int ) );
	fout.write( ( char* )&_tSlotThree._manaP1._nIce, sizeof( int ) );
	fout.write( ( char* )&_tSlotThree._manaP1._nAir, sizeof( int ) );

	fout.write( ( char* )&_tSlotThree._spellP1._nFireXP, sizeof( int ) );
	fout.write( ( char* )&_tSlotThree._spellP1._nFireLevel, sizeof( int ) );
	fout.write( ( char* )&_tSlotThree._spellP1._nEarthXP, sizeof( int ) );
	fout.write( ( char* )&_tSlotThree._spellP1._nEarthLevel, sizeof( int ) );
	fout.write( ( char* )&_tSlotThree._spellP1._nIceXP, sizeof( int ) );
	fout.write( ( char* )&_tSlotThree._spellP1._nIceLevel, sizeof( int ) );
	fout.write( ( char* )&_tSlotThree._spellP1._nAirXP, sizeof( int ) );
	fout.write( ( char* )&_tSlotThree._spellP1._nAirLevel, sizeof( int ) );

	fout.write( ( char* )&_tSlotThree._nPlayerOneHealth, sizeof( int ) );

	if( 2 == _tSlotThree._nPlayerCount )
	{
		fout.write( ( char* )&_tSlotThree._nPlayerTwoHealth, sizeof( int ) );

		fout.write( ( char* )&_tSlotThree._nPlayerTwoPosX, sizeof( int ) );
		fout.write( ( char* )&_tSlotThree._nPlayerTwoPosY, sizeof( int ) );

		fout.write( ( char* )&_tSlotThree._manaP2._nFire, sizeof( int ) );
		fout.write( ( char* )&_tSlotThree._manaP2._nEarth, sizeof( int ) );
		fout.write( ( char* )&_tSlotThree._manaP2._nIce, sizeof( int ) );
		fout.write( ( char* )&_tSlotThree._manaP2._nAir, sizeof( int ) );

		fout.write( ( char* )&_tSlotThree._spellP2._nFireXP, sizeof( int ) );
		fout.write( ( char* )&_tSlotThree._spellP2._nFireLevel, sizeof( int ) );
		fout.write( ( char* )&_tSlotThree._spellP2._nEarthXP, sizeof( int ) );
		fout.write( ( char* )&_tSlotThree._spellP2._nEarthLevel, sizeof( int ) );
		fout.write( ( char* )&_tSlotThree._spellP2._nIceXP, sizeof( int ) );
		fout.write( ( char* )&_tSlotThree._spellP2._nIceLevel, sizeof( int ) );
		fout.write( ( char* )&_tSlotThree._spellP2._nAirXP, sizeof( int ) );
		fout.write( ( char* )&_tSlotThree._spellP2._nAirLevel, sizeof( int ) );
	}

	fout.write( ( char* )&_tSlotThree._nPlayerOneScore, sizeof( int ) );
	fout.write( ( char* )&_tSlotThree._nPlayerTwoScore, sizeof( int ) );

	if( _nSlot == 3 )
		pLevel->SaveCurrLevelState( &fout );
	else pLevel->SaveLevelFromMemory( &fout, m_tLevelInfo[2].szFileName, 
		&m_tLevelInfo[2].pTerrainTiles, &m_tLevelInfo[2].pEventTiles, &m_tLevelInfo[2].pSwitches );

	fout.close( );

	for( int i = 0; i < 3; ++i )
	{
		delete[] m_tLevelInfo[i].szFileName;
		m_tLevelInfo[i].szFileName = 0;

		for( unsigned int j = 0; j < m_tLevelInfo[i].pTerrainTiles.size( ); ++j )
			delete m_tLevelInfo[i].pTerrainTiles[j];
		m_tLevelInfo[i].pTerrainTiles.clear( );

		for( unsigned int j = 0; j < m_tLevelInfo[i].pEventTiles.size( ); ++j )
			delete m_tLevelInfo[i].pEventTiles[j];
		m_tLevelInfo[i].pEventTiles.clear( );

		for( unsigned int j = 0; j < m_tLevelInfo[i].pSwitches.size( ); ++j )
			delete m_tLevelInfo[i].pSwitches[j];
		m_tLevelInfo[i].pSwitches.clear( );
	}

	return true;
}