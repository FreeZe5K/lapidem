//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CLevel.h"
//
//  Author Name :   Pablo Leon
//
//  Purpose     :   Handles the terrain objects as well and loading levels
//					into the game
//////////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>
#include <fstream>
using std::vector;
using std::ofstream;
using std::ifstream;

#include "CTerrainBase.h"
#include "CLevelSwitch.h"

class CBase;

class CLevel
{
private:
	int m_nWorldCollumn;
	int m_nWorldRow;

	int m_nTileWidth;
	int m_nTileHeight;

	int m_nTileCollumn;
	int m_nTileRow;

	int m_nBaseTileID;

	int m_nTileSetImageID;
	int m_nBackGroundImageID;

	int m_nBGM;

	// Used For TileSwitching
	//int m_nBoundaryTileID;
	//int m_nRockTileID;
	//int m_nWaterTileID;
	//int m_nLavaTileID;
	int m_nTerrainTileIDs[MAX_TERRAIN];

	float m_nEndPosX;
	float m_nEndPosY;

	vector<CBase*> m_pLevelSwitches;

	char* m_szNextLevelFileName;
	char* m_szLevelFileName;

	vector<CBase*>	m_pTerrainTiles;
	vector<CBase*>	m_pRenderObjs;
	vector<CBase*>	m_pEventTiles;

public:
	CLevel( );
	~CLevel( );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Accessors"
	//////////////////////////////////////////////////////////////////////////
	int GetWorldCollumn( ) const          { return m_nWorldCollumn;          }
	int GetWorldRow( ) const              { return m_nWorldRow;              }

	int GetTileWidth( ) const             { return m_nTileWidth;             }
	int GetTileHeight( ) const            { return m_nTileHeight;            }

	int GetTileCollumn( ) const           { return m_nTileCollumn;           }
	int GetTileRow( ) const               { return m_nTileRow;               }

	int GetBaseTileID( ) const            { return m_nBaseTileID;            }

	int GetTileSet( ) const               { return m_nTileSetImageID;        }
	int GetBackGroundImage( ) const       { return m_nBackGroundImageID;     } 
	int GetBGM( ) const                   { return m_nBGM;                   }

	int GetTileIDFromType(int nType)      { return m_nTerrainTileIDs[nType]; }

	//	filename ----------------------   whatever#.laplvl"
	char* GetNextLevelFileName( ) const   { return m_szNextLevelFileName;    }
	char* GetLevelFileName( ) const       { return m_szLevelFileName;        }

	void ResetSwitches();

private:
	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Mutators"
	//////////////////////////////////////////////////////////////////////////
	void    SetWorldCollumn( int nWorldCollumn )            { m_nWorldCollumn         = nWorldCollumn;    }
	void    SetWorldRow( int nWorldRow )                    { m_nWorldRow             = nWorldRow;        }
	void    SetTileWidth( int nTileWidth )                  { m_nTileWidth            = nTileWidth;       }
	void    SetTileHeight( int nTileHeight )                { m_nTileHeight           = nTileHeight;      }
	void    SetTileCollumn( int nTileCollumn )              { m_nTileCollumn          = nTileCollumn;     }
	void    SetTileRow( int nTileRow )                      { m_nTileRow              = nTileRow;         }
	void    SetBaseTileID( int nBaseTileID )                { m_nBaseTileID           = nBaseTileID;      }

	void    SetTileSetID( int nTileSetImageID )	            { m_nTileSetImageID       = nTileSetImageID;  }
	void    SetBGM( int nBGM )	                            { m_nBGM = nBGM;                              }

	__inline void SetBackGroundImage( int nBackGroundImageID )
	{ 
		m_nBackGroundImageID;
		m_nBackGroundImageID = nBackGroundImageID; 
	}

public:
	void Update( float );
	void Render( );

	/////////////////////////////////////////////////////////////////////////////////////
	// Function: "LoadNewLevel"
	//
	// Purpose:		Loads the data from a .lapidlvl binary file and with it creates the
	//				terrain and events
	/////////////////////////////////////////////////////////////////////////////////////
	void LoadNewLevel( char* );

