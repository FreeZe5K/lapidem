#include "CGameplayState.h"
#include "CCamera.h"
#include "CPlayer.h"
#include "Corona_ObjectManager.h"
#include "Corona_EventHandler.h"
#include "CProfiler.h"

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

	m_pPlayerOne	= new CPlayer();
	m_pPlayerTwo	= NULL;

	CCamera::InitCamera(0.0f, 0.0f, (float)CGame::GetInstance()->GetScreenWidth(), (float)CGame::GetInstance()->GetScreenHeight(),
						CGame::GetInstance()->GetScreenWidth() * .66f, CGame::GetInstance()->GetScreenHeight() * .66f, m_pPlayerOne );

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

	theLevel.LoadNewLevel("resource/data/textlvl2.laplvl");
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

	if( m_pDI->KeyPressed( DIK_F ) )
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

void CGameplayState::Update( )
{
	m_pCoM->UpdateObjects(CGame::GetInstance()->GetElapsedTime());
}

void CGameplayState::Render( )
{
	m_pTM->Draw( m_nImageID, 0, 0, 1.0f, 1.0f, 
		NULL, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB( 200, 255, 255, 255 ) );
	theLevel.RenderBackGround();
	m_pCoM->RenderObjects();
}

void CGameplayState::Exit( )
{
	m_pWM->UnloadWave( m_nSoundID[1] );
	m_pWM->UnloadWave( m_nSoundID[0] );
	m_pTM->UnloadTexture( m_nImageID );
	if(m_pPlayerOne)
	m_pPlayerOne->Release();
	if(m_pPlayerTwo)
	m_pPlayerTwo->Release();
	Corona_ObjectManager::GetInstance()->RemoveAllObjects();
	theLevel.Clear();

#if _DEBUG
	CProfiler::GetInstance()->Save("CodeProfilerOutput.txt");
#endif

}