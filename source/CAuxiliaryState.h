//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CAuxiliaryState.h"
//
//  Author Name :   Mike Hatter
//////////////////////////////////////////////////////////////////////////
#pragma once

#include "CGame.h"
#include "IGameState.h"
#include "CHighScores.h"
#include "CPauseMenuState.h"
#include "CGameplayState.h"

using std::fstream;
using std::ios;
using std::string;

class CGame;
class IGameState;

class CAuxiliaryState : public IGameState
{
private:
	struct tSlotInfo
	{
		int _nNumPlayers;
		int _nCurrentLevel;

		int _nPlayerOneHealth;
		int _nPlayerOneScore;

		int _nPlayerTwoHealth;
		int _nPlayerTwoScore;
	} _tSlotOne, _tSlotTwo, _tSlotThree;

	CSGD_Direct3D           *m_pD3D;
	CSGD_TextureManager     *m_pTM;
	CSGD_DirectSound        *m_pDS;
	CSGD_WaveManager        *m_pWM;
	CSGD_DirectInput        *m_pDI;

	CHighScores				m_HS;

	int                     m_nBrightness;
	bool                    m_bFullScreenON;

	int                     m_nHowToTimer;
	int                     m_nHowToCurrent;
	int                     m_nState;
	int                     m_nChoice;
	int                     m_nHowToAlpha;
	int                     m_nImageID[5];
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

	bool LoadSlotInfo( const char* );
	bool UpdateSlotInfo( const char*, int );

	tSlotInfo GetSlotInfoOne( )     { return _tSlotOne;       }
	tSlotInfo GetSlotInfoTwo( )     { return _tSlotTwo;       }
	tSlotInfo GetSlotInfoThree( )   { return _tSlotThree;     }
 
	int GetBrightness( )  const     { return m_nBrightness;   }
	bool GetFS( )         const     { return m_bFullScreenON; }

	void ToggleBrightness( int _i ) { m_nBrightness   = _i;   }
	void toggleFS( bool _b )        { m_bFullScreenON = _b;   }

	void Enter( );
	bool Input( );
	void Update( float );
	void Render( );
	void Exit( );
};