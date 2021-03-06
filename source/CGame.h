//////////////////////////////////////////////////////////////
//  File Name   :   "CGame.h"
//
//  Author Name :   Mike Hatter
//
//  Purpose     :   To encapsulate all game related code.
//////////////////////////////////////////////////////////////
#pragma once                                      // Necessary Include protection.

//////////////////////////////
// Include Wrappers
#include "Wrappers/CSGD_Direct3D.h"           // Wrapper for Direct3D.
#include "Wrappers/CSGD_TextureManager.h"     // Wrapper for Drawing.
#include "Wrappers/CSGD_DirectSound.h"        // Wrapper for using DirectSound.
#include "Wrappers/CSGD_WaveManager.h"        // Wrapper for managing .wav files.
#include "Wrappers/CSGD_DirectInput.h"        // Wrapper for DirectInput.

//////////////////////////////
// Interfaces
#include "IGameState.h"                           // Interface for game-states.

//////////////////////////////
// Menus
#include "CMenuState.h"
#include "CAttractState.h"
#include "CPauseMenuState.h"
#include "CGameplayState.h"
#include "CAuxiliaryState.h"

//////////////////////////////
// Base classes
#include "CBase.h"

//////////////////////////////
// Additional Includes
#include "CBitmapFont.h"
#include <vector>                                 // We're going to store states in a vector.
#include <stdlib.h>                               // We need this to run batch files.
#include <fstream>
#include <string>
using std::string;

//////////////////////////////
// Macros
#define RAND_FLOAT(min,max) (((rand()/(float)RAND_MAX)*((max)-(min)))+(min))

//////////////////////////////////////////////////////////////////////////
// CGame class
//////////////////////////////////////////////////////////////////////////
class CGame
{
private:
	//////////////////////////////
	// Singleton pointers
	CSGD_Direct3D           *m_pD3D;
	CSGD_TextureManager     *m_pTM;
	CSGD_DirectSound        *m_pDS;
	CSGD_WaveManager        *m_pWM;
	CSGD_DirectInput        *m_pDI;

	//////////////////////////////
	// Vector of game states
	vector<IGameState*>     m_vGS;

	//////////////////////////////
	// Vector for cheat codes
	string                  m_szCHEATSTRING;
	
	//////////////////////////////
	// Asset ID's
	// Images - 
	//     Main Menu BG
	//     Pause Menu BG
	//     How To Play BG
	//      
	// Sounds -
	//     0. Main Menu Music
	//     1. Menu Click
	//     2. Level One Music
	//     3. Victory Music
	//     4. Player Died Music
	//     5. Player Hit Effect
	//     6. Spell Fired Effect
	//     7. Tile Destroyed Effect
	//     8. Level Two Music
	//     9. Level Three Music
	int                     m_nBrightImage;
	int                     m_nBrightness;
	int                     m_nImageID[3];
	int                     m_nSoundID[10];

	int                     m_nSoundEffectVolume;
	int                     m_nMusicVolume;

	//////////////////////////////
	// Screen dimensions
	int                     m_nScreenWidth;
	int                     m_nScreenHeight;

	//////////////////////////////
	// Is the game in fullscreen?
	bool                    m_bIsNotFullscreen;

	bool                    m_bFullScreenChange;
	int                     m_nFSValue;

	//////////////////////////////
	// Is the game in debug mode?
	bool                    m_bIsInDebug;

	//////////////////////////////
	// Is the game paused?
	bool                    m_bIsPaused;

	//////////////////////////////
	// Bitmap Font
	CBitmapFont*            m_bmFont;

	//////////////////////////////
	// Variables to count time
	// and frames per second
	DWORD                   m_dwTimeStamp;
	DWORD                   m_dwPreviousTimeStamp;
	DWORD                   m_dwFPSTimeStamp;

	float                   m_fElapsedTime;
	float                   m_fGameTime;

	int                     m_nFrameCount;
	int                     m_nFPS;

