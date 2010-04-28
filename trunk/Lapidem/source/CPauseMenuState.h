#pragma once

#include "IGameState.h"
#include "CPlayer.h"
#include "CGame.h"

class CGame;
class CPlayer;
class IGameState;
class CSGD_Direct3D;
class CSGD_TextureManager;
class CSGD_DirectSound;
class CSGD_WaveManager;
class CSGD_DirectInput;

class CPauseMenuState : public IGameState
{
private:
	// - - - - - - - - - - -
	// For saving
	// - - - - - - - - - - -
	struct tSlotInfo
	{
		int            _nPlayerCount;
		int            _nPositionX;
		int            _nPositionY;
		int            _nSinglePlayerScore;
		int            _nPlayerOneScore;
		int            _nPlayerTwoScore;
	} _tSlotOne, _tSlotTwo, _tSlotThree;
	// - - - - - - - - - - -

	int                     m_nState;
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

	bool SaveGame( int ); 

	void Enter( );
	bool Input( );
	void Update( float );
	void Render( );
	void Exit( );
};