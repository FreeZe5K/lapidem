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
				m_nChoice = 3;
		}

		if( m_pDI->KeyPressed( DIK_DOWN ) || m_pDI->JoystickDPadPressed(3) )
		{
			if( ++m_nChoice > 3 )
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
				//SaveGame( "resource/data/config.xml" );				m_nChoice  = 0;
				m_nState   = 2;
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

		if(m_pDI->KeyPressed( DIK_UP ) || m_pDI->JoystickDPadPressed( 2 ) )
		{
			if( --m_nChoice < 0 ) 
				m_nChoice = 2;
		}

		if(m_pDI->KeyPressed( DIK_DOWN ) || m_pDI->JoystickDPadPressed( 3 ) )
		{
			if( ++m_nChoice > 2 )
				m_nChoice = 0;
		}

		if(m_pDI->KeyDown( DIK_LEFT ) || m_pDI->JoystickDPadDown( 0 ) )
		{
			if( m_nChoice == 0 )
				CGame::GetInstance( )->SetMusicVolume( 
				CGame::GetInstance( )->GetMusicVolume( ) - 1 );

			else if( m_nChoice == 1 )
				CGame::GetInstance( )->SetSoundFXVolume( 
				CGame::GetInstance( )->GetSoundFXVolume( ) - 1 );
		}

		if( m_pDI->KeyDown( DIK_RIGHT ) || m_pDI->JoystickDPadDown(1) )
		{
			if( m_nChoice == 0 )
				CGame::GetInstance( )->SetMusicVolume( 
				CGame::GetInstance( )->GetMusicVolume( ) + 1 );

			else if( m_nChoice == 1 )
				CGame::GetInstance( )->SetSoundFXVolume( 
				CGame::GetInstance( )->GetSoundFXVolume( ) + 1 );
		}

		if( m_pDI->KeyPressed( DIK_RETURN ) || m_pDI->JoystickButtonPressed(1) )
		{
			if( m_nChoice == 2 )
			{
				CAuxiliaryState::GetInstance( )->SaveConfig
					( "resource/data/Lapidem_Config.dat" );
				m_nState   = 0; // Done
				m_nChoice  = 0;
			}
		}
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
			CGame::GetInstance( )->GetFont( )->Draw( ">", 150, 96, 
				1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "RESUME", 180, 100, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "OPTIONS", 170, 120, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SAVE AND QUIT", 170, 140, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "RAGE QUIT", 170, 160, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 1 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 150, 116, 
				1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "RESUME", 170, 100, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "OPTIONS", 180, 120, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SAVE AND QUIT", 170, 140, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "RAGE QUIT", 170, 160, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 2 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 150, 136, 
				1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "RESUME", 170, 100, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "OPTIONS", 170, 120, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SAVE AND QUIT", 180, 140, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "RAGE QUIT", 170, 160, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 3 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 150, 156, 
				1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "RESUME", 170, 100, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "OPTIONS", 170, 120, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SAVE AND QUIT", 170, 140, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "RAGE QUIT", 180, 160, 
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
			CGame::GetInstance( )->GetFont( )->Draw( ">", 150, 96, 
				1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "MUSIC", 180, 100, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetMusicVolume( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 350, 100, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "SOUND FX", 170, 120, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetSoundFXVolume( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 350, 120, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "DONE", 170, 140, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 1 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 150, 116, 
				1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "MUSIC", 170, 100, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetMusicVolume( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 350, 100, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "SOUND FX", 180, 120, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetSoundFXVolume( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 350, 120, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "DONE", 170, 140, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 2 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 150, 136, 
				1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "MUSIC", 170, 100, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetMusicVolume( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 350, 100, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "SOUND FX", 170, 120, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			sprintf_s( cBuffer, "%i", CGame::GetInstance( )->GetSoundFXVolume( ) );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 350, 120, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			CGame::GetInstance( )->GetFont( )->Draw( "DONE", 180, 140, 
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
			CGame::GetInstance( )->GetFont( )->Draw( ">", 150, 96, 
				1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 1", 180, 100, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 2", 170, 120, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 3", 170, 140, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "BACK", 170, 160, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 1 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 150, 116, 
				1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 1", 170, 100, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 2", 180, 120, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 3", 170, 140, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "BACK", 170, 160, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 2 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 150, 136, 
				1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 1", 170, 100, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 2", 170, 120, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 3", 180, 140, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "BACK", 170, 160, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 3 )
		{
			CGame::GetInstance( )->GetFont( )->Draw( ">", 150, 156, 
				1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 1", 170, 100, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 2", 170, 120, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "SLOT 3", 170, 140, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( "BACK", 180, 160, 
				0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
	}
}

void CPauseMenuState::Exit( )
{
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
		}

		_tSlotOne._manaP1._nFire      = CGameplayState::GetInstance( )->GetPlayerOne( )->GetFireEnergy( );
		_tSlotOne._manaP1._nEarth     = CGameplayState::GetInstance( )->GetPlayerOne( )->GetEarthEnergy( );
		_tSlotOne._manaP1._nIce       = CGameplayState::GetInstance( )->GetPlayerOne( )->GetIceEnergy( );
		_tSlotOne._manaP1._nAir       = CGameplayState::GetInstance( )->GetPlayerOne( )->GetWindEnergy( );
		
		// TODO ::
		//      ^ Probably going to crash...
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
		}

		_tSlotTwo._manaP1._nFire      = CGameplayState::GetInstance( )->GetPlayerOne( )->GetFireEnergy( );
		_tSlotTwo._manaP1._nEarth     = CGameplayState::GetInstance( )->GetPlayerOne( )->GetEarthEnergy( );
		_tSlotTwo._manaP1._nIce       = CGameplayState::GetInstance( )->GetPlayerOne( )->GetIceEnergy( );
		_tSlotTwo._manaP1._nAir       = CGameplayState::GetInstance( )->GetPlayerOne( )->GetWindEnergy( );
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
		}

		_tSlotThree._manaP1._nFire      = CGameplayState::GetInstance( )->GetPlayerOne( )->GetFireEnergy( );
		_tSlotThree._manaP1._nEarth     = CGameplayState::GetInstance( )->GetPlayerOne( )->GetEarthEnergy( );
		_tSlotThree._manaP1._nIce       = CGameplayState::GetInstance( )->GetPlayerOne( )->GetIceEnergy( );
		_tSlotThree._manaP1._nAir       = CGameplayState::GetInstance( )->GetPlayerOne( )->GetWindEnergy( );
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