	int                     m_nTimeLeft;

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Constructor"
	//
	//  Purpose   :   Disabled in the singleton design pattern. 
	//////////////////////////////////////////////////////////////////////////
	CGame( );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Copy Constructor"
	//
	//  Purpose   :   Disabled in the singleton design pattern. 
	//////////////////////////////////////////////////////////////////////////
	CGame( const CGame& );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Assignment Operator"
	//
	//  Purpose   :   Disabled in the singleton design pattern. 
	//////////////////////////////////////////////////////////////////////////
	CGame& operator=( const CGame& );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Destructor"
	//
	//  Purpose   :   Disabled in the singleton design pattern. 
	//////////////////////////////////////////////////////////////////////////
	~CGame( ) { /* DO NOTHING */ }

public:
	//////////////////////////////
	// Singleton accessor
	static CGame* GetInstance( );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Initialize"
	//
	//  Purpose   :   Setup the game. 
	//////////////////////////////////////////////////////////////////////////
	void Initialize( HWND, HINSTANCE, int, int, bool );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Main"
	//
	//  Purpose   :   The main game loop, calls render and update. 
	//                Also does all calculations for time.
	//////////////////////////////////////////////////////////////////////////
	bool Main( );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Shutdown"
	//
	//  Purpose   :   Shutdown the game and clean up all objects. 
	//////////////////////////////////////////////////////////////////////////
	void Shutdown( );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Input"
	//
	//  Purpose   :   Handles the keyboard. 
	//////////////////////////////////////////////////////////////////////////
	bool Input( );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Update"
	//
	//  Purpose   :   Updates the game objects ( and time ). 
	//////////////////////////////////////////////////////////////////////////
	void Update( float );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Render"
	//
	//  Purpose   :   Draws objects and prints text to the screen. 
	//////////////////////////////////////////////////////////////////////////
	void Render( );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "ChangeState"
	//
	//  Purpose   :   Changes the current state to the one passed in. 
	//////////////////////////////////////////////////////////////////////////
	void ChangeState( IGameState* );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "PushState"
	//
	//  Purpose   :   Push a state on top of the current state. 
	//////////////////////////////////////////////////////////////////////////
	void PushState( IGameState* );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "PopState"
	//
	//  Purpose   :   Take the top state off the vector. 
	//////////////////////////////////////////////////////////////////////////
	void PopState( );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "CheckForCheats"
	//
	//  Purpose   :   Check to see if a cheat was entered and respond accordingly.
	//////////////////////////////////////////////////////////////////////////
	void CheckForCheats( );

	//////////////////////////////////////////////////////////////////////////
	//  Function  : Accessors
	//
	//  Purpose   : Returns the specified type.
	//////////////////////////////////////////////////////////////////////////
	CBitmapFont* GetFont( )       const      { return m_bmFont;                }
	bool GetDebugMode( )          const      { return m_bIsInDebug;            }
	bool GetPaused( )             const      { return m_bIsPaused;             }

	string GetCheatString( )      const      { return m_szCHEATSTRING;         }

	int GetSoundFXVolume( )       const      { return m_nSoundEffectVolume;    }
	int GetMusicVolume( )         const      { return m_nMusicVolume;          }
	int GetFPS( )                 const      { return m_nFPS;                  }

	float GetElapsedTime( )       const      { return m_fElapsedTime;          }

	int GetScreenWidth( )		  const		 { return m_nScreenWidth;		   }
	int GetScreenHeight( )		  const      { return m_nScreenHeight;		   }

	int GetMainMenuMusic( )       const      { return m_nSoundID[0];           }
	int GetMenuTick( )            const      { return m_nSoundID[1];           }
	int GetLevelOneMusic( )       const      { return m_nSoundID[2];           }
	int GetVictoryMusic( )        const      { return m_nSoundID[3];           }
	int GetLostMusic( )           const      { return m_nSoundID[4];           }
	int GetPlayerHitSound( )      const      { return m_nSoundID[5];           }
	int GetShotFiredSound( )      const      { return m_nSoundID[6];           }
	int GetTileDestroyedSound( )  const      { return m_nSoundID[7];           }
	int GetLevelTwoMusic( )       const      { return m_nSoundID[8];           }
	int GetLevelThreeMusic( )     const      { return m_nSoundID[9];           }

	int GetTimeLeft( )            const      { return m_nTimeLeft;             }
	int GetBrightness( )          const      { return m_nBrightness;           }

	DWORD GetFPSTimeStamp( )      const      { return m_dwFPSTimeStamp;        }

	int GetFSValue( )             const      { return m_nFSValue;              }

	__inline void TriggerFSTrue( )  
	{ m_bFullScreenChange = true; m_nFSValue = 1; }

	__inline void TriggerFSFalse( )  
	{ m_bFullScreenChange = true; m_nFSValue = 2; }

	//////////////////////////////////////////////////////////////////////////
	//  Function  : Modifiers
	//
	//  Purpose   : Modifies the specified type.
	//////////////////////////////////////////////////////////////////////////
	void SetDebugMode( bool _b )             { m_bIsInDebug           = _b;    }
	void SetPaused( bool _b )                { m_bIsPaused            = _b;    }
	void SetCheatString( string _s )         { m_szCHEATSTRING        = _s;    }
	void SetMusicVolume( int _i )            { m_nMusicVolume         = _i;    }
	void SetSoundFXVolume( int _i )          { m_nSoundEffectVolume   = _i;    }
	void SetTimeLeft( int _i )               { m_nTimeLeft            = _i;    }
	void SetBrightness( int _i )             { m_nBrightness          = _i;    }
	void SetFSValue( int _i )                { m_nFSValue             = _i;    }
};