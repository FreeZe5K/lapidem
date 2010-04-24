#pragma once

#include "CGame.h"
#include "IGameState.h"
#include "CLevel.h"

class CCamera;
class CPlayer;
class IGameState;
class Corona_ObjectManager;
class Corona_EventHandler;


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
	Corona_ObjectManager	*m_pCoM;
	Corona_EventHandler		*m_pCeH;
	CCamera					*theCamera;

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
	void Update( );
	void Render( );
	void Exit( );
};