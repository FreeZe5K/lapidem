#pragma once

#include "CGame.h"
#include "IGameState.h"

class IGameState;

class CGameplayState : public IGameState
{
private:
	int                     m_nImageID;
	int                     m_nSoundID[2];

	bool                    m_bIsPaused;

	CSGD_Direct3D           *m_pD3D;
	CSGD_TextureManager     *m_pTM;
	CSGD_DirectSound        *m_pDS;
	CSGD_WaveManager        *m_pWM;
	CSGD_DirectInput        *m_pDI;

	CGameplayState( )  { /* DO NOTHING */ }
	~CGameplayState( ) { /* DO NOTHING */ }
	CGameplayState( const CGameplayState& );
	CGameplayState& operator=( const CGameplayState& );

public:
	static CGameplayState* GetInstance( );

	void Enter( );
	bool Input( );
	void Update( );
	void Render( );
	void Exit( );
};