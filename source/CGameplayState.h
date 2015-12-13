#pragma once

#include "CGame.h"
#include "IGameState.h"
#include "CParticleManager.h"
#include "CLoading.h"
#include "CEmitter.h"
#include "CLevel.h"

class CCamera;
class CPlayer;
class CEnemy;
class IGameState;
class Corona_ObjectManager;
class Corona_EventHandler;

class CGameplayState : public IGameState, public CLDevice 
{
private:
	int                     m_nImageID[6];
	int                     m_nSlotLoadedFrom;
	int						m_nPlayerOneScore;
	int						m_nPlayerTwoScore;
	int						m_nSinglePlayerScore;
	int                     m_nCurrentLevel;
	int						m_nDifficulty;

	float					m_fP2RespawnTimer;
	float                   m_fMMCurrentRotation[5];

	bool                    m_bIsPaused;
	bool                    m_bMapActive;
	bool					m_bTwoPlayers;
	bool                    m_bLoadedFromFile;
	bool                    m_bPlayerReachedEnd;
	bool					m_bBossSpawned;

	CSGD_Direct3D           *m_pD3D;
	CSGD_TextureManager     *m_pTM;
	CSGD_DirectSound        *m_pDS;
	CSGD_WaveManager        *m_pWM;
	CSGD_DirectInput        *m_pDI;
	Corona_ObjectManager	*m_pCoM;
	Corona_EventHandler		*m_pCeH;
	CCamera					*theCamera;

	tVector2D               m_tDir;

	CLevel					theLevel;

	CPlayer					*m_pPlayerOne;
	CPlayer					*m_pPlayerTwo;
	CEnemy					*thaBoss;

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
	int     GetCurrentLevel( )               { return m_nCurrentLevel;        }
	int		GetDifficulty() 				 { return m_nDifficulty;		  }

	bool    GetLoadedFromFile( )             { return m_bLoadedFromFile;      }
	bool    GetTwoPlayerMode( )              { return m_bTwoPlayers;          }

	void    SetSinglePlayerScore( int _i )   { m_nSinglePlayerScore   =  _i;  }
	void    SetPlayerOneScore( int _i )      { m_nPlayerOneScore      =  _i;  }
	void    SetPlayerTwoScore( int _i )      { m_nPlayerTwoScore      =  _i;  }
	void    SetCurrentLevel( int _i )        { m_nCurrentLevel        =  _i;  }
	void    SetSlotLoaded( int _i )          { m_nSlotLoadedFrom      =  _i; m_bLoadedFromFile = true; }
	void	SetDifficulty( int _i )			 { m_nDifficulty          =  _i;  }

	void    SetLoadedFromFile( bool _b )     { m_bLoadedFromFile      =  _b;  }
	void    bTwoPlayerMode( bool _b )        { m_bTwoPlayers          =  _b;  }
	void    SetPlayerReachedEnd( bool _b )   { m_bPlayerReachedEnd    =  _b;  }

	void	AddScore( int _n ) { m_nSinglePlayerScore+=_n; }
#ifdef _DEBUG
	void spawnenergy( );
#endif
};