#include "CGameplayState.h"
#include "CCamera.h"
#include "CPlayer.h"
#include "Corona_ObjectManager.h"
#include "Corona_EventHandler.h"
#include "CProfiler.h"
#include "CParticleManager.h"
#include "CAnimationWarehouse.h"

CGameplayState* CGameplayState::GetInstance( )
{
	static CGameplayState instance;
	return &instance;
}

void CGameplayState::Enter( )
{
	m_pD3D          = CSGD_Direct3D::GetInstance();
	m_pTM           = CSGD_TextureManager::GetInstance();
	m_pDS           = CSGD_DirectSound::GetInstance();
	m_pWM           = CSGD_WaveManager::GetInstance();
	m_pDI           = CSGD_DirectInput::GetInstance();

	m_pEF			= CEmitterFactory::GetInstance();
	m_pEF->Initialize();

	CAnimationWarehouse::GetInstance( )->LoadAnimationSet( 
		"resource/idlewalk.Anim", D3DCOLOR_XRGB( 255, 255, 255 ) );

	m_pPlayerOne	= new CPlayer( );
	m_pPlayerOne->SetAnimation(0, 0);

	if(m_bTwoPlayers)
	{
		m_pPlayerTwo = new CPlayer();
		m_pPlayerTwo->SetPosX(190); m_pPlayerTwo->SetPosY(400);
		m_pPlayerTwo->SetAnimation(0, 0);
	}
	else
	m_pPlayerTwo	= NULL;

	CCamera::InitCamera(0.0f, 0.0f, (float)CGame::GetInstance()->GetScreenWidth(),
		(float)CGame::GetInstance( )->GetScreenHeight(), m_pPlayerOne );

	m_pCoM			= Corona_ObjectManager::GetInstance();
	m_pCeH			= Corona_EventHandler::GetInstance();

	m_pWM->Stop( CGame::GetInstance( )->GetMainMenuMusic( ) );
	m_pWM->Play( CGame::GetInstance( )->GetGameBGMusic( ), DSBPLAY_LOOPING );

	m_pWM->SetVolume( CGame::GetInstance( )->GetGameBGMusic( ), 
		CGame::GetInstance( )->GetSoundFXVolume( ) ); 

	// - - - - - - - - - - - - - -
	// Change the background image.
	// - - - - - - - - - - - - - -
	/* Note by Pablo
	Background image will be handled by the CLevel.
	*/
	m_nImageID[0]   = m_pTM->LoadTexture( "resource/graphics/placeholderArt.png" );
	m_nImageID[1]   = m_pTM->LoadTexture( "resource/graphics/Lapidem_ISinglePlayer.png" );
	m_nImageID[2]   = m_pTM->LoadTexture( "resource/graphics/Lapidem_IMultiPlayer.png" );
	// - - - - - - - - - - - - - -

	m_pCoM->AddObject(m_pPlayerOne);

	if(m_pPlayerTwo)
		m_pCoM->AddObject(m_pPlayerTwo);

	m_pPM = CParticleManager::GetInstance( );
	m_pEF = CEmitterFactory::GetInstance( );
	m_pEF->Initialize( );

	theLevel.LoadNewLevel("resource/data/level1.laplvl");	

	CBase* pEntry = theLevel.GetEntryPoint();

	if( m_bLoadedFromFile )
	{
		// todo - handle what to do if
		// more then one player is active.
		if( 1 == m_nSlotLoadedFrom )
		{
			pEntry->SetPosX( float( CMenuState::GetInstance( )->GetSlotOne( ).nPositionX ) );
			m_pPlayerOne->SetPosX( float( CMenuState::GetInstance( )->GetSlotOne( ).nPositionX ) );
			pEntry->SetPosY( float( CMenuState::GetInstance( )->GetSlotOne( ).nPositionY ) );
			m_pPlayerOne->SetPosY( float( CMenuState::GetInstance( )->GetSlotOne( ).nPositionY ) );
			m_nPlayerOneScore     = CMenuState::GetInstance( )->GetSlotOne( ).nPlayerOneScore;
			m_nPlayerTwoScore     = CMenuState::GetInstance( )->GetSlotOne( ).nPlayerTwoScore;
			m_nSinglePlayerScore  = CMenuState::GetInstance( )->GetSlotOne( ).nSinglePlayerScore;
		}
		else if( 2 == m_nSlotLoadedFrom )
		{
			pEntry->SetPosX( float( CMenuState::GetInstance( )->GetSlotTwo( ).nPositionX ) );
			m_pPlayerOne->SetPosX( float( CMenuState::GetInstance( )->GetSlotTwo( ).nPositionX ) );
			pEntry->SetPosY( float( CMenuState::GetInstance( )->GetSlotTwo( ).nPositionY ) );
			m_pPlayerOne->SetPosY( float( CMenuState::GetInstance( )->GetSlotTwo( ).nPositionY ) );
			m_nPlayerOneScore     = CMenuState::GetInstance( )->GetSlotTwo( ).nPlayerOneScore;
			m_nPlayerTwoScore     = CMenuState::GetInstance( )->GetSlotTwo( ).nPlayerTwoScore;
			m_nSinglePlayerScore  = CMenuState::GetInstance( )->GetSlotTwo( ).nSinglePlayerScore;
		}
		else if( 3 == m_nSlotLoadedFrom )
		{
			pEntry->SetPosX( float( CMenuState::GetInstance( )->GetSlotThree( ).nPositionX ) );
			m_pPlayerOne->SetPosX( float( CMenuState::GetInstance( )->GetSlotThree( ).nPositionX ) );
			pEntry->SetPosY( float( CMenuState::GetInstance( )->GetSlotThree( ).nPositionY ) );
			m_pPlayerOne->SetPosY( float( CMenuState::GetInstance( )->GetSlotThree( ).nPositionY ) );
			m_nPlayerOneScore     = CMenuState::GetInstance( )->GetSlotThree( ).nPlayerOneScore;
			m_nPlayerTwoScore     = CMenuState::GetInstance( )->GetSlotThree( ).nPlayerTwoScore;
			m_nSinglePlayerScore  = CMenuState::GetInstance( )->GetSlotThree( ).nSinglePlayerScore;
		}
	}
	else
	{
		m_pPlayerOne->SetPosX( pEntry->GetPosX());
		m_pPlayerOne->SetPosY( pEntry->GetPosY());
	}

	if( m_pPlayerTwo)
	Corona_EventHandler::GetInstance()->RegisterClient(this, "P2 OFFSCREEN");
	m_fP2RespawnTimer = -1.0f;
}

