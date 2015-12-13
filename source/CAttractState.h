#pragma once
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
	int                     m_nSSImageID[5];
	int	                    m_nCurrentPosition;
	int                     m_nTimer;
	bool                    m_bSecond;
	bool                    m_bAllowedToPlay;

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

	bool GetOneSecond( )     { return m_bSecond; }
	void SetOneSecond( )     { m_bSecond = true; }

	void Enter( );
	bool Input( );
	void Update( float );
	void Render( );
	void Exit( );
};