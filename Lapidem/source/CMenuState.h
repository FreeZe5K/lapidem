//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CMenuState.h"
//
//  Author Name :   Mike Hatter
//////////////////////////////////////////////////////////////////////////
#pragma once

#include "CGame.h"
#include "IGameState.h"

class CGame;
class IGameState;

class CMenuState : public IGameState
{
private:
	int                     m_nChoice;
	int                     m_nImageID;
	int                     m_nSoundID[2];
	int                     m_nAttractTimer;

	int                     m_nScrollSpeedOne;
	int                     m_nScrollSpeedTwo;
	int                     m_nScrollSpeedThree;
	int                     m_nScrollSpeedFour;

	CSGD_Direct3D           *m_pD3D;
	CSGD_TextureManager     *m_pTM;
	CSGD_DirectSound        *m_pDS;
	CSGD_WaveManager        *m_pWM;
	CSGD_DirectInput        *m_pDI;

	CMenuState( )  { /* DO NOTHING */ }
	~CMenuState( ) { /* DO NOTHING */ }
	CMenuState( const CMenuState& );
	CMenuState& operator=( const CMenuState& );

public:
	static CMenuState* GetInstance( );

	void Enter( );
	bool Input( );
	void Update( );
	void Render( );
	void Exit( );
};