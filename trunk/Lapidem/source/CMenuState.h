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
class CSGD_Direct3D;
class CSGD_TextureManager;
class CSGD_DirectSound;
class CSGD_WaveManager;
class CSGD_DirectInput;

class CMenuState : public IGameState
{
private:
	struct tSlots
	{
		int            nPlayerCount;
		int            nPositionX;
		int            nPositionY;
		int            nPlayerTwoPosX;
		int            nPlayerTwoPosY;
		int            nSinglePlayerScore;
		int            nPlayerOneScore;
		int            nPlayerTwoScore;
	} tSlotOne, tSlotTwo, tSlotThree;

	int                     m_nState;
	int                     m_nChoice;
	int                     m_nPlayerCount;
	int                     m_nImageID;
	int                     m_nAttractTimer;

	CSGD_Direct3D           *m_pD3D;
	CSGD_TextureManager     *m_pTM;
	CSGD_DirectSound        *m_pDS;
	CSGD_WaveManager        *m_pWM;
	CSGD_DirectInput        *m_pDI;

	CMenuState( )  { /* DO NOTHING */ }
	~CMenuState( ) { /* DO NOTHING */ }
	CMenuState( const CMenuState& );
	CMenuState& operator=( const CMenuState& );


	float m_fInputTimer;

public:
	static CMenuState* GetInstance( );

	int GetPlayerCount( )          { return m_nPlayerCount; }

	tSlots GetSlotOne( )           { return tSlotOne;       }
	tSlots GetSlotTwo( )           { return tSlotTwo;       }
	tSlots GetSlotThree( )         { return tSlotThree;     }

	void ResetAttractTimer( )      { m_nAttractTimer  = 0;   }
	void SetPlayerCount( int _i )  { m_nPlayerCount   = _i;  }

	bool Load( int );

	void Enter( );
	bool Input( );
	void Update( float );
	void Render( );
	void Exit( );
};