	/////////////////////////////////////////////////////////////////////////////////////
	// Function: "LoadNextLevel"
	//
	// Purpose:		Loads the data from a next level and with it creates the
	//				terrain and events
	/////////////////////////////////////////////////////////////////////////////////////
	bool LoadNextLevel( );

	/////////////////////////////////////////////////////////////////////////////////////
	// Function: "Clear"
	//
	// Purpose:		Clears all the data on the class
	/////////////////////////////////////////////////////////////////////////////////////
	void Clear( );

	/////////////////////////////////////////////////////////////////////////////////////
	// Function: "RenderBackGround"
	//
	// Purpose:		Renders the background image
	/////////////////////////////////////////////////////////////////////////////////////
	void RenderBackGround( );

	/////////////////////////////////////////////////////////////////////////////////////
	// Function: "Check Collision"
	//
	// Purpose:		Verifies if there is a collision beetween the terrain and the passed object
	//				returns the terrain object if a collision occurs
	////////////////////////////////////////////////////////////////////////////////////
	CBase* CheckCollision( CBase* );
	CBase* GetEntryPoint( );
	CBase* GetEndPoint( );

	bool IsOnScreen( CBase* );
	bool NextLevelOpen();

	CBase* GetTile(int nPosX, int nPosY);

	void CLevel::SaveLevelFromMemory(ofstream* fout, char* szFileName, 
		vector<CBase*>* pTerrainTiles, vector<CBase*>* pEventTiles, vector<CBase*>* pSwitches);
	void SaveCurrLevelState(ofstream* fout);

	void LoadLevelToMemory(ifstream* fin, char* &szFileName, vector<CBase*>* pTerrainTiles, 
		vector<CBase*>* pEventTiles, vector<CBase*>* pSwitches);
	bool LoadLevelFromSave(ifstream* fin);

	__inline void SetNextLevelFileName( char* szNextLevelFileName )	
	{	
		if( m_szNextLevelFileName )
			delete m_szNextLevelFileName; 
		m_szNextLevelFileName = szNextLevelFileName;	
	}

	/////////////////////////////////////////////////////////////////////////////////////
	// Function:    "Accessors"
	//
	// Purpose:     Get stuff.
	/////////////////////////////////////////////////////////////////////////////////////
	float GetLevelEndX( )            { return m_nEndPosX;                              }
	float GetLevelEndY( )            { return m_nEndPosY;                              }

	float GetSwitchOnePosX( )        { return m_pLevelSwitches[0]->GetPosX( );         }
	float GetSwitchOnePosY( )        { return m_pLevelSwitches[0]->GetPosY( );         }
	bool  GetSwitchOneON( )          { return ((CLevelSwitch*)m_pLevelSwitches[0])->GetSwitchState( );  }
	
	float GetSwitchTwoPosX( )        { return m_pLevelSwitches[1]->GetPosX( );         }
	float GetSwitchTwoPosY( )        { return m_pLevelSwitches[1]->GetPosY( );         }
	bool  GetSwitchTwoON( )          { return ((CLevelSwitch*)m_pLevelSwitches[1])->GetSwitchState( );  }
	
	float GetSwitchThreePosX( )      { return m_pLevelSwitches[2]->GetPosX( );         }
	float GetSwitchThreePosY( )      { return m_pLevelSwitches[2]->GetPosY( );         }
	bool  GetSwitchThreeON( )        { return ((CLevelSwitch*)m_pLevelSwitches[2])->GetSwitchState( );  }
	
	float GetSwitchFourPosX( )       { return m_pLevelSwitches[3]->GetPosX( );         }
	float GetSwitchFourPosY( )       { return m_pLevelSwitches[3]->GetPosY( );         }
	bool  GetSwitchFourON( )         { return ((CLevelSwitch*)m_pLevelSwitches[3])->GetSwitchState( );  }
};