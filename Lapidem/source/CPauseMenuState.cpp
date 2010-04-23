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

	m_nImageID      = m_pTM->LoadTexture( "resource/graphics/Lapidem_Paused.png");
}

bool CPauseMenuState::Input( )
{
	if( m_pDI->KeyPressed( DIK_UP ) )
	{
		--m_nChoice;

		if( m_nChoice < 0 ) 
			m_nChoice = 3;
	}

	if( m_pDI->KeyPressed( DIK_DOWN ) )
	{
		++m_nChoice;

		if( m_nChoice > 3 )
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
		}
		else if( m_nChoice == 2 ) // Save and quit
		{
			//SaveGame( "resource/data/config.xml" );
			CGame::GetInstance( )->SetPaused( false );
			CGame::GetInstance( )->ChangeState( CMenuState::GetInstance( ) );
		}
		else // Anything that isn't already an option ( or in case of stupid user ); exit to main menu.
		{
			CGame::GetInstance( )->SetPaused( false );
			CGame::GetInstance( )->ChangeState( CMenuState::GetInstance( ) );
		}
	}

	return true;
}

void CPauseMenuState::Update( )
{
}

void CPauseMenuState::Render( )
{
	m_pTM->Draw( m_nImageID, 0, 0, 1.0f );

	if( m_nChoice == 0 )
		CGame::GetInstance( )->GetFont( )->Draw( ">", 195, 160, 1.0f, D3DCOLOR_XRGB( 255, 255, 255 ) );
	else if( m_nChoice == 1 )
		CGame::GetInstance( )->GetFont( )->Draw( ">", 195, 192, 1.0f, D3DCOLOR_XRGB( 255, 255, 255 ) );
	else if( m_nChoice == 2 )
		CGame::GetInstance( )->GetFont( )->Draw( ">", 135, 222, 1.0f, D3DCOLOR_XRGB( 255, 255, 255 ) );
	else if( m_nChoice == 3 )
		CGame::GetInstance( )->GetFont( )->Draw( ">", 177, 252, 1.0f, D3DCOLOR_XRGB( 255, 255, 255 ) );
}

void CPauseMenuState::Exit( )
{
}

bool CPauseMenuState::SaveGame( const char* szFile )
{
	return true;
}