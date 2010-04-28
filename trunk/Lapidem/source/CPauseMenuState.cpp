//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CPauseMenuState.cpp"
//
//  Author Name :   Mike Hatter
//
//  Purpose     :   
//////////////////////////////////////////////////////////////////////////
#include "CPauseMenuState.h"

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
		if( m_pDI->KeyPressed( DIK_UP ) )
		{
			if( --m_nChoice < 0 ) 
				m_nChoice = 3;
		}

		if( m_pDI->KeyPressed( DIK_DOWN ) )
		{
			if( ++m_nChoice > 3 )
				m_nChoice = 0;
		}

		if( m_pDI->KeyPressed( DIK_P ) )
		{
			CGame::GetInstance( )->PopState( );
			CGame::GetInstance( )->SetPaused( false );
		}

		if( m_pDI->KeyPressed( DIK_RETURN ) )
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
				//SaveGame( "resource/data/config.xml" );
				m_nChoice  = 0;
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

		m_pWM->SetVolume( CGameplayState::GetInstance( )->GetMusic( ), 
			CGame::GetInstance( )->GetMusicVolume( ) ); 
		m_pWM->SetVolume( CGameplayState::GetInstance( )->GetSoundFX( ), 
			CGame::GetInstance( )->GetSoundFXVolume( ) ); 

		if( m_pDI->KeyPressed( DIK_UP ) )
		{
			if( --m_nChoice < 0 ) 
				m_nChoice = 2;
		}

		if( m_pDI->KeyPressed( DIK_DOWN ) )
		{
			if( ++m_nChoice > 2 )
				m_nChoice = 0;
		}

		if( m_pDI->KeyDown( DIK_LEFT ) )
		{
			if( m_nChoice == 0 )
				CGame::GetInstance( )->SetMusicVolume( 
				CGame::GetInstance( )->GetMusicVolume( ) - 1 );

			else if( m_nChoice == 1 )
				CGame::GetInstance( )->SetSoundFXVolume( 
				CGame::GetInstance( )->GetSoundFXVolume( ) - 1 );
		}

		if( m_pDI->KeyDown( DIK_RIGHT ) )
		{
			if( m_nChoice == 0 )
				CGame::GetInstance( )->SetMusicVolume( 
				CGame::GetInstance( )->GetMusicVolume( ) + 1 );

			else if( m_nChoice == 1 )
				CGame::GetInstance( )->SetSoundFXVolume( 
				CGame::GetInstance( )->GetSoundFXVolume( ) + 1 );
		}

		if( m_pDI->KeyPressed( DIK_RETURN ) )
		{
			if( m_nChoice == 2 )
			{
				m_nState   = 0; // Done
				m_nChoice  = 0;
			}
		}
	}
	else if( 2 == m_nState ) // Save game
	{
		if( m_pDI->KeyPressed( DIK_UP ) )
		{
			if( --m_nChoice < 0 ) 
				m_nChoice = 3;
		}

		if( m_pDI->KeyPressed( DIK_DOWN ) )
		{
			if( ++m_nChoice > 3 )
				m_nChoice = 0;
		}

		if( m_pDI->KeyPressed( DIK_RETURN ) )
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
	// - - - - - - - - - - - - - - -
	// Needed for sprint 2 checkoff
	// - - - - - - - - - - - - - - -
	// Location
	// Score
	// Co-op or single player
	// 
	// Load game
	//
	// 3 slots are available
	// - - - - - - - - - - - - - - -

	// Slot                                // - 
	// Single player or multi-player       // - 
	// Level                               // 
	// Player health                       // 
	// Player position                     // 
	// Player score                        // 
	// Time left                           // 
	// Currently equipped ability          // 
	// Fire mana                           // 
	// Earth mana                          // 
	// Wind mana                           // 
	// Ice mana                            // 
	// 
	// Pablo :: 
	//       Save the state of all the blocks
	// GetLevelFileName( )          :: CLevel
	//

	// - - - - - - - - - - - - - -
	// Read in slot 1 / 2 / 3 info.
	// - - - - - - - - - - - - - -
	char _saveSlot[128] = "resource/data/Lapidem_GameSaves.bin";
	ifstream fin( _saveSlot, std::ios_base::in | std::ios_base::binary );

	if( fin.is_open( ) )
	{
		fin.read( ( char* )&_tSlotOne._nPlayerCount, sizeof( int ) );
		fin.read( ( char* )&_tSlotOne._nPositionX, sizeof( int ) );
		fin.read( ( char* )&_tSlotOne._nPositionY, sizeof( int ) );
		fin.read( ( char* )&_tSlotOne._nSinglePlayerScore, sizeof( int ) );
		fin.read( ( char* )&_tSlotOne._nPlayerOneScore, sizeof( int ) );
		fin.read( ( char* )&_tSlotOne._nPlayerTwoScore, sizeof( int ) );

		fin.read( ( char* )&_tSlotTwo._nPlayerCount, sizeof( int ) );
		fin.read( ( char* )&_tSlotTwo._nPositionX, sizeof( int ) );
		fin.read( ( char* )&_tSlotTwo._nPositionY, sizeof( int ) );
		fin.read( ( char* )&_tSlotTwo._nSinglePlayerScore, sizeof( int ) );
		fin.read( ( char* )&_tSlotTwo._nPlayerOneScore, sizeof( int ) );
		fin.read( ( char* )&_tSlotTwo._nPlayerTwoScore, sizeof( int ) );

		fin.read( ( char* )&_tSlotThree._nPlayerCount, sizeof( int ) );
		fin.read( ( char* )&_tSlotThree._nPositionX, sizeof( int ) );
		fin.read( ( char* )&_tSlotThree._nPositionY, sizeof( int ) );
		fin.read( ( char* )&_tSlotThree._nSinglePlayerScore, sizeof( int ) );
		fin.read( ( char* )&_tSlotThree._nPlayerOneScore, sizeof( int ) );
		fin.read( ( char* )&_tSlotThree._nPlayerTwoScore, sizeof( int ) );
	} fin.close( );

	// - - - - - - - - - - - - - -
	// Change the current slot info.
	// - - - - - - - - - - - - - -
	if( 1 == _nSlot )
	{
		if( CMenuState::GetInstance( )->GetPlayerCount( ) == 1 )
			_tSlotOne._nPlayerCount = 1;
		else _tSlotOne._nPlayerCount = 2;

		_tSlotOne._nPositionX = int( CGameplayState::GetInstance( )->GetPlayerOne( )->GetPosX( ) );
		_tSlotOne._nPositionY = int( CGameplayState::GetInstance( )->GetPlayerOne( )->GetPosY( ) );

		_tSlotOne._nSinglePlayerScore = CGameplayState::GetInstance( )->GetSinglePlayerScore( );
		_tSlotOne._nPlayerOneScore    = CGameplayState::GetInstance( )->GetPlayerOneScore( );
		_tSlotOne._nPlayerTwoScore    = CGameplayState::GetInstance( )->GetPlayerTwoScore( );
	}
	else if( 2 == _nSlot )
	{
		if( CMenuState::GetInstance( )->GetPlayerCount( ) == 1 )
			_tSlotTwo._nPlayerCount = 1;
		else _tSlotTwo._nPlayerCount = 2;

		_tSlotTwo._nPositionX = int( CGameplayState::GetInstance( )->GetPlayerOne( )->GetPosX( ) );
		_tSlotTwo._nPositionY = int( CGameplayState::GetInstance( )->GetPlayerOne( )->GetPosY( ) );

		_tSlotTwo._nSinglePlayerScore = CGameplayState::GetInstance( )->GetSinglePlayerScore( );
		_tSlotTwo._nPlayerOneScore    = CGameplayState::GetInstance( )->GetPlayerOneScore( );
		_tSlotTwo._nPlayerTwoScore    = CGameplayState::GetInstance( )->GetPlayerTwoScore( );
	}
	else if( 3 == _nSlot )
	{
		if( CMenuState::GetInstance( )->GetPlayerCount( ) == 1 )
			_tSlotThree._nPlayerCount = 1;
		else _tSlotThree._nPlayerCount = 2;

		_tSlotThree._nPositionX = int( CGameplayState::GetInstance( )->GetPlayerOne( )->GetPosX( ) );
		_tSlotThree._nPositionY = int( CGameplayState::GetInstance( )->GetPlayerOne( )->GetPosY( ) );

		_tSlotThree._nSinglePlayerScore = CGameplayState::GetInstance( )->GetSinglePlayerScore( );
		_tSlotThree._nPlayerOneScore    = CGameplayState::GetInstance( )->GetPlayerOneScore( );
		_tSlotThree._nPlayerTwoScore    = CGameplayState::GetInstance( )->GetPlayerTwoScore( );
	}

	// - - - - - - - - - - - - - -
	// Save all slots.
	// - - - - - - - - - - - - - -
	ofstream fout( _saveSlot, std::ios_base::out | std::ios::trunc | std::ios::binary );

	fout.write( ( char* )&_tSlotOne._nPlayerCount, sizeof( int ) );
	fout.write( ( char* )&_tSlotOne._nPositionX, sizeof( int ) );
	fout.write( ( char* )&_tSlotOne._nPositionY, sizeof( int ) );
	fout.write( ( char* )&_tSlotOne._nSinglePlayerScore, sizeof( int ) );
	fout.write( ( char* )&_tSlotOne._nPlayerOneScore, sizeof( int ) );
	fout.write( ( char* )&_tSlotOne._nPlayerTwoScore, sizeof( int ) );

	fout.write( ( char* )&_tSlotTwo._nPlayerCount, sizeof( int ) );
	fout.write( ( char* )&_tSlotTwo._nPositionX, sizeof( int ) );
	fout.write( ( char* )&_tSlotTwo._nPositionY, sizeof( int ) );
	fout.write( ( char* )&_tSlotTwo._nSinglePlayerScore, sizeof( int ) );
	fout.write( ( char* )&_tSlotTwo._nPlayerOneScore, sizeof( int ) );
	fout.write( ( char* )&_tSlotTwo._nPlayerTwoScore, sizeof( int ) );

	fout.write( ( char* )&_tSlotThree._nPlayerCount, sizeof( int ) );
	fout.write( ( char* )&_tSlotThree._nPositionX, sizeof( int ) );
	fout.write( ( char* )&_tSlotThree._nPositionY, sizeof( int ) );
	fout.write( ( char* )&_tSlotThree._nSinglePlayerScore, sizeof( int ) );
	fout.write( ( char* )&_tSlotThree._nPlayerOneScore, sizeof( int ) );
	fout.write( ( char* )&_tSlotThree._nPlayerTwoScore, sizeof( int ) );

	fout.close( );

	return true;
}