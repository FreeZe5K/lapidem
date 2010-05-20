#include "stdheaders.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CGameOver.h"
#include "CProfiler.h"
#include "CAnimationWarehouse.h"

#ifdef _DEBUG
#include "CPickups.h"
#endif

#define PLAYER_SPEED 110
CGameplayState* CGameplayState::GetInstance( )
{
	static CGameplayState instance;
	return &instance;
}

void CGameplayState::Enter( )
{
	m_bBossSpawned = false;
	thaBoss        = NULL;
	m_pPlayerOne   = NULL;
	m_pPlayerTwo   = NULL;
	m_fP2RespawnTimer      = 0.0f;
	m_nPlayerOneScore      = 0;
	m_nPlayerTwoScore      = 0;
	m_nSinglePlayerScore   = 0;
	m_nCurrentLevel        = 1;

	m_pD3D          = CSGD_Direct3D::GetInstance( );
	m_pTM           = CSGD_TextureManager::GetInstance( );
	m_pDS           = CSGD_DirectSound::GetInstance( );
	m_pWM           = CSGD_WaveManager::GetInstance( );
	m_pDI           = CSGD_DirectInput::GetInstance( );

	CAnimationWarehouse::GetInstance( )->Init( );

	m_bMapActive    = true;

	m_pPlayerOne	= new CPlayer( );
	m_pPlayerOne->SetAnimation( 0, 0 );
	
	if( m_bLoadedFromFile )
	{
		if( 1 == m_nSlotLoadedFrom )
		{
			if( 2 == CMenuState::GetInstance( )->GetSlotOne( ).nPlayerCount )
			{
				m_bTwoPlayers = true;
				CMenuState::GetInstance( )->SetPlayerCount( 2 );
			}
		}
		else if( 2 == m_nSlotLoadedFrom )
		{
			if( 2 == CMenuState::GetInstance( )->GetSlotTwo( ).nPlayerCount )
			{
				m_bTwoPlayers = true;
				CMenuState::GetInstance( )->SetPlayerCount( 2 );
			}
		}
		else if( 3 == m_nSlotLoadedFrom )
		{
			if( 2 == CMenuState::GetInstance( )->GetSlotThree( ).nPlayerCount )
			{
				m_bTwoPlayers = true;
				CMenuState::GetInstance( )->SetPlayerCount( 2 );
			}
		}
	} else theLevel.LoadNewLevel( "resource/data/level1.laplvl" );	// if not loaded from file

	CBase* pEntry = theLevel.GetEntryPoint( );

	if( m_bTwoPlayers )
	{
		m_pPlayerTwo = new CPlayer( );
		m_pPlayerTwo->SetPosX( 190 ); 
		m_pPlayerTwo->SetPosY( 400 );
		m_pPlayerTwo->SetAnimation( 0, 0 );
	}else m_pPlayerTwo = 0;

	CCamera::InitCamera( 0.0f, 0.0f, float(CGame::GetInstance( )->GetScreenWidth( ) ),
		float( CGame::GetInstance( )->GetScreenHeight( ) ), m_pPlayerOne, m_pPlayerTwo  );

	m_pCoM			= Corona_ObjectManager::GetInstance( );
	m_pCoM->SetCamera(CCamera::GetCamera());
	m_pCeH			= Corona_EventHandler::GetInstance( );

	m_pWM->SetVolume( CGame::GetInstance( )->GetLevelOneMusic( ), 
			CGame::GetInstance( )->GetMusicVolume( ) ); 
		m_pWM->SetVolume( CGame::GetInstance( )->GetLevelTwoMusic( ), 
			CGame::GetInstance( )->GetMusicVolume( ) );
		m_pWM->SetVolume( CGame::GetInstance( )->GetLevelTwoMusic( ), 
			CGame::GetInstance( )->GetMusicVolume( ) );
	m_pWM->SetVolume( CGame::GetInstance( )->GetMenuTick( ), 
		CGame::GetInstance( )->GetSoundFXVolume( ) ); 

	m_pWM->Stop( CGame::GetInstance( )->GetMainMenuMusic( ) );
	if( m_nCurrentLevel == 1 )
		m_pWM->Play( CGame::GetInstance( )->GetLevelOneMusic( ), DSBPLAY_LOOPING );
	else if( m_nCurrentLevel == 2 )
		m_pWM->Play( CGame::GetInstance( )->GetLevelTwoMusic( ), DSBPLAY_LOOPING );
	else if( m_nCurrentLevel == 3 )
		m_pWM->Play( CGame::GetInstance( )->GetLevelThreeMusic( ), DSBPLAY_LOOPING );

	// - - - - - - - - - - - - - -
	// Change the background image.
	// - - - - - - - - - - - - - -
	// Note by Pablo ::
	// Background image will be handled by the CLevel.
	m_nImageID[0]   = m_pTM->LoadTexture( "resource/graphics/placeholderArt.png");
	m_nImageID[1]   = m_pTM->LoadTexture( "resource/graphics/Lapidem_ISinglePlayer.png" );
	m_nImageID[2]   = m_pTM->LoadTexture( "resource/graphics/Lapidem_IMultiPlayer.png" );
	m_nImageID[3]   = m_pTM->LoadTexture( "resource/graphics/Lapidem_Minimap.png" );
	m_nImageID[4]   = m_pTM->LoadTexture( "resource/graphics/Lapidem_MMArrow.png" );
	m_nImageID[5]   = m_pTM->LoadTexture( "resource/graphics/Lapidem_MMComplete.png" );
	// - - - - - - - - - - - - - -

	if( m_pPlayerTwo )
		m_pCoM->AddObject( m_pPlayerTwo );

	if( m_bLoadedFromFile )
	{
		if( 1 == m_nSlotLoadedFrom )
		{
			pEntry->SetPosX( float( CMenuState::GetInstance( )->GetSlotOne( ).nPositionX ) );
			m_pPlayerOne->SetPosX( float( CMenuState::GetInstance( )->GetSlotOne( ).nPositionX ) );
			pEntry->SetPosY( float( CMenuState::GetInstance( )->GetSlotOne( ).nPositionY ) );
			m_pPlayerOne->SetPosY( float( CMenuState::GetInstance( )->GetSlotOne( ).nPositionY ) );

			m_pPlayerOne->setWindEnergy( CMenuState::GetInstance( )->GetSlotOne( ).manaP1.nAir );
			m_pPlayerOne->setFireEnergy( CMenuState::GetInstance( )->GetSlotOne( ).manaP1.nFire );
			m_pPlayerOne->setEarthEnergy( CMenuState::GetInstance( )->GetSlotOne( ).manaP1.nEarth );
			m_pPlayerOne->setIceEnergy( CMenuState::GetInstance( )->GetSlotOne( ).manaP1.nIce );

			m_pPlayerOne->SetHealth( CMenuState::GetInstance( )->GetSlotOne( ).nPlayerOneHealth );

			if( 2 == CMenuState::GetInstance( )->GetSlotOne( ).nPlayerCount )
			{
				m_pPlayerTwo->SetHealth( CMenuState::GetInstance( )->GetSlotOne( ).nPlayerTwoHealth );

				m_pPlayerTwo->SetPosX( float( CMenuState::GetInstance( )->GetSlotOne( ).nPlayerTwoPosX ) );
				m_pPlayerTwo->SetPosY( float( CMenuState::GetInstance( )->GetSlotOne( ).nPlayerTwoPosY ) );
				
				m_pPlayerTwo->setWindEnergy( CMenuState::GetInstance( )->GetSlotOne( ).manaP2.nAir );
				m_pPlayerTwo->setFireEnergy( CMenuState::GetInstance( )->GetSlotOne( ).manaP2.nFire );
				m_pPlayerTwo->setEarthEnergy( CMenuState::GetInstance( )->GetSlotOne( ).manaP2.nEarth );
				m_pPlayerTwo->setIceEnergy( CMenuState::GetInstance( )->GetSlotOne( ).manaP2.nIce );
			}

			m_nPlayerOneScore     = CMenuState::GetInstance( )->GetSlotOne( ).nPlayerOneScore;
			m_nPlayerTwoScore     = CMenuState::GetInstance( )->GetSlotOne( ).nPlayerTwoScore;
		}
		else if( 2 == m_nSlotLoadedFrom )
		{
			pEntry->SetPosX( float( CMenuState::GetInstance( )->GetSlotTwo( ).nPositionX ) );
			m_pPlayerOne->SetPosX( float( CMenuState::GetInstance( )->GetSlotTwo( ).nPositionX ) );
			pEntry->SetPosY( float( CMenuState::GetInstance( )->GetSlotTwo( ).nPositionY ) );
			m_pPlayerOne->SetPosY( float( CMenuState::GetInstance( )->GetSlotTwo( ).nPositionY ) );
			
			m_pPlayerOne->setWindEnergy( CMenuState::GetInstance( )->GetSlotTwo( ).manaP1.nAir );
			m_pPlayerOne->setFireEnergy( CMenuState::GetInstance( )->GetSlotTwo( ).manaP1.nFire );
			m_pPlayerOne->setEarthEnergy( CMenuState::GetInstance( )->GetSlotTwo( ).manaP1.nEarth );
			m_pPlayerOne->setIceEnergy( CMenuState::GetInstance( )->GetSlotTwo( ).manaP1.nIce );

			m_pPlayerOne->SetHealth( CMenuState::GetInstance( )->GetSlotTwo( ).nPlayerOneHealth );

			if( 2 == CMenuState::GetInstance( )->GetSlotTwo( ).nPlayerCount )
			{
				m_pPlayerTwo->SetHealth( CMenuState::GetInstance( )->GetSlotTwo( ).nPlayerTwoHealth );

				m_pPlayerTwo->SetPosX( float( CMenuState::GetInstance( )->GetSlotTwo( ).nPlayerTwoPosX ) );
				m_pPlayerTwo->SetPosY( float( CMenuState::GetInstance( )->GetSlotTwo( ).nPlayerTwoPosY ) );
				
				m_pPlayerTwo->setWindEnergy( CMenuState::GetInstance( )->GetSlotTwo( ).manaP2.nAir );
				m_pPlayerTwo->setFireEnergy( CMenuState::GetInstance( )->GetSlotTwo( ).manaP2.nFire );
				m_pPlayerTwo->setEarthEnergy( CMenuState::GetInstance( )->GetSlotTwo( ).manaP2.nEarth );
				m_pPlayerTwo->setIceEnergy( CMenuState::GetInstance( )->GetSlotTwo( ).manaP2.nIce );
			}

			m_nPlayerOneScore     = CMenuState::GetInstance( )->GetSlotTwo( ).nPlayerOneScore;
			m_nPlayerTwoScore     = CMenuState::GetInstance( )->GetSlotTwo( ).nPlayerTwoScore;
		}
		else if( 3 == m_nSlotLoadedFrom )
		{
			pEntry->SetPosX( float( CMenuState::GetInstance( )->GetSlotThree( ).nPositionX ) );
			m_pPlayerOne->SetPosX( float( CMenuState::GetInstance( )->GetSlotThree( ).nPositionX ) );
			pEntry->SetPosY( float( CMenuState::GetInstance( )->GetSlotThree( ).nPositionY ) );
			m_pPlayerOne->SetPosY( float( CMenuState::GetInstance( )->GetSlotThree( ).nPositionY ) );
			
			m_pPlayerOne->setWindEnergy( CMenuState::GetInstance( )->GetSlotThree( ).manaP2.nAir );
			m_pPlayerOne->setFireEnergy( CMenuState::GetInstance( )->GetSlotThree( ).manaP2.nFire );
			m_pPlayerOne->setEarthEnergy( CMenuState::GetInstance( )->GetSlotThree( ).manaP2.nEarth );
			m_pPlayerOne->setIceEnergy( CMenuState::GetInstance( )->GetSlotThree( ).manaP2.nIce );

			m_pPlayerOne->SetHealth( CMenuState::GetInstance( )->GetSlotThree( ).nPlayerOneHealth );

			if( 2 == CMenuState::GetInstance( )->GetSlotThree( ).nPlayerCount )
			{
				m_pPlayerTwo->SetHealth( CMenuState::GetInstance( )->GetSlotThree( ).nPlayerTwoHealth );

				m_pPlayerTwo->SetPosX( float( CMenuState::GetInstance( )->GetSlotThree( ).nPlayerTwoPosX ) );
				m_pPlayerTwo->SetPosY( float( CMenuState::GetInstance( )->GetSlotThree( ).nPlayerTwoPosY ) );
				
				m_pPlayerTwo->setWindEnergy( CMenuState::GetInstance( )->GetSlotThree( ).manaP2.nAir );
				m_pPlayerTwo->setFireEnergy( CMenuState::GetInstance( )->GetSlotThree( ).manaP2.nFire );
				m_pPlayerTwo->setEarthEnergy( CMenuState::GetInstance( )->GetSlotThree( ).manaP2.nEarth );
				m_pPlayerTwo->setIceEnergy( CMenuState::GetInstance( )->GetSlotThree( ).manaP2.nIce );
			}

			m_nPlayerOneScore     = CMenuState::GetInstance( )->GetSlotThree( ).nPlayerOneScore;
			m_nPlayerTwoScore     = CMenuState::GetInstance( )->GetSlotThree( ).nPlayerTwoScore;
		}
	}
	else
	{
		m_pPlayerOne->SetPosX( pEntry->GetPosX( ) );
		m_pPlayerOne->SetPosY( pEntry->GetPosY( ) );
	}

	m_pCoM->AddObject( m_pPlayerOne );

	if( m_pPlayerTwo )
		Corona_EventHandler::GetInstance( )->RegisterClient( this, "P2 OFFSCREEN" );
	m_fP2RespawnTimer = -1.0f;

	m_tDir._x = 0.0f;
	m_tDir._y = -50.0f;

	CGame::GetInstance( )->SetTimeLeft( 600 );
}

