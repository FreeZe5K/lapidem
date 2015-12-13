#pragma once

#include "IGameState.h"
#include "CPlayer.h"
#include "CGame.h"

#include <fstream>

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
		int            _nCurrentLevel;

		int            _nPositionX;
		int            _nPositionY;

		int            _nPlayerTwoPosX;
		int            _nPlayerTwoPosY;

		int            _nPlayerOneScore;
		int            _nPlayerTwoScore;

		int            _nPlayerOneHealth;
		int            _nPlayerTwoHealth;

		struct tMana
		{
			int        _nFire;
			int        _nEarth;
			int        _nIce;
			int        _nAir;
		} _manaP1, _manaP2;

		struct tSpell
		{
			int        _nFireXP;
			int        _nFireLevel;
			int        _nEarthXP;
			int        _nEarthLevel;			
			int        _nIceXP;
			int        _nIceLevel;
			int        _nAirXP;
			int        _nAirLevel;
		} _spellP1, _spellP2;
	} _tSlotOne, _tSlotTwo, _tSlotThree;

	struct tLevelInfo
	{
		char *szFileName;
		vector<CBase*> pTerrainTiles;
		vector<CBase*> pEventTiles;
		vector<CBase*> pSwitches;
	} m_tLevelInfo[3];
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
	void SaveCurrLevelState(ofstream* fout);

	void Enter( );
	bool Input( );
	void Update( float );
	void Render( );
	void Exit( );
};