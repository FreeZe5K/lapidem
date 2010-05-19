//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CAuxiliaryState.h"
//
//  Author Name :   Mike Hatter
//////////////////////////////////////////////////////////////////////////
#pragma once

#include "CGame.h"
#include "IGameState.h"
#include "CHighScores.h"

using std::fstream;
using std::ios;
using std::string;

class CGame;
class IGameState;

class CAuxiliaryState : public IGameState
{
private:
	CSGD_Direct3D           *m_pD3D;
	CSGD_TextureManager     *m_pTM;
	CSGD_DirectSound        *m_pDS;
	CSGD_WaveManager        *m_pWM;
	CSGD_DirectInput        *m_pDI;

	CHighScores				m_HS;

	int                     m_nState;
	int                     m_nChoice;
	int                     m_nImageID[2];
	int                     m_nCreditScroll;

	/////////////////////////
	// High score info
	string                  m_szPlayerNames[10];
	int                     m_nPlayerScores[10];

	CAuxiliaryState( )  { /* DO NOTHING */ }
	~CAuxiliaryState( ) { /* DO NOTHING */ }
	CAuxiliaryState( const CAuxiliaryState& );
	CAuxiliaryState& operator=( const CAuxiliaryState& );

public:
	static CAuxiliaryState* GetInstance( );

	void SetMenuState( int _i ) { m_nState = _i; }

	bool LoadConfig( const char* );
	bool SaveConfig( const char* );

	void Enter( );
	bool Input( );
	void Update( float );
	void Render( );
	void Exit( );
};