bool CGameplayState::Input( )
{
	if( isalpha( CSGD_DirectInput::GetInstance( )->CheckBufferedKeysEx( ) ) )
		CGame::GetInstance( )->SetCheatString( CGame::GetInstance( )->
		GetCheatString( ) += toupper( CSGD_DirectInput::GetInstance( )->CheckBufferedKeysEx( ) ) );

	if( m_pDI->KeyPressed( DIK_P ) || m_pDI->KeyPressed( DIK_ESCAPE ) || m_pDI->JoystickButtonPressed(9) )
	{
		CGame::GetInstance( )->PushState( CPauseMenuState::GetInstance( ) );
		CGame::GetInstance( )->SetPaused( true );
	}


	if( m_pDI->KeyDown( DIK_D ) || m_pDI->JoystickDPadDown(1) )
		m_pPlayerOne->SetVelX( 100 );
	else if( m_pDI->KeyDown( DIK_A )  || m_pDI->JoystickDPadDown(0) )
		m_pPlayerOne->SetVelX( -100 );
	else
		m_pPlayerOne->SetVelX( 0 );

	if( m_pDI->KeyDown( DIK_W ) || m_pDI->JoystickDPadDown(2) )
		m_pPlayerOne->Jump();

	if( m_pDI->KeyDown( DIK_F ) || m_pDI->JoystickButtonDown(1) )
		m_pPlayerOne->Attack(1);



	if( m_pDI->KeyPressed( DIK_1 ) )
		m_pPlayerOne->SetEleType( OBJ_ICE  );
	else if( m_pDI->KeyPressed( DIK_2 ) )
		m_pPlayerOne->SetEleType( OBJ_FIRE );

	if(!m_pPlayerTwo)
	{

		if( m_pDI->KeyPressed( DIK_3 ) )
			m_pPlayerOne->SetEleType( OBJ_EARTH );
		else if( m_pDI->KeyPressed( DIK_4 ) )
			m_pPlayerOne->SetEleType( OBJ_WIND );

		else if(m_pDI->JoystickButtonPressed(4))
			m_pPlayerOne->SetEleType((EleType)(m_pPlayerOne->GetEleType() + OBJ_ICE));
		else if(m_pDI->JoystickButtonPressed(5))
			m_pPlayerOne->SetEleType((EleType)(m_pPlayerOne->GetEleType() - OBJ_ICE));
	}
	else
	{

		//Player One Controls if there are two players:
		if(m_pDI->JoystickButtonPressed(4))
			m_pPlayerOne->SetEleType( OBJ_ICE );
		else if(m_pDI->JoystickButtonPressed(5))
			m_pPlayerOne->SetEleType( OBJ_FIRE );




		//Player Two Controls:
		if(m_pDI->KeyDown( DIK_LEFT) || m_pDI->JoystickDPadDown(0, 1) )
			m_pPlayerTwo->SetVelX( -100 );
		else if( m_pDI->KeyDown( DIK_RIGHT ) || m_pDI->JoystickDPadDown(1, 1)  )
			m_pPlayerTwo->SetVelX( 100 );
		else
			m_pPlayerTwo->SetVelX( 0 );

		if( m_pDI->KeyDown( DIK_UP )  || m_pDI->JoystickDPadDown(2, 1) )
			m_pPlayerTwo->Jump();
		
		if(m_pDI->KeyDown( DIK_RSHIFT ) || m_pDI->JoystickButtonDown(1, 1) )
			m_pPlayerTwo->Attack(1);

		if(m_pDI->JoystickButtonPressed(4,1))
			m_pPlayerTwo->SetEleType( OBJ_WIND );
		else if(m_pDI->JoystickButtonPressed(5,1))
			m_pPlayerTwo->SetEleType( OBJ_EARTH );

	}

	return true;
}