bool CGameplayState::Input( )
{
	if( isalpha( CSGD_DirectInput::GetInstance( )->CheckBufferedKeysEx( ) ) )
		CGame::GetInstance( )->SetCheatString( CGame::GetInstance( )->
		GetCheatString( ) += toupper( CSGD_DirectInput::GetInstance( )->CheckBufferedKeysEx( ) ) );

	if( m_pDI->KeyPressed( DIK_P ) || m_pDI->KeyPressed( DIK_ESCAPE ) || 
		m_pDI->JoystickButtonPressed( 9 ) )
	{
		CGame::GetInstance( )->PushState( CPauseMenuState::GetInstance( ) );
		CGame::GetInstance( )->SetPaused( true );
	}

	if(!m_pPlayerOne->GetTossed())
	{
		if( m_pDI->KeyDown( DIK_D ) || m_pDI->JoystickDPadDown( 1 ) )
			m_pPlayerOne->SetVelX( PLAYER_SPEED );
		else if( m_pDI->KeyDown( DIK_A )  || m_pDI->JoystickDPadDown(0) )
			m_pPlayerOne->SetVelX( -PLAYER_SPEED );
		else m_pPlayerOne->SetVelX( PLAYER_SPEED * m_pDI->JoystickGetLStickXNormalized( ) );

		if( m_pDI->KeyDown( DIK_W ) || m_pDI->JoystickDPadDown( 2 ) || 
			m_pDI->JoystickGetLStickYNormalized( ) < -0.5f )
			m_pPlayerOne->Jump( );
	}
	if( m_pDI->KeyDown( DIK_F ) || m_pDI->JoystickButtonDown( 0 ) || 
		m_pDI->JoystickButtonDown( 7 ) )
		m_pPlayerOne->Attack( 1 );

	if( m_pDI->KeyDown( DIK_R ) || m_pDI->JoystickButtonDown( 1 ) || 
		m_pDI->JoystickButtonDown( 6 ) )
		m_pPlayerOne->Attack( 2 );

	if( m_pDI->KeyPressed( DIK_T ) || m_pDI->JoystickButtonDown( 2 ))
		m_pPlayerOne->Attack( 3 );

	if( m_pDI->KeyPressed( DIK_1 ) )
		m_pPlayerOne->SetEleType( OBJ_ICE  );
	else if( m_pDI->KeyPressed( DIK_2 ) )
		m_pPlayerOne->SetEleType( OBJ_FIRE );

	if( !m_pPlayerTwo )
	{
		//Player One controls if there is only 1 player:
		if( m_pDI->KeyPressed( DIK_3 ) )
			m_pPlayerOne->SetEleType( OBJ_EARTH );
		else if( m_pDI->KeyPressed( DIK_4 ) )
			m_pPlayerOne->SetEleType( OBJ_WIND );

		else if( m_pDI->JoystickButtonPressed( 4 ) )
			m_pPlayerOne->SetEleType( EleType( m_pPlayerOne->GetEleType( ) + OBJ_ICE ) );
		else if( m_pDI->JoystickButtonPressed( 5 ) )
			m_pPlayerOne->SetEleType( EleType( m_pPlayerOne->GetEleType( ) - OBJ_ICE ) );
	}
	else
	{
		//Player One Controls if there are two players:
		if( m_pDI->JoystickButtonPressed( 4 ) )
			m_pPlayerOne->SetEleType( OBJ_ICE );
		else if( m_pDI->JoystickButtonPressed( 5 ) )
			m_pPlayerOne->SetEleType( OBJ_FIRE );

		//Player Two Controls:

		if(!m_pPlayerTwo->GetTossed())
		{
			if( m_pDI->KeyDown( DIK_LEFT ) || m_pDI->JoystickDPadDown( 0, 1 ) )
				m_pPlayerTwo->SetVelX( -PLAYER_SPEED );
			else if( m_pDI->KeyDown( DIK_RIGHT ) || m_pDI->JoystickDPadDown( 1, 1 ) )
				m_pPlayerTwo->SetVelX( PLAYER_SPEED );
			else
				m_pPlayerTwo->SetVelX( PLAYER_SPEED * m_pDI->JoystickGetLStickXNormalized( 1 ) );

			if( m_pDI->KeyDown( DIK_UP ) || m_pDI->JoystickDPadDown( 2, 1 ) || 
				m_pDI->JoystickGetLStickYNormalized( 1 ) < -0.5f  )
				m_pPlayerTwo->Jump( );
		}

		if(m_pDI->KeyDown( DIK_RSHIFT ) || m_pDI->JoystickButtonDown( 0, 1 ) || 
			m_pDI->JoystickButtonDown( 7, 1 ) )
			m_pPlayerTwo->Attack( 1 );

		if(m_pDI->KeyDown( DIK_NUMPAD0 ) || m_pDI->JoystickButtonDown( 1, 1 ) ||
			m_pDI->JoystickButtonDown( 6, 1 ) )
			m_pPlayerTwo->Attack( 2 );

		if(m_pDI->KeyDown( DIK_NUMPADENTER ) || m_pDI->JoystickButtonDown( 2, 1 ) )
			m_pPlayerTwo->Attack( 3 );

		if(m_pDI->KeyDown( DIK_NUMPAD1 ))
		{
			if(m_pPlayerTwo->GetEleType() == OBJ_WIND )
				m_pPlayerTwo->SetEleType( OBJ_EARTH );
			else m_pPlayerTwo->SetEleType( OBJ_WIND );
		}

		if( m_pDI->JoystickButtonPressed( 4, 1 ) )
			m_pPlayerTwo->SetEleType( OBJ_WIND );
		else if( m_pDI->JoystickButtonPressed( 5, 1 ) )
			m_pPlayerTwo->SetEleType( OBJ_EARTH );
	}

	if( m_pDI->KeyPressed( DIK_M ) ) m_bMapActive = !m_bMapActive;

#ifdef _DEBUG
	if(m_pDI->KeyDown( DIK_K ) )
		spawnenergy();
#endif
	return true;
}

