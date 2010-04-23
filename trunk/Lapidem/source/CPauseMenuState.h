#pragma once

#include "IGameState.h"
#include "CGame.h"

class CPauseMenuState : public IGameState
{
private:
	int                     m_nChoice;
	int                     m_nImageID;
	int                     m_nSoundID;

	CSGD_Direct3D           *m_pD3D;
	CSGD_TextureManager     *m_pTM;
	CSGD_DirectSound        *m_pDS;
	CSGD_WaveManager        *m_pWM;
	CSGD_DirectInput        *m_pDI;

	CPauseMenuState( )  { /* DO NOTHING */ }
	~CPauseMenuState( ) { /* DO NOTHING */ }
	CPauseMenuState( const CPauseMenuState& );
	CPauseMenuState& operator=( const CPauseMenuState& );

public:
	static CPauseMenuState* GetInstance( );

	bool SaveGame( const char* );

	void Enter( );
	bool Input( );
	void Update( );
	void Render( );
	void Exit( );
};