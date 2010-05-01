//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CAuxiliaryState.h"
//
//  Author Name :   Mike Hatter
//////////////////////////////////////////////////////////////////////////
#pragma once

#include "CGame.h"
#include "IGameState.h"

using std::fstream;
using std::ios;
using std::string;

class CGame;
class IGameState;

class CAuxiliaryState : public IGameState
{
private:
	int                     m_nState;
	int                     m_nChoice;
	int                     m_nImageID[2];
	int                     m_nSoundID[2];
	int                     m_nCreditScroll;

	int                     m_nScrollSpeedOne;
	int                     m_nScrollSpeedTwo;
	int                     m_nScrollSpeedThree;
	int                     m_nScrollSpeedFour;

	/////////////////////////
	// High score info
	string                  m_szPlayerNames[10];
	int                     m_nPlayerScores[10];

	CSGD_Direct3D           *m_pD3D;
	CSGD_TextureManager     *m_pTM;
	CSGD_DirectSound        *m_pDS;
	CSGD_WaveManager        *m_pWM;
	CSGD_DirectInput        *m_pDI;

	CAuxiliaryState( )  { /* DO NOTHING */ }
	~CAuxiliaryState( ) { /* DO NOTHING */ }
	CAuxiliaryState( const CAuxiliaryState& );
	CAuxiliaryState& operator=( const CAuxiliaryState& );

public:
	static CAuxiliaryState* GetInstance( );

	void SetMenuState( int _i ) { m_nState = _i; }

	void Enter( );
	bool Input( );
	void Update( float );
	void Render( );
	void Exit( );
};