void CGameplayState::Update( float fET )
{
#ifdef _DEBUG
	CProfiler::GetInstance( )->Start( "Profiler Start" );
	CProfiler::GetInstance( )->Start( "CGameplay Update" );
	CProfiler::GetInstance( )->End( "Profiler Start" );
#endif

	CParticleManager::GetInstance( )->Update( fET );

	if( !m_bBossSpawned && !strcmp( theLevel.GetNextLevelFileName(), " " ) )
	{
		int GameTime( CGame::GetInstance( )->GetTimeLeft( ) );

		if( GameTime <  180)
			GameTime = 2;
		else GameTime = 1;

		thaBoss = new CEnemy(OBJ_SHIELD, 750, 500, GameTime);
		Corona_ObjectManager::GetInstance()->AddObject(thaBoss);
		m_bBossSpawned = true;
	}

	if( m_nCurrentLevel == 1)
	{
		if( m_pWM->IsWavePlaying( CGame::GetInstance( )->GetLevelTwoMusic( ) ) )
			m_pWM->Stop( CGame::GetInstance( )->GetLevelTwoMusic( ) );
		else if( m_pWM->IsWavePlaying( CGame::GetInstance( )->GetLevelThreeMusic( ) ) )
			m_pWM->Stop( CGame::GetInstance( )->GetLevelTwoMusic( ) );

		if( !m_pWM->IsWavePlaying( CGame::GetInstance( )->GetLevelOneMusic( ) ) )
			m_pWM->Play( CGame::GetInstance( )->GetLevelOneMusic( ), DSBPLAY_LOOPING );
	}
	else if( m_nCurrentLevel == 2)
	{
		if( m_pWM->IsWavePlaying( CGame::GetInstance( )->GetLevelOneMusic( ) ) )
			m_pWM->Stop( CGame::GetInstance( )->GetLevelOneMusic( ) );
		else if( m_pWM->IsWavePlaying( CGame::GetInstance( )->GetLevelThreeMusic( ) ) )
			m_pWM->Stop( CGame::GetInstance( )->GetLevelThreeMusic( ) );
		
		if( !m_pWM->IsWavePlaying( CGame::GetInstance( )->GetLevelTwoMusic( ) ) )
			m_pWM->Play( CGame::GetInstance( )->GetLevelTwoMusic( ), DSBPLAY_LOOPING );
	}
	else if( m_nCurrentLevel == 3)
	{
		if( m_pWM->IsWavePlaying( CGame::GetInstance( )->GetLevelTwoMusic( ) ) )
			m_pWM->Stop( CGame::GetInstance( )->GetLevelTwoMusic( ) );
		else if( m_pWM->IsWavePlaying( CGame::GetInstance( )->GetLevelOneMusic( ) ) )
			m_pWM->Stop( CGame::GetInstance( )->GetLevelOneMusic( ) );
		
		if( !m_pWM->IsWavePlaying( CGame::GetInstance( )->GetLevelThreeMusic( ) ) )
			m_pWM->Play( CGame::GetInstance( )->GetLevelThreeMusic( ), DSBPLAY_LOOPING );
	}

	if(m_bBossSpawned && thaBoss->GetHealth() <= 0)
	{
		CGameOver::GetInstance( )->SetState( 2 );
		CGameOver::GetInstance( )->SetCondition( 1 );
		CGame::GetInstance( )->ChangeState( CGameOver::GetInstance( ) );
		return;
	}

	if( m_pPlayerTwo )
	{
		// Respawn player 2 if he was off screen
		if( m_fP2RespawnTimer >= 0.0f )
		{
			m_fP2RespawnTimer = m_fP2RespawnTimer + fET;

			if( m_fP2RespawnTimer > 3.0f )
			{
				// set the timer to a negative number so this 
				// code does not run again until the event is trigered
				m_fP2RespawnTimer = -1.0f;

				// set the pos of the p2 to almost the pos of p1, then 
				// move the player2 out of player 1 so it looks nice
				m_pPlayerTwo->SetPosX( m_pPlayerOne->GetPosX( ) + 2 );
				m_pPlayerTwo->SetPosY( m_pPlayerOne->GetPosY( ) );
				m_pPlayerTwo->MoveOutOf( m_pPlayerOne, fET);
			}
		}
	}

	if( theLevel.NextLevelOpen( ) )
	{
		if( m_bPlayerReachedEnd )
		{

			m_pCoM->RemoveAllObjects();
			m_pCoM->AddObject(m_pPlayerOne);
			if(m_pPlayerTwo)
			{
				m_pCoM->AddObject(m_pPlayerTwo);

				if(m_pPlayerTwo->GetReticle())
					m_pCoM->AddObject(m_pPlayerTwo->GetReticle());
			}

			if(m_pPlayerOne->GetReticle())
				m_pCoM->AddObject(m_pPlayerOne->GetReticle());

			theLevel.LoadNextLevel();
			++m_nCurrentLevel;

			CBase* pEntry = theLevel.GetEntryPoint( );
			m_pPlayerOne->SetPosX( pEntry->GetPosX( ) );
			m_pPlayerOne->SetPosY( pEntry->GetPosY( ) );

			m_bPlayerReachedEnd = false;
		}
	}
	else m_bPlayerReachedEnd = false;

	if( m_bMapActive )
	{
		// - - - - - - - - - - -
		// Switch One
		// - - - - - - - - - - -
		tVector2D _s1;
		_s1._x = theLevel.GetSwitchOnePosX( ) - m_pPlayerOne->GetPosX( );
		_s1._y = theLevel.GetSwitchOnePosY( ) - m_pPlayerOne->GetPosY( );

		m_fMMCurrentRotation[0] = Lapidem_Math::GetInstance( )->
			AngleBetweenVectors( m_tDir, _s1 );

		if( theLevel.GetSwitchOnePosX( ) < m_pPlayerOne->GetPosX( ) )
			m_fMMCurrentRotation[0] *= -1;

		// - - - - - - - - - - -
		// Switch Two
		// - - - - - - - - - - -
		tVector2D _s2;
		_s2._x = theLevel.GetSwitchTwoPosX( ) - m_pPlayerOne->GetPosX( );
		_s2._y = theLevel.GetSwitchTwoPosY( ) - m_pPlayerOne->GetPosY( );

		m_fMMCurrentRotation[1] = Lapidem_Math::GetInstance( )->
			AngleBetweenVectors( m_tDir, _s2 );

		if( theLevel.GetSwitchTwoPosX( ) < m_pPlayerOne->GetPosX( ) )
			m_fMMCurrentRotation[1] *= -1;

		// - - - - - - - - - - -
		// Switch Three
		// - - - - - - - - - - -
		tVector2D _s3;
		_s3._x = theLevel.GetSwitchThreePosX( ) - m_pPlayerOne->GetPosX( );
		_s3._y = theLevel.GetSwitchThreePosY( ) - m_pPlayerOne->GetPosY( );

		m_fMMCurrentRotation[2] = Lapidem_Math::GetInstance( )->
			AngleBetweenVectors( m_tDir, _s3 );

		if( theLevel.GetSwitchThreePosX( ) < m_pPlayerOne->GetPosX( ) )
			m_fMMCurrentRotation[2] *= -1;

		// - - - - - - - - - - -
		// Switch Four
		// - - - - - - - - - - -
		tVector2D _s4;
		_s4._x = theLevel.GetSwitchFourPosX( ) - m_pPlayerOne->GetPosX( );
		_s4._y = theLevel.GetSwitchFourPosY( ) - m_pPlayerOne->GetPosY( );

		m_fMMCurrentRotation[3] = Lapidem_Math::GetInstance( )->
			AngleBetweenVectors( m_tDir, _s4 );

		if( theLevel.GetSwitchFourPosX( ) < m_pPlayerOne->GetPosX( ) )
			m_fMMCurrentRotation[3] *= -1;

		// - - - - - - - - - - -
		// Level End
		// - - - - - - - - - - -
		tVector2D _end;
		_end._x = theLevel.GetLevelEndX( ) - m_pPlayerOne->GetPosX( );
		_end._y = theLevel.GetLevelEndY( ) - m_pPlayerOne->GetPosY( );

		m_fMMCurrentRotation[4] = Lapidem_Math::GetInstance( )->
		AngleBetweenVectors( m_tDir, _end );

		// Internal bug fix
		//if( theLevel.GetLevelEndX( ) < m_pPlayerOne->GetPosX( ) )
		//	m_fMMCurrentRotation[4] *= -1;
	}

	m_pCoM->UpdateObjects( CGame::GetInstance( )->GetElapsedTime( ) );
	theLevel.Update( fET );
	m_pCeH->ProcessEvents( );

	if( m_pPlayerOne->GetHealth( ) <= 0 )
	{
		m_pPlayerOne->SetFainted(true);
	}
	if( m_pPlayerTwo)
	{
		if( m_pPlayerTwo->GetHealth( ) <= 0 )
		{
			m_pPlayerTwo->SetFainted(true);
		}
	}

	if( m_pPlayerOne->GetFainted()  )//&& m_pPlayerTwo )
	{
		if( !m_pPlayerTwo || m_pPlayerTwo->GetFainted() )
		{
			// Player lost
			CGameOver::GetInstance( )->SetState( 1 );

			// Player died because their health ran out
			CGameOver::GetInstance( )->SetCondition( 1 );

			// Change the state to game over
			CGame::GetInstance( )->ChangeState( CGameOver::GetInstance( ) );
		}
	}

#ifdef _DEBUG
	CProfiler::GetInstance( )->Start( "Profiler End" );
	CProfiler::GetInstance( )->End( "CGameplay Update" );
	CProfiler::GetInstance( )->End( "Profiler End" );
#endif
}

