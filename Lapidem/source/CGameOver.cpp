#include "CGameOver.h"

CGameOver* CGameOver::GetInstance( )
{
	static CGameOver instance;
	return &instance;
}

void CGameOver::Enter( )
{
	m_pD3D              = CSGD_Direct3D::GetInstance( );
	m_pTM               = CSGD_TextureManager::GetInstance( );
	m_pDS               = CSGD_DirectSound::GetInstance( );
	m_pWM               = CSGD_WaveManager::GetInstance( );
	m_pDI               = CSGD_DirectInput::GetInstance( );

	m_nExitTimer        = 0;
	m_bIsAllowedToExit  = false;

	m_nImageID[0]       = m_pTM->LoadTexture( "resource/graphics/Lapidem_YouDied.png" );
	m_nImageID[1]       = m_pTM->LoadTexture( "resource/graphics/Lapidem_YouWon.png" );

	m_pWM->SetVolume( CGame::GetInstance( )->GetLostMusic( ), 
		CGame::GetInstance( )->GetMusicVolume( ) );
	m_pWM->SetVolume( CGame::GetInstance( )->GetVictoryMusic( ), 
		CGame::GetInstance( )->GetSoundFXVolume( ) );

	if( 1 == m_nState )
		m_pWM->Play( CGame::GetInstance( )->GetLostMusic( ) );
	else if( 2 == m_nState )
		m_pWM->Play( CGame::GetInstance( )->GetVictoryMusic( ) );

	m_HS.Load( "resource/data/scores.bin" );


	m_CurrScore.score =   CGameplayState::GetInstance()->GetSinglePlayerScore();

	sprintf_s( m_CurrScore.name, sizeof(char)*4, "AAA" );
	m_nLetterSelection = 0;


}

bool CGameOver::Input( )
{

		// Set the state to high scores
		/*if( m_bIsAllowedToExit )
		{
		CAuxiliaryState::GetInstance( )->SetMenuState( 1 );
		CGame::GetInstance( )->ChangeState( CAuxiliaryState::GetInstance( ) );
		}*/

		if( m_pDI->KeyPressedEx( DIK_RETURN) ||m_pDI->KeyPressedEx( DIK_ESCAPE) || m_pDI->KeyPressedEx( DIK_SPACE ) )
		{
			if( m_HS.Try( m_CurrScore.score ) )
			{
				m_HS.Add( m_CurrScore.score, m_CurrScore.name );
			}

		CAuxiliaryState::GetInstance( )->SetMenuState( 1 );
		CGame::GetInstance( )->ChangeState( CAuxiliaryState::GetInstance( ) );

		}

		if( m_pDI->KeyPressedEx ( DIK_UP ) )
			m_CurrScore.name[m_nLetterSelection] += 1;
		else if( m_pDI->KeyPressedEx ( DIK_DOWN ) )
			m_CurrScore.name[m_nLetterSelection] -= 1;

		else if( m_pDI->KeyPressedEx ( DIK_LEFT ) )
			m_nLetterSelection--;
		else if( m_pDI->KeyPressedEx ( DIK_RIGHT ) )
			m_nLetterSelection++;	
	
	

	return true;
}

void CGameOver::Update( float fDT )
{
	if( ++m_nExitTimer > 250 )
		m_bIsAllowedToExit = true;



	if( m_nLetterSelection > 2 )
		m_nLetterSelection = 0;
	else if( m_nLetterSelection < 0 )
		m_nLetterSelection = 2;

	if( m_CurrScore.name[m_nLetterSelection] > 'Z' )
		m_CurrScore.name[m_nLetterSelection] = 'A';
	else if( m_CurrScore.name[m_nLetterSelection] < 'A' )
		m_CurrScore.name[m_nLetterSelection] = 'Z';


}

void CGameOver::Render( )
{
	if( 1 == m_nState ) // Player lost
	{
		m_pTM->Draw( m_nImageID[0], 0, 0 );
		
	}
	else if( 2 == m_nState ) // Player won
	{
		m_pTM->Draw( m_nImageID[1], 0, 0 );

	}
		char cBuffer[128];
		char szFirstLetter[] = "a";
		char szSecondLetter[] = "a";
		char szThirdLetter[] = "a";

		szFirstLetter[0] = m_CurrScore.name[0];
		szSecondLetter[0] = m_CurrScore.name[1];
		szThirdLetter[0] = m_CurrScore.name[2];

		switch( m_nLetterSelection )
		{
		case 0:

			CGame::GetInstance( )->GetFont( )->Draw( szFirstLetter, 
				220, 400, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( szSecondLetter, 
				240, 400, 0.7f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( szThirdLetter, 
				260, 400, 0.7f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			break;
		case 1:

			CGame::GetInstance( )->GetFont( )->Draw( szFirstLetter, 
				220, 400, 0.7f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( szSecondLetter, 
				240, 400, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( szThirdLetter, 
				260, 400, 0.7f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			break;
		case 2:

			CGame::GetInstance( )->GetFont( )->Draw( szFirstLetter, 
				220, 400, 0.7f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( szSecondLetter, 
				240, 400, 0.7f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			CGame::GetInstance( )->GetFont( )->Draw( szThirdLetter, 
				260, 400, 1.0f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			break;

		};


		sprintf_s( cBuffer, "%i", m_CurrScore.score );
		//CGame::GetInstance( )->GetFont( )->Draw( m_CurrScore.name, 
		//		220, 400, 0.7f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		CGame::GetInstance( )->GetFont( )->Draw( cBuffer, 
			420, 400, 0.7f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		

}

void CGameOver::Exit( )
{
	m_bIsAllowedToExit = false;
	m_pTM->UnloadTexture( m_nImageID[1] );
	m_pTM->UnloadTexture( m_nImageID[0] );

	m_HS.Save( "resource/data/scores.bin" );

}