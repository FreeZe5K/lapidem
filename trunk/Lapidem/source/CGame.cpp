//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CGame.cpp"
//
//  Author Name :   Mike Hatter
//
//  Purpose     :   The main game loop.
//////////////////////////////////////////////////////////////////////////
#include "CGame.h"
#include "CSpellFactory.h"
CGame *CGame::GetInstance( )
{
	static CGame instance;
	return &instance;
}

CGame::CGame( )
{
	m_pD3D      = NULL;
	m_pTM       = NULL;
	m_pDS       = NULL;
	m_pWM       = NULL;
	m_pDI       = NULL;
	m_bmFont    = NULL;

	m_nImageID	             = -1;
	m_nSoundID	             = -1;

	m_nMusicVolume           = 65;
	m_nSoundEffectVolume     = 100;

	m_bIsInDebug             = false;
	m_bIsPaused              = false;
	m_bIsNotFullscreen       = true;
}

void CGame::Initialize( HWND hWnd, HINSTANCE hInstance, 
					   int nScreenWidth, int nScreenHeight, bool bIsWindowed )
{
	// Get pointers to singletons
	m_pD3D  = CSGD_Direct3D::GetInstance( );
	m_pTM   = CSGD_TextureManager::GetInstance( );
	m_pDS   = CSGD_DirectSound::GetInstance( );
	m_pWM   = CSGD_WaveManager::GetInstance( );
	m_pDI   = CSGD_DirectInput::GetInstance( );

	// Initialize singletons
	m_pD3D->InitDirect3D( hWnd, nScreenWidth, nScreenHeight, bIsWindowed );
	m_pTM->InitTextureManager( m_pD3D->GetDirect3DDevice( ), m_pD3D->GetSprite( ) );
	m_pDS->InitDirectSound( hWnd );
	m_pWM->InitWaveManager( hWnd, m_pDS->GetDSObject( ) );
	m_pDI->InitDirectInput( hWnd, hInstance, DI_KEYBOARD );

	m_nScreenWidth        = nScreenWidth;
	m_nScreenHeight       = nScreenHeight;
	m_bIsNotFullscreen    = bIsWindowed;

	m_bmFont              = new CBitmapFont();
	m_bmFont->Load( "resource/graphics/Lapidem_Font.bmp", "resource/data/Lapidem_BitmapFontSizes.txt" );

	m_dwTimeStamp         = GetTickCount( );
	m_dwPreviousTimeStamp = GetTickCount( );

	ChangeState( CMenuState::GetInstance( ) );
}

void CGame::Shutdown( )
{
	
	ChangeState( NULL );
	CSpellFactory::GetInstance()->DeleteInstance();

	if( m_bmFont )
	{
		delete m_bmFont;
		m_bmFont = NULL;
	}

	if( m_pDI )
	{
		m_pDI->ShutdownDirectInput( );
		m_pDI = NULL;
	}

	if( m_pWM )
	{
		m_pWM->ShutdownWaveManager( );
		m_pWM = NULL;
	}

	if( m_pDS )
	{
		m_pDS->ShutdownDirectSound( );
		m_pDS = NULL;
	}

	if( m_pTM )
	{
		m_pTM->ShutdownTextureManager( );
		m_pTM = NULL;
	}

	if( m_pD3D )
	{
		m_pD3D->ShutdownDirect3D( );
		m_pD3D = NULL;
	}
}

bool CGame::Main( )
{
	DWORD dwStartTimeStamp = GetTickCount( );
	m_fElapsedTime         = float( dwStartTimeStamp - m_dwPreviousTimeStamp ) / 1000.0f;
	m_dwPreviousTimeStamp  = dwStartTimeStamp;


#ifdef _DEBUG
	if(m_fElapsedTime > .25f)
		m_fElapsedTime = .25f;
#endif

	if( !Input( ) ) 
		return false;

	CheckForCheats( );

	Update( float( m_fElapsedTime ) );
	Render( );

	m_nFrameCount++;

	if( GetTickCount( ) - m_dwFPSTimeStamp > 1000 )
	{
		m_nFPS              = m_nFrameCount;
		m_nFrameCount       = 0;
		m_dwFPSTimeStamp    = GetTickCount( );
	}

	return true;
}

bool CGame::Input( )
{
	m_pDI->ReadDevices( );

	if( ( m_pDI->KeyPressed( DIK_RETURN ) ) && 
		( m_pDI->KeyDown( DIK_LALT ) ||  m_pDI->KeyDown( DIK_RALT ) ) )
	{
		m_bIsNotFullscreen = !m_bIsNotFullscreen;
		m_pD3D->ChangeDisplayParam( m_nScreenWidth, m_nScreenHeight, m_bIsNotFullscreen );
		m_pDI->ReadDevices( );
	}

	if( m_vGS.size( ) > 0 )
		if( !m_vGS[m_vGS.size( ) - 1]->Input( ) )
			return false;
	return true;
}

void CGame::Update( float _fps )
{
	if( !m_bIsPaused )
		if( m_vGS.size( ) > 0 )
			for( unsigned int index = 0; index < m_vGS.size( ); index++ )
				m_vGS[index]->Update( _fps );
	m_pWM->Update( );
}

void CGame::Render( )
{
	m_pD3D->Clear( 0, 0, 0 );
	m_pD3D->DeviceBegin( );
	m_pD3D->SpriteBegin( );

	if( m_vGS.size( ) > 0 )
		for( unsigned int index = 0; index < m_vGS.size( ); index++ )
			m_vGS[index]->Render( );

	m_pD3D->SpriteEnd( );
	m_pD3D->DeviceEnd( );
	m_pD3D->Present( );
}

void CGame::ChangeState( IGameState* newState )
{
	while( m_vGS.size( ) > 0 ) PopState( );
	if( newState != NULL ) PushState( newState );
}

void CGame::PushState( IGameState* newState )
{
	m_vGS.push_back( newState );
	m_vGS.back( )->Enter( );
}

void CGame::PopState( )
{
	m_vGS.back( )->Exit( );
	m_vGS.pop_back( );
}

void CGame::CheckForCheats( )
{
	if( strstr( m_szCHEATSTRING.c_str( ), "DEBUG" ) )
	{
		m_szCHEATSTRING.clear( );
		m_bIsInDebug = !m_bIsInDebug;
	}
}