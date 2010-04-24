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
using std::vector;

class CBase;


class CLevel
{
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

	char* m_szNextLevelFileName;

	vector<CBase*>	m_pTerrainTiles;
	vector<CBase*>	m_pEventTiles;

public:
	CLevel();
	~CLevel();

	
	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Accessors"
	//////////////////////////////////////////////////////////////////////////
	int GetWorldCollumn(){return m_nWorldCollumn;}
	int GetWorldRow(){return m_nWorldRow;}

	int GetTileWidth(){return m_nTileWidth;}
	int GetTileHeight(){return m_nTileHeight;}

	int GetTileCollumn(){return m_nTileCollumn;}
	int GetTileRow(){return m_nTileRow;}

	int GetBaseTileID(){return m_nBaseTileID;}

	int GetTileSet(){return m_nTileSetImageID;}
	int GetBackGroundImage(){return m_nBackGroundImageID;}
	int GetBGM(){return m_nBGM;}

	char* GetNextLevelFileName(){return m_szNextLevelFileName;}

private:
	

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Mutators"
	//////////////////////////////////////////////////////////////////////////
	void		SetWorldCollumn(int nWorldCollumn) { m_nWorldCollumn=nWorldCollumn;	}
	void		SetWorldRow(int nWorldRow) { m_nWorldRow = nWorldRow;	}
	void		SetTileWidth(int nTileWidth) {  m_nTileWidth=nTileWidth;	}
	void		SetTileHeight(int nTileHeight) { m_nTileHeight=nTileHeight;	}
	void		SetTileCollumn(int nTileCollumn) {  m_nTileCollumn=nTileCollumn;	}
	void		SetTileRow(int nTileRow) { m_nTileRow=nTileRow;	}
	void		SetBaseTileID(int nBaseTileID) { m_nBaseTileID=nBaseTileID;	}

	void	SetTileSetID(int nTileSetImageID)	{  m_nTileSetImageID = nTileSetImageID;	}
	void	SetBackGroundImage( int nBackGroundImageID){m_nBackGroundImageID; m_nBackGroundImageID=nBackGroundImageID;	}
	void	SetBGM(int nBGM)	{m_nBGM=nBGM;}


	void	SetNextLevelFileName(char* szNextLevelFileName)	
	{	if(m_szNextLevelFileName)delete	m_szNextLevelFileName;m_szNextLevelFileName=szNextLevelFileName;	}
	
	


public:

	/////////////////////////////////////////////////////////////////////////////////////
	// Function: "LoadNewLevel"
	//
	// Purpose:		Loads the data from a .lapidlvl binary file and with it creates the
	//				terrain and events
	/////////////////////////////////////////////////////////////////////////////////////
	void LoadNewLevel( char* filename );

	/////////////////////////////////////////////////////////////////////////////////////
	// Function: "LoadNextLevel"
	//
	// Purpose:		Loads the data from a next level and with it creates the
	//				terrain and events
	/////////////////////////////////////////////////////////////////////////////////////
	bool LoadNextLevel(  );


	/////////////////////////////////////////////////////////////////////////////////////
	// Function: "RenderBackGround"
	//
	// Purpose:		Renders the background image
	/////////////////////////////////////////////////////////////////////////////////////
	void RenderBackGround();
	
};