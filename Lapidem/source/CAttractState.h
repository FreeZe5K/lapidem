#pragma once
#pragma comment(lib, "strmiids.lib")

#define WM_GRAPHNOTIFY      WM_APP + 1

#include "IGameState.h"
#include "CGame.h"
#include <DShow.h>

class IGameState;
class CSGD_Direct3D;
class CSGD_TextureManager;
class CSGD_DirectSound;
class CSGD_WaveManager;
class CSGD_DirectInput;

class CAttractState : public IGameState
{
private:
	HWND                    main_hwnd;

	IGraphBuilder           *m_pGraphBuilder;
	IMediaControl           *m_pMediaControl;
	IVideoWindow            *m_pVideoWindow;
	IMediaEventEx           *m_pMediaEvent;

	CSGD_Direct3D           *m_pD3D;
	CSGD_DirectInput        *m_pDI;
	CSGD_TextureManager     *m_pTM;
	CSGD_DirectSound        *m_pDS;
	CSGD_WaveManager        *m_pWM;

	CAttractState( )  { /* DO NOTHING */ }
	~CAttractState( ) { /* DO NOTHING */ }
	CAttractState( const CAttractState& );
	CAttractState& operator=( const CAttractState& );

public:
	static CAttractState* GetInstance( );

	void SetHWND( HWND _h )      { main_hwnd = _h;     }
	HWND GetHWND( )              { return main_hwnd;   }

	void PlayFile( );
	void HandlePlayEvent( );

	void Enter( );
	bool Input( );
	void Update( float );
	void Render( );
	void Exit( );
};