void CGameplayState::Render( )
{
	theLevel.Render( );
	m_pCoM->RenderObjects( );
	CParticleManager::GetInstance( )->Render( );

	
	if( 1 == CMenuState::GetInstance( )->GetPlayerCount( ) )
	{
		m_pTM->Draw( m_nImageID[1], 0, 0, 1.0f, 1.0f, NULL, 
			0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB( 160, 255, 255, 255 ) );

		char cBuffer[128];
		sprintf_s( cBuffer, "HEALTH  - %i", m_pPlayerOne->GetHealth( ) );
		CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
			5, 0, 0.7f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );



		if( OBJ_FIRE == m_pPlayerOne->GetEleType( ) )
		{
			sprintf_s( cBuffer, "ELEMENT - %s - %i", "FIRE", m_pPlayerOne->GetFireEnergy() );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
				5, 18, 0.7f, D3DCOLOR_ARGB( 255, 255, 150, 150 ) );

		}
		else if( OBJ_ICE == m_pPlayerOne->GetEleType( ) )
		{
			sprintf_s( cBuffer, "ELEMENT - %s - %i", "ICE", m_pPlayerOne->GetIceEnergy() );			
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
				5, 18, 0.7f, D3DCOLOR_ARGB( 255, 150, 150, 255 ) );
		}
		else if( OBJ_WIND == m_pPlayerOne->GetEleType( ) )
		{
			sprintf_s( cBuffer, "ELEMENT - %s - %i", "WIND", m_pPlayerOne->GetWindEnergy() );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
				5, 18, 0.7f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( OBJ_EARTH == m_pPlayerOne->GetEleType( ) )
		{
			sprintf_s( cBuffer, "ELEMENT - %s - %i", "EARTH", m_pPlayerOne->GetEarthEnergy() );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
				5, 18, 0.7f, D3DCOLOR_ARGB( 255, 160, 255, 40 ) );
		}

		sprintf_s( cBuffer, "%i",  m_pPlayerOne->GetT3Count());
		CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
			250, 20, 1.5f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );


		sprintf_s( cBuffer, "SCORE  - %i", GetPlayerOne( )->GetScore( ));
		CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
			5, 35, 0.7f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	}
	else if( 2 == CMenuState::GetInstance( )->GetPlayerCount( ) )
	{
		m_pTM->Draw( m_nImageID[2], 0, 0, 1.0f, 1.0f, NULL, 
			0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB( 160, 255, 255, 255 ) );

		// Player one
		// - - - - - -
		char cBuffer[64];
		sprintf_s( cBuffer, "HEALTH   - %i", m_pPlayerOne->GetHealth( ) );
		CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
			5, 0, 0.7f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

		if( OBJ_FIRE == m_pPlayerOne->GetEleType( ) )
		{
			sprintf_s( cBuffer, "ELEMENT - %s", "FIRE" );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
				5, 18, 0.7f, D3DCOLOR_ARGB( 255, 255, 150, 150 ) );

			sprintf_s( cBuffer, "ENERGY  - %i", m_pPlayerOne->GetFireEnergy() );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
			5, 35, 0.7f, D3DCOLOR_ARGB( 255, 255, 150, 150 ) );
		}
		else if( OBJ_ICE == m_pPlayerOne->GetEleType( ) )
		{
			sprintf_s( cBuffer, "ELEMENT - %s", "ICE" );			
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
				5, 18, 0.7f, D3DCOLOR_ARGB( 255, 150, 150, 255 ) );

			sprintf_s( cBuffer, "ENERGY  - %i", m_pPlayerOne->GetIceEnergy() );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
				5, 35, 0.7f, D3DCOLOR_ARGB( 255, 150, 150, 255 ));
		}
		else if( OBJ_WIND == m_pPlayerOne->GetEleType( ) )
		{
			sprintf_s( cBuffer, "ELEMENT - %s", "WIND");
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
				5, 18, 0.7f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			sprintf_s( cBuffer, "ENERGY  - %i", m_pPlayerOne->GetWindEnergy() );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
				5, 35, 0.7f,D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( OBJ_EARTH == m_pPlayerOne->GetEleType( ) )
		{
			sprintf_s( cBuffer, "ELEMENT - %s", "EARTH" );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
				5, 18, 0.7f, D3DCOLOR_ARGB( 255, 160, 255, 40 ) );

			sprintf_s( cBuffer, "ENERGY  - %i", m_pPlayerOne->GetEarthEnergy() );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
				5, 35, 0.7f, D3DCOLOR_ARGB( 255, 160, 255, 40 ) );
		}

		// Player two
		// - - - - - -
		sprintf_s( cBuffer, "HEALTH   - %i", m_pPlayerTwo->GetHealth( ) );
		CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
			420, 0, 0.7f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

		if( OBJ_WIND == m_pPlayerTwo->GetEleType( ) )
		{
			sprintf_s( cBuffer, "ELEMENT - %s", "WIND" );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
				420, 18, 0.7f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			sprintf_s( cBuffer, "ENERGY  - %i", m_pPlayerTwo->GetWindEnergy() );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
				420, 35, 0.7f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else if( OBJ_EARTH == m_pPlayerTwo->GetEleType( ) )
		{
			sprintf_s( cBuffer, "ELEMENT - %s", "EARTH" );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
				420, 18, 0.7f, D3DCOLOR_ARGB( 255, 160, 255, 40 ) );

			sprintf_s( cBuffer, "ENERGY  - %i", m_pPlayerTwo->GetEarthEnergy() );
			CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
				420, 35, 0.7f,D3DCOLOR_ARGB( 255, 160, 255, 40 ) );
		}
		sprintf_s( cBuffer, "%i",  m_pPlayerOne->GetT3Count());
		CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
			250, 20, 1.5f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

		sprintf_s( cBuffer, "%i",  m_pPlayerTwo->GetT3Count());
		CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
			340, 20, 1.5f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

	
		sprintf_s( cBuffer, "SCORE- %i", GetPlayerOne( )->GetScore( ));//,GetPlayerTwo( )->GetScore( ) );
		CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
			5, 50, 0.7f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		sprintf_s( cBuffer, "SCORE- %i", GetPlayerTwo( )->GetScore( ) );
		CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
			420, 50, 0.7f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

		//CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
		//	240, 0, 0.7f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
	}


	if( m_bMapActive )
	{
		m_pTM->Draw( m_nImageID[3], 0, 0 );

		if( !theLevel.GetSwitchOneON( ) )
			m_pTM->Draw( m_nImageID[4], 53, 385, 0.4f, 0.4f, NULL,
				16.0f, 110.0f, m_fMMCurrentRotation[0] );

		if( !theLevel.GetSwitchTwoON( ) )
			m_pTM->Draw( m_nImageID[4], 53, 385, 0.4f, 0.4f, NULL,
				16.0f, 110.0f, m_fMMCurrentRotation[1] );

		if( !theLevel.GetSwitchThreeON( ) )
			m_pTM->Draw( m_nImageID[4], 53, 385, 0.4f, 0.4f, NULL,
				16.0f, 110.0f, m_fMMCurrentRotation[2] );

		if( !theLevel.GetSwitchFourON( ) )
			m_pTM->Draw( m_nImageID[4], 53, 385, 0.4f, 0.4f, NULL,
				16.0f, 110.0f, m_fMMCurrentRotation[3] );

		if( theLevel.NextLevelOpen( ) )
			m_pTM->Draw( m_nImageID[5], 53, 385, 0.4f, 0.4f, NULL,
				16.0f, 110.0f, m_fMMCurrentRotation[4] );
	}
}

