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
	m_pD3D          = CSGD_Direct3D::GetInstance( );
	m_pTM           = CSGD_TextureManager::GetInstance( );
	m_pDS           = CSGD_DirectSound::GetInstance( );
	m_pWM           = CSGD_WaveManager::GetInstance( );
	m_pDI           = CSGD_DirectInput::GetInstance( );

	m_nState        = 0;
	m_nChoice       = 0;
	m_nAttractTimer = 0;
	m_nPlayerCount  = 1;

	m_fInputTimer = 0.0f;

	m_nImageID      = m_pTM->LoadTexture( "resource/graphics/Lapidem_MainMenuBG.png" );

	m_pWM->SetVolume( CGame::GetInstance( )->GetMainMenuMusic( ), 
			CGame::GetInstance( )->GetMusicVolume( ) ); 
		m_pWM->SetVolume( CGame::GetInstance( )->GetMenuTick( ), 
			CGame::GetInstance( )->GetSoundFXVolume( ) );  

	if( !m_pWM->IsWavePlaying( CGame::GetInstance( )->GetMainMenuMusic( ) ) )
		m_pWM->Play( CGame::GetInstance( )->GetMainMenuMusic( ), DSBPLAY_LOOPING );

	CGameplayState::GetInstance( )->SetPlayerReachedEnd( false );
	CAuxiliaryState::GetInstance( )->LoadSlotInfo( "resource/data/Lapidem_SlotInfo.bin" );
}