void CGameplayState::Update( float fET )
{
	CProfiler::GetInstance()->Start( "Profiler Start" );
	CProfiler::GetInstance()->Start( "CGameplay Update" );
	CProfiler::GetInstance()->End( "Profiler Start" );

	if( m_pPlayerTwo)
	{
		// Respawn player 2 if he was off screen
		if( m_fP2RespawnTimer >= 0.0f )
		{
			m_fP2RespawnTimer += fET;


			if( m_fP2RespawnTimer > 3.0f )
			{
				// set the timer to a negative number so this code does not run again until the event is trigered
				m_fP2RespawnTimer = -1.0f;

				// set the pos of the p2 to almost the pos of p1, then move the player2 out of player 1 so it looks nice
				m_pPlayerTwo->SetPosX( m_pPlayerOne->GetPosX() + 2);
				m_pPlayerTwo->SetPosY( m_pPlayerOne->GetPosY() );
				m_pPlayerTwo->MoveOutOf( m_pPlayerOne);
			}
		}
	}

	m_pCoM->UpdateObjects(CGame::GetInstance()->GetElapsedTime());
	theLevel.Update(fET);
	m_pPM->Update( fET );
	m_pCeH->ProcessEvents( );

	CProfiler::GetInstance( )->Start( "Profiler End" );
	CProfiler::GetInstance( )->End( "CGameplay Update" );
	CProfiler::GetInstance( )->End( "Profiler End" );
}

