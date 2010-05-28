#pragma once

#include "IGameState.h"
#include "CGame.h"
#include "CHighScores.h"

class CGame;
class IGameState;
class CSGD_Direct3D;
class CSGD_TextureManager;
class CSGD_DirectSound;
class CSGD_WaveManager;
class CSGD_DirectInput;


class CGameOver : public IGameState
{
private:
	int       m_nState;
	int       m_nCondition;
	int       m_nExitTimer;

	float m_fDelay;
	int       m_nImageID[2];

	bool      m_bIsAllowedToExit;

	CHighScores	m_HS;
	Score		m_CurrScore;
	int m_nLetterSelection;

	CSGD_Direct3D           *m_pD3D;
	CSGD_TextureManager     *m_pTM;
	CSGD_DirectSound        *m_pDS;
	CSGD_WaveManager        *m_pWM;
	CSGD_DirectInput        *m_pDI;

	CGameOver( )    { /* DO NOTHING */ }
	~CGameOver( )   { /* DO NOTHING */ }
	CGameOver( const CGameOver& );
	CGameOver& operator =( const CGameOver& );

public:
	static CGameOver* GetInstance( );
	
	void Enter( );
	bool Input( );
	void Update( float );
	void Render( );
	void Exit( );

	int GetState( )              { return m_nState;      }
	int GetCondition( )          { return m_nCondition;  }

	void SetState( int _i )      { m_nState      = _i;   }
	void SetCondition( int _i )  { m_nCondition  = _i;   }
};