#pragma once

#include "CGame.h"
#include "IGameState.h"
#include "CParticleManager.h"
#include "CLevel.h"

class CCamera;
class CPlayer;
class IGameState;
class Corona_ObjectManager;
class Corona_EventHandler;
class CEmitterFactory;

class CGameplayState : public IGameState, public CLDevice 
{
private:
	int                     m_nImageID[3];
	int                     m_nSlotLoadedFrom;
	int                     m_nSinglePlayerScore;
	int                     m_nPlayerOneScore;
	int                     m_nPlayerTwoScore;

	float					m_fP2RespawnTimer;

	bool                    m_bIsPaused;
	bool					m_bTwoPlayers;
	bool                    m_bLoadedFromFile;

	CSGD_Direct3D           *m_pD3D;
	CSGD_TextureManager     *m_pTM;
	CSGD_DirectSound        *m_pDS;
	CSGD_WaveManager        *m_pWM;
	CSGD_DirectInput        *m_pDI;
	Corona_ObjectManager	*m_pCoM;
	Corona_EventHandler		*m_pCeH;
	CCamera					*theCamera;

	CEmitterFactory         *m_pEF;
	CParticleManager        *m_pPM;

	CLevel					theLevel;

	CPlayer					*m_pPlayerOne;
	CPlayer					*m_pPlayerTwo;

	CGameplayState( )  { /* DO NOTHING */ }
	~CGameplayState( ) { /* DO NOTHING */ }
	CGameplayState( const CGameplayState& );
	CGameplayState& operator=( const CGameplayState& );

public:
	static CGameplayState* GetInstance( );

	void Enter( );
	bool Input( );
	void Update( float );
	void Render( );
	void Exit( );
	void HandleEvent( CEvent* pEvent );

	CLevel*    GetLevel()                    { return &theLevel;              }
	CPlayer*   GetPlayerOne( )               { return m_pPlayerOne;           }
	CPlayer*   GetPlayerTwo( )               { return m_pPlayerTwo;           }

	int     GetSinglePlayerScore( )          { return m_nSinglePlayerScore;   }
	int     GetPlayerOneScore( )             { return m_nPlayerOneScore;      }
	int     GetPlayerTwoScore( )             { return m_nPlayerTwoScore;      }
	int     GetSlotLoaded( )                 { return m_nSlotLoadedFrom;      }

	bool    GetLoadedFromFile( )             { return m_bLoadedFromFile;      }
	bool    GetTwoPlayerMode( )              { return m_bTwoPlayers;          }

	void    SetSinglePlayerScore( int _i )   { m_nSinglePlayerScore   =  _i;  }
	void    SetPlayerOneScore( int _i )      { m_nPlayerOneScore      =  _i;  }
	void    SetPlayerTwoScore( int _i )      { m_nPlayerTwoScore      =  _i;  }
	void    SetSlotLoaded( int _i )          { m_nSlotLoadedFrom      =  _i;  }

	void    SetLoadedFromFile( bool _b )     { m_bLoadedFromFile      =  _b;  }
	void    bTwoPlayerMode( bool _b )        { m_bTwoPlayers          =  _b;  }
};