void CGameplayState::Render( )
{
	theLevel.Render( );
	m_pCoM->RenderObjects( );
	m_pPM->Render( );

	if( 1 == CMenuState::GetInstance( )->GetPlayerCount( ) )
		m_pTM->Draw( m_nImageID[1], 0, 0 );
	else if( 2 == CMenuState::GetInstance( )->GetPlayerCount( ) )
		m_pTM->Draw( m_nImageID[2], 0, 0 );
}

void CGameplayState::Exit( )
{
	if( m_pPM )
	{
		m_pPM->UnloadAll( );
		m_pPM = NULL;
	}

	if( m_pEF )
	{
		m_pEF->UnloadAll( );
		m_pEF = NULL;
	}

	m_pCeH->SendEvent( "EnemyDied", NULL ); 
	m_pCeH->ProcessEvents( );

	m_pWM->Stop( CGame::GetInstance( )->GetGameBGMusic( ) );
	m_pWM->Reset( CGame::GetInstance( )->GetGameBGMusic( ) );
	m_pWM->Stop( CGame::GetInstance( )->GetMainMenuMusic( ) );
	m_pWM->Reset( CGame::GetInstance( )->GetMainMenuMusic( ) );

	m_pTM->UnloadTexture( m_nImageID[2] );
	m_pTM->UnloadTexture( m_nImageID[1] );
	m_pTM->UnloadTexture( m_nImageID[0] );

	m_pCoM->RemoveAllObjects( );

	if( m_pPlayerOne )
		m_pPlayerOne->Release( );
	if(  m_pPlayerTwo )
		m_pPlayerTwo->Release( );

	theCamera->ShutDownCamera( );
	theCamera = NULL;

	CCamera::GetCamera( )->ShutDownCamera( );

	theLevel.Clear( );
	m_pCoM->DeleteInstance( );
	CAnimationWarehouse::GetInstance()->DeleteInstance( );
	// TODO :: Remove this comment if you don't need it!
	//CProfiler::GetInstance( )->DeleteInstance( );

	m_bLoadedFromFile = false;
	m_bTwoPlayers	  = false;

	
	if( m_pPlayerTwo)
	Corona_EventHandler::GetInstance()->UnregisterClient("P2 OFFSCREEN", this );

#if _DEBUG
	CProfiler::GetInstance( )->Save( "CodeProfilerOutput.txt" );
	CProfiler::GetInstance( )->DeleteInstance( );
#endif
}



void CGameplayState::HandleEvent(CEvent* pEvent)
{
	if(!strcmp(pEvent->GetEventID().c_str(), "P2 OFFSCREEN"))
	{
		if( m_fP2RespawnTimer == -1.0f)
		{
		m_fP2RespawnTimer = 0.0f;
		// TODO: create some particle effet that las 3 secs from the pos the player 2 was to where player 1 is
		CEmitter* emmiter ;//= new CEmitter();
		emmiter = CEmitterFactory::GetInstance()->CreateEmitter( "return" );

		emmiter->SetPosX(m_pPlayerTwo->GetPosX());
		emmiter->SetPosY(m_pPlayerTwo->GetPosY());

		TVECTOR direction;
		direction.x = (m_pPlayerOne->GetPosX() - m_pPlayerTwo->GetPosX());
		direction.y = (m_pPlayerOne->GetPosY() - m_pPlayerTwo->GetPosY());
		direction.z = 0.0f;
		direction.w = 0.0f;

		direction = Lapidem_Math::GetInstance()->VectorNormalize( direction );
		
		emmiter->SetVelX(direction.x * (m_pPlayerOne->GetPosX() - m_pPlayerTwo->GetPosX())/1.5f);
		emmiter->SetVelY(direction.y * (m_pPlayerOne->GetPosY() - m_pPlayerTwo->GetPosY())/1.5f);
		CParticleManager::GetInstance( )->AddEmitter( emmiter );



		}
	}

}