void CGameplayState::Exit( )
{
	m_pCeH->SendEvent( "EnemyDied", NULL ); 
	m_pCeH->ProcessEvents( );

	CParticleManager::GetInstance()->ClearAll();

	m_pWM->Stop( CGame::GetInstance( )->GetLevelOneMusic( ) );
	m_pWM->Stop( CGame::GetInstance( )->GetLevelTwoMusic( ) );
	m_pWM->Stop( CGame::GetInstance( )->GetLevelThreeMusic( ) );
	m_pWM->Reset( CGame::GetInstance( )->GetLevelOneMusic( ) );
	m_pWM->Reset( CGame::GetInstance( )->GetLevelTwoMusic( ) );
	m_pWM->Reset( CGame::GetInstance( )->GetLevelThreeMusic( ) );
	m_pWM->Stop( CGame::GetInstance( )->GetMainMenuMusic( ) );
	m_pWM->Reset( CGame::GetInstance( )->GetMainMenuMusic( ) );

	m_pTM->UnloadTexture( m_nImageID[5] );
	m_pTM->UnloadTexture( m_nImageID[4] );
	m_pTM->UnloadTexture( m_nImageID[3] );
	m_pTM->UnloadTexture( m_nImageID[2] );
	m_pTM->UnloadTexture( m_nImageID[1] );
	m_pTM->UnloadTexture( m_nImageID[0] );
	m_pCoM->RemoveAllObjects( );

	

	if( m_pPlayerOne )
	{
		AddScore( m_pPlayerOne->GetScore() );
		m_pPlayerOne->Release( );
	}
	if( m_pPlayerTwo )
	{
		AddScore( m_pPlayerOne->GetScore() );
		m_pPlayerTwo->Release( );
	}
	if( thaBoss )
		thaBoss->Release();

	theLevel.Clear( );
	m_pCoM->DeleteInstance( );
	CAnimationWarehouse::GetInstance()->DeleteInstance( );

	m_bLoadedFromFile = false;
	m_bTwoPlayers	  = false;

	if( m_pPlayerTwo )
		Corona_EventHandler::GetInstance( )->UnregisterClient( "P2 OFFSCREEN", this );

#if _DEBUG
	CProfiler::GetInstance( )->Save( "CodeProfilerOutput.txt" );
	CProfiler::GetInstance( )->DeleteInstance( );
#endif
}

