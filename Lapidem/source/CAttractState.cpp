//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CAttractState.cpp"
//
//  Author Name :   Mike Hatter
//
//  Purpose     :   
//////////////////////////////////////////////////////////////////////////
#include "CAttractState.h"

CAttractState* CAttractState::GetInstance( )
{
	static CAttractState instance;
	return &instance;
}

void CAttractState::Enter( )
{
	m_pD3D          = CSGD_Direct3D::GetInstance( );
	m_pTM           = CSGD_TextureManager::GetInstance( );
	m_pDS           = CSGD_DirectSound::GetInstance( );
	m_pWM           = CSGD_WaveManager::GetInstance( );
	m_pDI           = CSGD_DirectInput::GetInstance( );

	ShowCursor( false );

	m_pGraphBuilder = NULL;
	m_pMediaControl = NULL;
	m_pVideoWindow  = NULL;
	m_pMediaEvent   = NULL;

	PlayFile( );
}

bool CAttractState::Input( )
{
	if( m_pDI->CheckBufferedKeysEx( ) || m_pDI->JoystickCheckBufferedButtons() != -1)
	{
		m_pMediaControl->Stop( );
		CMenuState::GetInstance( )->ResetAttractTimer( );
		CGame::GetInstance( )->PopState( );
	}
	else if( m_pDI->KeyPressed( DIK_UP ) || 
		m_pDI->KeyPressed( DIK_DOWN ) || 
		m_pDI->KeyPressed( DIK_LEFT ) || 
		m_pDI->KeyPressed( DIK_RIGHT ) )
	{
		m_pMediaControl->Stop( );
		CMenuState::GetInstance( )->ResetAttractTimer( );
		CGame::GetInstance( )->PopState( );
	}

	return true;
}

void CAttractState::Update( float fET )
{
}

void CAttractState::Render( )
{
}

void CAttractState::Exit( )
{
	if( m_pMediaControl )
	{
		m_pMediaControl->Stop( );
		m_pMediaControl->Release( );
		m_pMediaControl = NULL;
	}

	if( m_pVideoWindow )
	{
		m_pVideoWindow->put_Visible( OAFALSE );
		m_pVideoWindow->put_Owner( NULL );
		m_pVideoWindow->Release( );
		m_pVideoWindow = NULL;
	}

	if( m_pMediaEvent )
	{
		m_pMediaEvent->Release( );
		m_pMediaEvent = NULL;
	}

	if( m_pGraphBuilder )
	{
		m_pGraphBuilder->Release( );
		m_pGraphBuilder = NULL;
	}

	ShowCursor( true );
	CMenuState::GetInstance( )->ResetAttractTimer( );
}

void CAttractState::PlayFile( )
{
	CoCreateInstance( CLSID_FilterGraph, NULL, CLSCTX_INPROC,
		IID_IGraphBuilder, ( void** )&m_pGraphBuilder );

	m_pGraphBuilder->RenderFile( L"resource/video/Lapidem_Attract.mpg", NULL );

	m_pGraphBuilder->QueryInterface( IID_IVideoWindow, ( void** )&m_pVideoWindow );
	m_pVideoWindow->put_Owner( ( OAHWND )main_hwnd );
	m_pVideoWindow->put_WindowStyle( WS_CHILD | WS_CLIPSIBLINGS );

	RECT rFillWindow;
	GetClientRect( main_hwnd, &rFillWindow );
	m_pVideoWindow->SetWindowPosition( 0, 0, rFillWindow.right, rFillWindow.bottom );

	m_pGraphBuilder->QueryInterface( IID_IMediaEventEx, ( void** )&m_pMediaEvent );
	m_pMediaEvent->SetNotifyWindow( ( OAHWND )main_hwnd, WM_GRAPHNOTIFY, 0 );

	m_pGraphBuilder->QueryInterface( IID_IMediaControl, ( void** )&m_pMediaControl );
	m_pMediaControl->Run( );
}

void CAttractState::HandlePlayEvent( )
{
	long evCode, param1, param2;
	HRESULT result;

	while( result = m_pMediaEvent->GetEvent( &evCode, &param1, &param2, 0 ), SUCCEEDED( result ) )
	{
		result = m_pMediaEvent->FreeEventParams( evCode, param1, param2 );

		if( ( evCode == EC_COMPLETE ) || ( evCode == EC_USERABORT ) )
		{
			CGame::GetInstance( )->PopState( );
			CMenuState::GetInstance( )->ResetAttractTimer( );
			break;
		}
	}
}