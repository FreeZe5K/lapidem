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

	CAnimationWarehouse::GetInstance()->LoadAnimationSet("resource/idlewalk.Anim",D3DCOLOR_XRGB(255,255,255));

	m_pPlayerOne	= new CPlayer();
	m_pPlayerTwo	= NULL;

	CCamera::InitCamera(0.0f, 0.0f, (float)CGame::GetInstance()->GetScreenWidth(),
						(float)CGame::GetInstance()->GetScreenHeight(), m_pPlayerOne );

	m_pCoM			= Corona_ObjectManager::GetInstance();
	m_pCeH			= Corona_EventHandler::GetInstance();


	// - - - - - - - - - - - - - -
	// Change the background image.
	// - - - - - - - - - - - - - -
	/* Note by Pablo
		Background image will be handled by the CLevel.
	*/
	m_nImageID      = m_pTM->LoadTexture( "resource/graphics/placeholderArt.png" );
	// - - - - - - - - - - - - - -

	m_nSoundID[0]   = m_pWM->LoadWave( "resource/audio/Lapidem_LevelOneMusic.wav" );
	m_nSoundID[1]   = m_pWM->LoadWave( "resource/audio/Lapidem_MainMenuTick.wav" );

	m_pWM->Play( m_nSoundID[0], DSBPLAY_LOOPING );
	m_pWM->SetVolume( m_nSoundID[0], CGame::GetInstance( )->GetMusicVolume( ) ); 
	m_pWM->SetVolume( m_nSoundID[1], CGame::GetInstance( )->GetSoundFXVolume( ) ); 

	{//Player One Init (For Testing!):

		m_pPlayerOne->SetImage(m_pTM->LoadTexture( "resource/graphics/Lapidem_TempJack.png"));
		m_pPlayerOne->SetPosX(200);
		m_pPlayerOne->SetPosY(200);
		m_pPlayerOne->SetWidth(16);
		m_pPlayerOne->SetHeight(64);

	}

	m_pCoM->AddObject(m_pPlayerOne);

	if(m_pPlayerTwo)
		m_pCoM->AddObject(m_pPlayerTwo);

	m_pPM = CParticleManager::GetInstance( );
	m_pEF = CEmitterFactory::GetInstance( );
	m_pEF->Initialize( );

	theLevel.LoadNewLevel("resource/data/level1.laplvl");
	

	CBase* pEntry = theLevel.GetEntryPoint();
	m_pPlayerOne->SetPosX( pEntry->GetPosX());
	m_pPlayerOne->SetPosY( pEntry->GetPosY());
}

bool CGameplayState::Input( )
{
	// - - - - - - - - - - - - - -
	// Remove this code when
	// functionality is added.
	// Sam:: Why? We need a pause =).
	// - - - - - - - - - - - - - -
	if( m_pDI->KeyPressed( DIK_P ) || m_pDI->KeyPressed( DIK_ESCAPE ) )
	{
		CGame::GetInstance( )->PushState( CPauseMenuState::GetInstance( ) );
		CGame::GetInstance( )->SetPaused( true );
	}
	// - - - - - - - - - - - - - -

	if( m_pDI->KeyDown( DIK_D ) )
		m_pPlayerOne->SetVelX( 100 );
	else if( m_pDI->KeyDown( DIK_A ) )
		m_pPlayerOne->SetVelX( -100 );
	else
		m_pPlayerOne->SetVelX( 0 );

	if( m_pDI->KeyDown( DIK_W ) )
		m_pPlayerOne->Jump();

	if( m_pDI->KeyDown( DIK_F ) )
		m_pPlayerOne->Attack(1);


	if( m_pPlayerTwo )
	{
		if(m_pDI->KeyDown( DIK_LEFT) )
			m_pPlayerTwo->SetVelX( -100 );
		else if( m_pDI->KeyDown( DIK_RIGHT ) )
			m_pPlayerTwo->SetVelX( 100 );
		else
			m_pPlayerTwo->SetVelX( 0 );

		if( m_pDI->KeyDown( DIK_UP ) )
			m_pPlayerTwo->Jump();
	}

	return true;
}

void CGameplayState::Update( float fET )
{
	m_pCoM->UpdateObjects(CGame::GetInstance()->GetElapsedTime());
	m_pPM->Update( fET );
	m_pCeH->ProcessEvents();
}

void CGameplayState::Render( )
{
	theLevel.RenderBackGround();
	m_pCoM->RenderObjects();
	m_pPM->Render( );
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

	m_pWM->UnloadWave( m_nSoundID[1] );
	m_pWM->UnloadWave( m_nSoundID[0] );
	m_pTM->UnloadTexture( m_nImageID );
	if(m_pPlayerOne)
	m_pPlayerOne->Release();
	if(m_pPlayerTwo)
	m_pPlayerTwo->Release();
	theCamera->ShutDownCamera();
	theCamera = NULL;
	m_pCoM->RemoveAllObjects();
	CCamera::GetCamera()->ShutDownCamera();
	//m_pCeH->ShutDownSystem();
	
	theLevel.Clear();
	m_pCoM->DeleteInstance();

#if _DEBUG
	CProfiler::GetInstance()->Save("CodeProfilerOutput.txt");
	CProfiler::GetInstance()->DeleteInstance();
#endif
}