void CGameplayState::HandleEvent( CEvent* pEvent )
{
	if( !strcmp( pEvent->GetEventID( ).c_str( ), "P2 OFFSCREEN" ) )
	{
		if( m_fP2RespawnTimer == -1.0f )
		{
			m_fP2RespawnTimer = 0.0f;
			
			tVector2D direction;
			direction._x = ( m_pPlayerOne->GetPosX( ) - m_pPlayerTwo->GetPosX( ) );
			direction._y = ( m_pPlayerOne->GetPosY( ) - m_pPlayerTwo->GetPosY( ) );
		}
	}
}

#ifdef _DEBUG
void CGameplayState::spawnenergy()
{
	CPickup * newpickup = new CPickup();
	newpickup->SetPosX(m_pPlayerOne->GetPosX() + 40);
	newpickup->SetPosY(m_pPlayerOne->GetPosY());
	newpickup->SetActive(true);

	newpickup->SetType(OBJ_ENERGY);
	newpickup->SetEleType(OBJ_EARTH);
	newpickup->SetImage( CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/Lapid_EarthEnergy.png"));
	newpickup->SetWidth(64);
	newpickup->SetHeight(48);
	Corona_ObjectManager::GetInstance()->AddObject(newpickup);
	newpickup->Release();

	newpickup = new CPickup();
	newpickup->SetPosX(m_pPlayerOne->GetPosX() + 60);
	newpickup->SetPosY(m_pPlayerOne->GetPosY());
	newpickup->SetActive(true);

	newpickup->SetType(OBJ_ENERGY);
	newpickup->SetEleType(OBJ_FIRE);
	newpickup->SetImage( CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/Lapid_FireEnergy.png"));
	newpickup->SetWidth(64);
	newpickup->SetHeight(48);
	Corona_ObjectManager::GetInstance()->AddObject(newpickup);
	newpickup->Release();

	newpickup = new CPickup();
	newpickup->SetPosX(m_pPlayerOne->GetPosX() + 80);
	newpickup->SetPosY(m_pPlayerOne->GetPosY());
	newpickup->SetActive(true);

	newpickup->SetType(OBJ_ENERGY);
	newpickup->SetEleType(OBJ_ICE);
	newpickup->SetImage( CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/Lapid_IceEnergy.png"));
	newpickup->SetWidth(64);
	newpickup->SetHeight(48);
	Corona_ObjectManager::GetInstance()->AddObject(newpickup);
	newpickup->Release();

	newpickup = new CPickup();
	newpickup->SetPosX(m_pPlayerOne->GetPosX() + 100);
	newpickup->SetPosY(m_pPlayerOne->GetPosY());
	newpickup->SetActive(true);

	newpickup->SetType(OBJ_T3SPELL);
	newpickup->SetEleType(OBJ_WIND);
	newpickup->SetImage( CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/Lapid_WindEnergy.png"));
	newpickup->SetWidth(64);
	newpickup->SetHeight(58);
	Corona_ObjectManager::GetInstance()->AddObject(newpickup);
	newpickup->Release();
}
#endif