bool CMenuState::Input( )
{
	m_fInputTimer += .0167f;

	if(m_pDI->JoystickGetLStickYNormalized())
	{
		if(m_fInputTimer < .15)
			return true;
		else
			m_fInputTimer = 0.0f;
	}

	if( 0 == m_nState ) // Main menu
	{
		if( m_pDI->KeyPressed( DIK_UP )  || m_pDI->JoystickDPadPressed( 2 ) || m_pDI->JoystickGetLStickYNormalized() < 0 )
		{
			m_nAttractTimer = 0;
			m_pWM->Play( CGame::GetInstance( )->GetMenuTick( ) );

			if( --m_nChoice < 0 ) 
				m_nChoice = 5;
		}

		if( m_pDI->KeyPressed( DIK_DOWN )  || m_pDI->JoystickDPadPressed( 3 ) || m_pDI->JoystickGetLStickYNormalized() > 0 )
		{
			m_nAttractTimer = 0;
			m_pWM->Play( CGame::GetInstance( )->GetMenuTick( ) );

			if( ++m_nChoice > 5 )
				m_nChoice = 0;
		}

		if( m_pDI->KeyPressed( DIK_RETURN )  || m_pDI->JoystickButtonPressed( 0 ) || m_pDI->JoystickButtonPressed( 1 ))
		{
			m_nAttractTimer = 0;

			if( m_nChoice == 0 )      // Play
				m_nState = 1;
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
	}
	else if( 1 == m_nState ) // New / load
	{
		if( m_pDI->KeyPressed( DIK_UP )  || m_pDI->JoystickDPadPressed( 2 ) || m_pDI->JoystickGetLStickYNormalized() < 0)
		{
			m_nAttractTimer = 0;
			m_pWM->Play( CGame::GetInstance( )->GetMenuTick( ) );

			if( --m_nChoice < 0 )
				m_nChoice = 2;
		}

		if( m_pDI->KeyPressed( DIK_DOWN )  || m_pDI->JoystickDPadPressed( 3 ) || m_pDI->JoystickGetLStickYNormalized() > 0 )
		{
			m_nAttractTimer = 0;
			m_pWM->Play( CGame::GetInstance( )->GetMenuTick( ) );

			if( ++m_nChoice > 2 )
				m_nChoice = 0;
		}

		if( m_pDI->KeyPressed( DIK_RETURN )  || m_pDI->JoystickButtonPressed( 0 ) || m_pDI->JoystickButtonPressed( 1 ) )
		{
			m_nAttractTimer = 0;

			if( m_nChoice == 0 )      // New Game
			{
				m_nChoice = 0;
				m_nState = 2;
			}
			else if( m_nChoice == 1 ) // Load Game
			{
				m_nChoice = 0;
				m_nState = 3;         // - Which Slot
			}
			else if( m_nChoice == 2 ) // Cancel
			{
				m_nChoice = 0;
				m_nState = 0;
			}
		}
	}
	else if( 2 == m_nState ) // Single / mutliplayer
	{
		if( m_pDI->KeyPressed( DIK_UP )  || m_pDI->JoystickDPadPressed( 2 ) || m_pDI->JoystickGetLStickYAmount() < 0 )
		{
			m_nAttractTimer = 0;
			m_pWM->Play( CGame::GetInstance( )->GetMenuTick( ) );

			if( --m_nChoice < 0 )
				m_nChoice = 2;
		}

		if( m_pDI->KeyPressed( DIK_DOWN )  || m_pDI->JoystickDPadPressed( 3 ) || m_pDI->JoystickGetLStickYAmount() > 0 )
		{
			m_nAttractTimer = 0;
			m_pWM->Play( CGame::GetInstance( )->GetMenuTick( ) );

			if( ++m_nChoice > 2 )
				m_nChoice = 0;
		}

		if( m_pDI->KeyPressed( DIK_RETURN )  || m_pDI->JoystickButtonPressed( 0 ) || m_pDI->JoystickButtonPressed( 1 ) )
		{
			m_nAttractTimer = 0;

			if( m_nChoice == 0 ) // Single Player
			{
				m_nPlayerCount = 1;
				CGame::GetInstance( )->ChangeState( CGameplayState::GetInstance( ) );
			}
			else if( m_nChoice == 1 ) // Two Player
			{
				m_nPlayerCount = 2; 
				CGameplayState::GetInstance( )->bTwoPlayerMode( true );
				CGame::GetInstance( )->ChangeState( CGameplayState::GetInstance( ) );
			}
			else if( m_nChoice == 2 ) // Cancel
			{
				m_nState = 1;
				m_nChoice = 0;
			}
		}
	}
	else if( 3 == m_nState )
	{
		if( m_pDI->KeyPressed( DIK_UP )  || m_pDI->JoystickDPadPressed( 2 ) || m_pDI->JoystickGetLStickYAmount() < 0)
		{
			m_nAttractTimer = 0;
			m_pWM->Play( CGame::GetInstance( )->GetMenuTick( ) );

			if( --m_nChoice < 0 )
				m_nChoice = 3;
		}

		if( m_pDI->KeyPressed( DIK_DOWN )  || m_pDI->JoystickDPadPressed( 3 ) || m_pDI->JoystickGetLStickYAmount() > 0 )
		{
			m_nAttractTimer = 0;
			m_pWM->Play( CGame::GetInstance( )->GetMenuTick( ) );

			if( ++m_nChoice > 3 )
				m_nChoice = 0;
		}

		if( m_pDI->KeyPressed( DIK_RETURN )  || m_pDI->JoystickButtonPressed( 0 ) || m_pDI->JoystickButtonPressed( 1 ) )
		{
			m_nAttractTimer = 0;

			if( 0 == m_nChoice ) // Load Slot 1
			{ /* TODO :: LOAD SLOT 1 */ 
				if(!Load( 1 ))
					return true;
				CGameplayState::GetInstance( )->SetSlotLoaded( 1 );
				CGame::GetInstance( )->ChangeState( CGameplayState::GetInstance( ) );
			}
			else if( 1 == m_nChoice ) // Load Slot 2
			{ /* TODO :: LOAD SLOT 1 */ 
				if(!Load( 2 ))
					return true;
				CGameplayState::GetInstance( )->SetSlotLoaded( 2 );
				CGame::GetInstance( )->ChangeState( CGameplayState::GetInstance( ) );
			}
			else if( 2 == m_nChoice ) // Load Slot 3
			{ /* TODO :: LOAD SLOT 1 */ 
				if(!Load( 3 ))
					return true;
				CGameplayState::GetInstance( )->SetSlotLoaded( 3 );
				CGame::GetInstance( )->ChangeState( CGameplayState::GetInstance( ) );
			}			
			else if( 3 == m_nChoice ) // Cancel
			{
				m_nChoice = 0;
				m_nState = 1;
			}
		}
	}

	return true;
}

void CMenuState::Update( float fET )
{
	// If 10 seconds have passed without input,
	// attract the user to play!
	if( m_nAttractTimer > 1000 )
	{
		m_nAttractTimer = 0;
		CGame::GetInstance( )->PushState( CAttractState::GetInstance( ) );
	} else ++m_nAttractTimer;
}

void CMenuState::Render( )
{
	m_pTM->Draw( m_nImageID, 0, 0, 1.0f, 1.0f, 
		NULL, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB( 200, 255, 255, 255 ) );

	if( 0 == m_nState )
	{
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
	}
	else if( 1 == m_nState ) // New or load
	{
		if( m_nChoice == 0 ) 
		{
			CGame::GetInstance( ) ->GetFont( )->Draw( ">", 50, 96, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( ) ->GetFont( )->Draw( "NEW GAME", 80, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( ) ->GetFont( )->Draw( "LOAD GAME", 70, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( ) ->GetFont( )->Draw( "CANCEL", 70, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 1 )
		{
			CGame::GetInstance( ) ->GetFont( )->Draw( ">", 50, 116, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( ) ->GetFont( )->Draw( "NEW GAME", 70, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( ) ->GetFont( )->Draw( "LOAD GAME", 80, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( ) ->GetFont( )->Draw( "CANCEL", 70, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 2 )
		{
			CGame::GetInstance( ) ->GetFont( )->Draw( ">", 50, 136, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( ) ->GetFont( )->Draw( "NEW GAME", 70, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( ) ->GetFont( )->Draw( "LOAD GAME", 70, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( ) ->GetFont( )->Draw( "CANCEL", 80, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
	}
	else if( 2 == m_nState ) // Single or multi-player
	{
		if( m_nChoice == 0 ) 
		{
			CGame::GetInstance( ) ->GetFont( )->Draw( ">", 50, 96, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( ) ->GetFont( )->Draw( "SINGLE PLAYER", 80, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( ) ->GetFont( )->Draw( "TWO PLAYER", 70, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( ) ->GetFont( )->Draw( "CANCEL", 70, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 1 )
		{
			CGame::GetInstance( ) ->GetFont( )->Draw( ">", 50, 116, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( ) ->GetFont( )->Draw( "SINGLE PLAYER", 70, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( ) ->GetFont( )->Draw( "TWO PLAYER", 80, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( ) ->GetFont( )->Draw( "CANCEL", 70, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 2 )
		{
			CGame::GetInstance( ) ->GetFont( )->Draw( ">", 50, 136, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( ) ->GetFont( )->Draw( "SINGLE PLAYER", 70, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( ) ->GetFont( )->Draw( "TWO PLAYER", 70, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( ) ->GetFont( )->Draw( "CANCEL", 80, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
	}
	else if( 3 == m_nState ) // Load which slot
	{
		if( m_nChoice == 0 ) 
		{
			CGame::GetInstance( ) ->GetFont( )->Draw( ">", 50, 96, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( ) ->GetFont( )->Draw( "SLOT 1", 80, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			
			if( CAuxiliaryState::GetInstance( )->GetSlotInfoOne( )._nCurrentLevel == 0 )
				CGame::GetInstance( ) ->GetFont( )->Draw( "( SLOT EMPTY )", 210, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			else
			{
				if( CAuxiliaryState::GetInstance( )->GetSlotInfoOne( )._nNumPlayers == 1 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "( 1P,", 210, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoOne( )._nNumPlayers == 2 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "( 2P,", 210, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				
				if( CAuxiliaryState::GetInstance( )->GetSlotInfoOne( )._nCurrentLevel == 1 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 1 )", 295, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoOne( )._nCurrentLevel == 2 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 2 )", 295, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoOne( )._nCurrentLevel == 3 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 3 )", 295, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}

			CGame::GetInstance( ) ->GetFont( )->Draw( "SLOT 2", 70, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			
			if( CAuxiliaryState::GetInstance( )->GetSlotInfoTwo( )._nCurrentLevel == 0 )
				CGame::GetInstance( ) ->GetFont( )->Draw( "( SLOT EMPTY )", 210, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			else
			{
				if( CAuxiliaryState::GetInstance( )->GetSlotInfoTwo( )._nNumPlayers == 1 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "( 1P,", 210, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoTwo( )._nNumPlayers == 2 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "( 2P,", 210, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				
				if( CAuxiliaryState::GetInstance( )->GetSlotInfoTwo( )._nCurrentLevel == 1 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 1 )", 295, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoTwo( )._nCurrentLevel == 2 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 2 )", 295, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoTwo( )._nCurrentLevel == 3 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 3 )", 295, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}

			CGame::GetInstance( ) ->GetFont( )->Draw( "SLOT 3", 70, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			
			if( CAuxiliaryState::GetInstance( )->GetSlotInfoThree( )._nCurrentLevel == 0 )
				CGame::GetInstance( ) ->GetFont( )->Draw( "( SLOT EMPTY )", 210, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			else
			{
				if( CAuxiliaryState::GetInstance( )->GetSlotInfoThree( )._nNumPlayers == 1 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "( 1P,", 210, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoThree( )._nNumPlayers == 2 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "( 2P,", 210, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				
				if( CAuxiliaryState::GetInstance( )->GetSlotInfoThree( )._nCurrentLevel == 1 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 1 )", 295, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoThree( )._nCurrentLevel == 2 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 2 )", 295, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoThree( )._nCurrentLevel == 3 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 3 )", 295, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}

			CGame::GetInstance( ) ->GetFont( )->Draw( "CANCEL", 70, 160, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 1 )
		{
			CGame::GetInstance( ) ->GetFont( )->Draw( ">", 50, 116, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( ) ->GetFont( )->Draw( "SLOT 1", 70, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			
			if( CAuxiliaryState::GetInstance( )->GetSlotInfoOne( )._nCurrentLevel == 0 )
				CGame::GetInstance( ) ->GetFont( )->Draw( "( SLOT EMPTY )", 210, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			else
			{
				if( CAuxiliaryState::GetInstance( )->GetSlotInfoOne( )._nNumPlayers == 1 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "( 1P,", 210, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoOne( )._nNumPlayers == 2 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "( 2P,", 210, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				
				if( CAuxiliaryState::GetInstance( )->GetSlotInfoOne( )._nCurrentLevel == 1 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 1 )", 295, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoOne( )._nCurrentLevel == 2 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 2 )", 295, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoOne( )._nCurrentLevel == 3 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 3 )", 295, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}

			CGame::GetInstance( ) ->GetFont( )->Draw( "SLOT 2", 80, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			
			if( CAuxiliaryState::GetInstance( )->GetSlotInfoTwo( )._nCurrentLevel == 0 )
				CGame::GetInstance( ) ->GetFont( )->Draw( "( SLOT EMPTY )", 210, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			else
			{
				if( CAuxiliaryState::GetInstance( )->GetSlotInfoTwo( )._nNumPlayers == 1 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "( 1P,", 210, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoTwo( )._nNumPlayers == 2 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "( 2P,", 210, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				
				if( CAuxiliaryState::GetInstance( )->GetSlotInfoTwo( )._nCurrentLevel == 1 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 1 )", 295, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoTwo( )._nCurrentLevel == 2 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 2 )", 295, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoTwo( )._nCurrentLevel == 3 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 3 )", 295, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}

			CGame::GetInstance( ) ->GetFont( )->Draw( "SLOT 3", 70, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			
			if( CAuxiliaryState::GetInstance( )->GetSlotInfoThree( )._nCurrentLevel == 0 )
				CGame::GetInstance( ) ->GetFont( )->Draw( "( SLOT EMPTY )", 210, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			else
			{
				if( CAuxiliaryState::GetInstance( )->GetSlotInfoThree( )._nNumPlayers == 1 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "( 1P,", 210, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoThree( )._nNumPlayers == 2 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "( 2P,", 210, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				
				if( CAuxiliaryState::GetInstance( )->GetSlotInfoThree( )._nCurrentLevel == 1 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 1 )", 295, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoThree( )._nCurrentLevel == 2 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 2 )", 295, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoThree( )._nCurrentLevel == 3 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 3 )", 295, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}

			CGame::GetInstance( ) ->GetFont( )->Draw( "CANCEL", 70, 160, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 2 )
		{
			CGame::GetInstance( ) ->GetFont( )->Draw( ">", 50, 136, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( ) ->GetFont( )->Draw( "SLOT 1", 70, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			
			if( CAuxiliaryState::GetInstance( )->GetSlotInfoOne( )._nCurrentLevel == 0 )
				CGame::GetInstance( ) ->GetFont( )->Draw( "( SLOT EMPTY )", 210, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			else
			{
				if( CAuxiliaryState::GetInstance( )->GetSlotInfoOne( )._nNumPlayers == 1 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "( 1P,", 210, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoOne( )._nNumPlayers == 2 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "( 2P,", 210, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				
				if( CAuxiliaryState::GetInstance( )->GetSlotInfoOne( )._nCurrentLevel == 1 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 1 )", 295, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoOne( )._nCurrentLevel == 2 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 2 )", 295, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoOne( )._nCurrentLevel == 3 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 3 )", 295, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}

			CGame::GetInstance( ) ->GetFont( )->Draw( "SLOT 2", 70, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			
			if( CAuxiliaryState::GetInstance( )->GetSlotInfoTwo( )._nCurrentLevel == 0 )
				CGame::GetInstance( ) ->GetFont( )->Draw( "( SLOT EMPTY )", 210, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			else
			{
				if( CAuxiliaryState::GetInstance( )->GetSlotInfoTwo( )._nNumPlayers == 1 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "( 1P,", 210, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoTwo( )._nNumPlayers == 2 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "( 2P,", 210, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				
				if( CAuxiliaryState::GetInstance( )->GetSlotInfoTwo( )._nCurrentLevel == 1 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 1 )", 295, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoTwo( )._nCurrentLevel == 2 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 2 )", 295, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoTwo( )._nCurrentLevel == 3 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 3 )", 295, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}

			CGame::GetInstance( ) ->GetFont( )->Draw( "SLOT 3", 80, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			
			if( CAuxiliaryState::GetInstance( )->GetSlotInfoThree( )._nCurrentLevel == 0 )
				CGame::GetInstance( ) ->GetFont( )->Draw( "( SLOT EMPTY )", 210, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			else
			{
				if( CAuxiliaryState::GetInstance( )->GetSlotInfoThree( )._nNumPlayers == 1 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "( 1P,", 210, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoThree( )._nNumPlayers == 2 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "( 2P,", 210, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				
				if( CAuxiliaryState::GetInstance( )->GetSlotInfoThree( )._nCurrentLevel == 1 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 1 )", 295, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoThree( )._nCurrentLevel == 2 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 2 )", 295, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoThree( )._nCurrentLevel == 3 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 3 )", 295, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}

			CGame::GetInstance( ) ->GetFont( )->Draw( "CANCEL", 70, 160, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( m_nChoice == 3 )
		{
			CGame::GetInstance( ) ->GetFont( )->Draw( ">", 50, 156, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( ) ->GetFont( )->Draw( "SLOT 1", 70, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			
			if( CAuxiliaryState::GetInstance( )->GetSlotInfoOne( )._nCurrentLevel == 0 )
				CGame::GetInstance( ) ->GetFont( )->Draw( "( SLOT EMPTY )", 210, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			else
			{
				if( CAuxiliaryState::GetInstance( )->GetSlotInfoOne( )._nNumPlayers == 1 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "( 1P,", 210, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoOne( )._nNumPlayers == 2 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "( 2P,", 210, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				
				if( CAuxiliaryState::GetInstance( )->GetSlotInfoOne( )._nCurrentLevel == 1 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 1 )", 295, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoOne( )._nCurrentLevel == 2 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 2 )", 295, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoOne( )._nCurrentLevel == 3 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 3 )", 295, 100, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}

			CGame::GetInstance( ) ->GetFont( )->Draw( "SLOT 2", 70, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			
			if( CAuxiliaryState::GetInstance( )->GetSlotInfoTwo( )._nCurrentLevel == 0 )
				CGame::GetInstance( ) ->GetFont( )->Draw( "( SLOT EMPTY )", 210, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			else
			{
				if( CAuxiliaryState::GetInstance( )->GetSlotInfoTwo( )._nNumPlayers == 1 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "( 1P,", 210, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoTwo( )._nNumPlayers == 2 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "( 2P,", 210, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				
				if( CAuxiliaryState::GetInstance( )->GetSlotInfoTwo( )._nCurrentLevel == 1 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 1 )", 295, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoTwo( )._nCurrentLevel == 2 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 2 )", 295, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoTwo( )._nCurrentLevel == 3 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 3 )", 295, 120, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}

			CGame::GetInstance( ) ->GetFont( )->Draw( "SLOT 3", 70, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			
			if( CAuxiliaryState::GetInstance( )->GetSlotInfoThree( )._nCurrentLevel == 0 )
				CGame::GetInstance( ) ->GetFont( )->Draw( "( SLOT EMPTY )", 210, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			else
			{
				if( CAuxiliaryState::GetInstance( )->GetSlotInfoThree( )._nNumPlayers == 1 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "( 1P,", 210, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoThree( )._nNumPlayers == 2 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "( 2P,", 210, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				
				if( CAuxiliaryState::GetInstance( )->GetSlotInfoThree( )._nCurrentLevel == 1 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 1 )", 295, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoThree( )._nCurrentLevel == 2 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 2 )", 295, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
				else if( CAuxiliaryState::GetInstance( )->GetSlotInfoThree( )._nCurrentLevel == 3 )
					CGame::GetInstance( ) ->GetFont( )->Draw( "LEVEL 3 )", 295, 140, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			}

			CGame::GetInstance( ) ->GetFont( )->Draw( "CANCEL", 80, 160, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
	}
}

bool CMenuState::Load( int _nSlot )
{
	int nDataChunkSize;
	CLevel* pLevel = CGameplayState::GetInstance()->GetLevel();

	char _saveSlot[128] = "resource/data/Lapidem_GameSaves.bin";
	ifstream fin( _saveSlot, std::ios_base::in | std::ios_base::binary );

	if( fin.is_open( ) )
	{
		// - - - - - - - - - - - - - - 
		// Slot 1
		// - - - - - - - - - - - - - - 
		fin.read( ( char* )&tSlotOne.nPlayerCount, sizeof( int ) );
		fin.read( ( char* )&tSlotOne.nPositionX, sizeof( int ) );
		fin.read( ( char* )&tSlotOne.nPositionY, sizeof( int ) );

		fin.read( ( char* )&tSlotOne.manaP1.nFire, sizeof( int ) );
		fin.read( ( char* )&tSlotOne.manaP1.nEarth, sizeof( int ) );
		fin.read( ( char* )&tSlotOne.manaP1.nIce, sizeof( int ) );
		fin.read( ( char* )&tSlotOne.manaP1.nAir, sizeof( int ) );

		fin.read( ( char* )&tSlotOne.nPlayerOneHealth, sizeof( int ) );

		if( 2 == tSlotOne.nPlayerCount )
		{
			fin.read( ( char* )&tSlotOne.nPlayerTwoHealth, sizeof( int ) );

			fin.read( ( char* )&tSlotOne.nPlayerTwoPosX, sizeof( int ) );
			fin.read( ( char* )&tSlotOne.nPlayerTwoPosY, sizeof( int ) );
			
			fin.read( ( char* )&tSlotOne.manaP2.nFire, sizeof( int ) );
			fin.read( ( char* )&tSlotOne.manaP2.nEarth, sizeof( int ) );
			fin.read( ( char* )&tSlotOne.manaP2.nIce, sizeof( int ) );
			fin.read( ( char* )&tSlotOne.manaP2.nAir, sizeof( int ) );
		}

		fin.read( ( char* )&tSlotOne.nPlayerOneScore, sizeof( int ) );
		fin.read( ( char* )&tSlotOne.nPlayerTwoScore, sizeof( int ) );

		fin.read( ( char* )&nDataChunkSize, sizeof( int ) );

		if( _nSlot == 1 )
		{
			if( !pLevel->LoadLevelFromSave( &fin ) )
			{
				fin.close( );
				return false;
			}
		} else fin.seekg( nDataChunkSize, ios_base::cur );

		// - - - - - - - - - - - - - - 
		// Slot 2
		// - - - - - - - - - - - - - - 
		fin.read( ( char* )&tSlotTwo.nPlayerCount, sizeof( int ) );
		fin.read( ( char* )&tSlotTwo.nPositionX, sizeof( int ) );
		fin.read( ( char* )&tSlotTwo.nPositionY, sizeof( int ) );

		fin.read( ( char* )&tSlotTwo.manaP1.nFire, sizeof( int ) );
		fin.read( ( char* )&tSlotTwo.manaP1.nEarth, sizeof( int ) );
		fin.read( ( char* )&tSlotTwo.manaP1.nIce, sizeof( int ) );
		fin.read( ( char* )&tSlotTwo.manaP1.nAir, sizeof( int ) );

		fin.read( ( char* )&tSlotTwo.nPlayerOneHealth, sizeof( int ) );

		if( 2 == tSlotTwo.nPlayerCount )
		{
			fin.read( ( char* )&tSlotTwo.nPlayerTwoHealth, sizeof( int ) );

			fin.read( ( char* )&tSlotTwo.nPlayerTwoPosX, sizeof( int ) );
			fin.read( ( char* )&tSlotTwo.nPlayerTwoPosY, sizeof( int ) );
			
			fin.read( ( char* )&tSlotTwo.manaP2.nFire, sizeof( int ) );
			fin.read( ( char* )&tSlotTwo.manaP2.nEarth, sizeof( int ) );
			fin.read( ( char* )&tSlotTwo.manaP2.nIce, sizeof( int ) );
			fin.read( ( char* )&tSlotTwo.manaP2.nAir, sizeof( int ) );
		}

		fin.read( ( char* )&tSlotTwo.nPlayerOneScore, sizeof( int ) );
		fin.read( ( char* )&tSlotTwo.nPlayerTwoScore, sizeof( int ) );

		fin.read( ( char* )&nDataChunkSize, sizeof( int ) );
		
		if( _nSlot == 2 )
		{
			if( !pLevel->LoadLevelFromSave( &fin ) )
			{
				fin.close( );
				return false;
			}
		} else fin.seekg( nDataChunkSize, ios_base::cur );

		// - - - - - - - - - - - - - - 
		// Slot 3
		// - - - - - - - - - - - - - - 
		fin.read( ( char* )&tSlotThree.nPlayerCount, sizeof( int ) );
		fin.read( ( char* )&tSlotThree.nPositionX, sizeof( int ) );
		fin.read( ( char* )&tSlotThree.nPositionY, sizeof( int ) );

		fin.read( ( char* )&tSlotThree.manaP1.nFire, sizeof( int ) );
		fin.read( ( char* )&tSlotThree.manaP1.nEarth, sizeof( int ) );
		fin.read( ( char* )&tSlotThree.manaP1.nIce, sizeof( int ) );
		fin.read( ( char* )&tSlotThree.manaP1.nAir, sizeof( int ) );

		fin.read( ( char* )&tSlotThree.nPlayerOneHealth, sizeof( int ) );

		if( 2 == tSlotThree.nPlayerCount )
		{
			fin.read( ( char* )&tSlotThree.nPlayerTwoHealth, sizeof( int ) );

			fin.read( ( char* )&tSlotThree.nPlayerTwoPosX, sizeof( int ) );
			fin.read( ( char* )&tSlotThree.nPlayerTwoPosY, sizeof( int ) );
			
			fin.read( ( char* )&tSlotThree.manaP2.nFire, sizeof( int ) );
			fin.read( ( char* )&tSlotThree.manaP2.nEarth, sizeof( int ) );
			fin.read( ( char* )&tSlotThree.manaP2.nIce, sizeof( int ) );
			fin.read( ( char* )&tSlotThree.manaP2.nAir, sizeof( int ) );
		}

		fin.read( ( char* )&tSlotThree.nPlayerOneScore, sizeof( int ) );
		fin.read( ( char* )&tSlotThree.nPlayerTwoScore, sizeof( int ) );

		fin.read( ( char*)&nDataChunkSize, sizeof( int ) );

		if( _nSlot == 3 )
		{
			if(!pLevel->LoadLevelFromSave(&fin))
			{
				fin.close();
				return false;
			}
		}
		else
			fin.seekg(nDataChunkSize, ios_base::cur);
	} fin.close( );

	if( 1 == _nSlot )
	{
		CGameplayState::GetInstance( )->SetSlotLoaded( 1 );
		CGameplayState::GetInstance( )->SetLoadedFromFile( true );
	}
	else if( 2 == _nSlot )
	{
		CGameplayState::GetInstance( )->SetSlotLoaded( 2 );
		CGameplayState::GetInstance( )->SetLoadedFromFile( true );
	}
	else if( 3 == _nSlot )
	{
		CGameplayState::GetInstance( )->SetSlotLoaded( 3 );
		CGameplayState::GetInstance( )->SetLoadedFromFile( true );
	}

	return true;
}

void CMenuState::Exit( )
{
	CAuxiliaryState::GetInstance( )->SaveConfig( "resource/data/Lapidem_Config.dat" );

	m_pWM->Stop( CGame::GetInstance( )->GetLevelOneMusic( ) );
	m_pWM->Stop( CGame::GetInstance( )->GetLevelTwoMusic( ) );
	m_pWM->Stop( CGame::GetInstance( )->GetLevelThreeMusic( ) );
	m_pTM->UnloadTexture( m_nImageID );
}