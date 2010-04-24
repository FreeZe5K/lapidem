#include "CGameplayState.h"
#include "CCamera.h"
#include "CPlayer.h"
#include "Corona_ObjectManager.h"

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


	// - - - - - - - - - - - - - -
	// Change the background image.
	// - - - - - - - - - - - - - -
	m_nImageID      = m_pTM->LoadTexture( "resource/graphics/placeholderArt.png" );
	// - - - - - - - - - - - - - -

	m_nSoundID[0]   = m_pWM->LoadWave( "resource/audio/Lapidem_LevelOneMusic.wav" );
	m_nSoundID[1]   = m_pWM->LoadWave( "resource/audio/Lapidem_MainMenuTick.wav" );

	m_pWM->Play( m_nSoundID[0], DSBPLAY_LOOPING );
	m_pWM->SetVolume( m_nSoundID[0], CGame::GetInstance( )->GetMusicVolume( ) ); 
	m_pWM->SetVolume( m_nSoundID[1], CGame::GetInstance( )->GetSoundFXVolume( ) ); 

	m_pCoM->AddObject(m_pPlayerOne);

	if(m_pPlayerTwo)
		m_pCoM->AddObject(m_pPlayerTwo);
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
	m_pCoM->RenderObjects();
}

void CGameplayState::Exit( )
{
	m_pWM->UnloadWave( m_nSoundID[1] );
	m_pWM->UnloadWave( m_nSoundID[0] );
	m_pTM->UnloadTexture( m_nImageID );
}