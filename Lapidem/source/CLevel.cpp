//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CLevel.cpp"
//
//  Author Name :   Pablo Leon
//
//  Purpose     :   Handles the terrain objects as well and loading levels
//					into the game
//////////////////////////////////////////////////////////////////////////
#include "CLevel.h"
#include "Wrappers/CSGD_TextureManager.h"
#include "Corona_ObjectManager.h"// CSGD_TextureManager.h"

#include "CTerrainBase.h"
#include <fstream>
#include <string>
using namespace std;

CLevel::CLevel()
{
	SetWorldCollumn(0);
	SetWorldRow(0);
	SetTileCollumn(0);
	SetTileRow(0);
	SetTileWidth(0);
	SetTileHeight(0);
	SetBaseTileID(0);

	SetTileSetID(-1);
	SetBackGroundImage(-1);
	SetBGM(-1);
	SetNextLevelFileName(0);

}

CLevel::~CLevel()
{
	
	if(m_szNextLevelFileName)
		delete	m_szNextLevelFileName;

	if( m_pEventTiles.size() > 0 )
	{
		for( UINT i = 0; i < m_pEventTiles.size(); ++i )
		{
			m_pEventTiles[i]->SetActive(false);
			m_pEventTiles[i]->Release();
		}
		m_pEventTiles.clear();

	}

	if( m_pTerrainTiles.size() > 0 )
	{
		for( UINT i = 0; i < m_pTerrainTiles.size(); ++i )
		{
			m_pTerrainTiles[i]->SetActive(false);
			m_pTerrainTiles[i]->Release();
		}
		m_pTerrainTiles.clear();

	}

}

void CLevel::LoadNewLevel( char* filename )
{
	ifstream in( filename, ios::in|ios::binary);

	if( in.is_open() )
	{
		   /*
                 *      Version test
                 *  World Dimension
                 *  Tile width
                 *  tile height
				 *		collumntile
				 *		rowtile
                 *      tileset filename
                 *      background filename
				 *		bgm filename
                 *      nextlevel filename
                 *      base tileID
                 *      tiles
                 *      events
                 */

		char size;
		int data;

		in.read( (char*)&size, sizeof(char));

		char* version = new char[size+1];
		in.read( version, size );
		version[size] =0;


		//in.read( (char*)&size, sizeof(char));
		in.read( (char*)&data, sizeof(int));
		SetWorldCollumn(data);
		//in.read( (char*)&size, sizeof(char));
		in.read( (char*)&data, sizeof(int));
		SetWorldRow(data);
		//in.read( (char*)&size, sizeof(char));
		in.read( (char*)&data, sizeof(int));
		SetTileWidth(data);
		//in.read( (char*)&size, sizeof(char));
		in.read( (char*)&data, sizeof(int));
		SetTileHeight(data);
		//in.read( (char*)&size, sizeof(char));
		in.read( (char*)&data, sizeof(int));
		SetTileCollumn(data);
		//in.read( (char*)&size, sizeof(char));
		in.read( (char*)&data, sizeof(int));
		SetTileRow(data);
	
		if( GetTileSet() != -1 )
			CSGD_TextureManager::GetInstance()->UnloadTexture( GetTileSet());
		in.read( (char*)&size, sizeof(char));
		char szTileSet[64];// = new char[size];
		in.read( szTileSet, sizeof(char)*size );
		szTileSet[size] = 0;
		string szTemp;
		if( size != 1 )
		{
		
		szTemp = "resource\\graphics\\";
		szTemp += szTileSet;
		SetTileSetID(CSGD_TextureManager::GetInstance()->LoadTexture(szTemp.c_str()));
		}else
			SetTileSetID(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/ScA_WorldTileset.PNG"));


		if( GetBackGroundImage() != -1 )
			CSGD_TextureManager::GetInstance()->UnloadTexture( GetBackGroundImage() );
		in.read( (char*)&size, sizeof(char));
		char szBGI[64];
		in.read( szBGI, sizeof(char)*size );
		szBGI[size] = 0;
		if( size != 1)
		{
		szTemp = "resource\\graphics\\";
		szTemp += szBGI;
		SetTileSetID(CSGD_TextureManager::GetInstance()->LoadTexture(szTemp.c_str()));
		}

		//////////////////
		// TODO: BGM
		/////////////////

		in.read( (char*)&size, sizeof(char));
		char szBGM[64];
		in.read( szBGM, sizeof(char)*size );

		

		if( GetNextLevelFileName() != NULL )
			delete m_szNextLevelFileName;
		in.read( (char*)&size, sizeof(char));
		char* szNextLvl = new char[size+1];
		in.read( szNextLvl, sizeof(char)*(size) );
		szNextLvl[size] = 0;
		SetNextLevelFileName(szNextLvl);

		if( m_pTerrainTiles.size() > 0 )
		{
			//TODO STUFF TO DO HERE
			for(unsigned int i = 0; i < m_pTerrainTiles.size(); ++i )
			{
				m_pTerrainTiles[i]->SetActive(false);
				m_pTerrainTiles[i]->Release();
			}


		}
		
		//in.read( (char*)&size, sizeof(char));
		in.read( (char*)&data, sizeof(int));
		SetBaseTileID(data);

		int Type;
		int ID;
		for( int i = 0; i < GetWorldCollumn()*GetWorldRow(); ++i )
		{
			//in.read( (char*)&size, sizeof(char));
			in.read( (char*)&ID , sizeof(int));
			//in.read( (char*)&size, sizeof(char));
			in.read( (char*)&Type , sizeof(int));

			switch ( Type )
			{
			default:
				CTerrainBase* newTerrain = new CTerrainBase();

				newTerrain->SetType( OBJ_TERRA );
				newTerrain->SetTypeTerrain( Type );
				newTerrain->SetBaseTileID( GetBaseTileID() );
				newTerrain->SetDamage(0);
				newTerrain->SetHealth(100);
				newTerrain->SetHeight( GetTileHeight());
				newTerrain->SetWidth( GetTileWidth());
				newTerrain->SetImage( GetTileSet());
				newTerrain->SetPosX( (float)(i%GetWorldCollumn())*GetTileWidth());
				newTerrain->SetPosY( (float)(i/GetWorldCollumn())*GetTileHeight());
				newTerrain->SetTileCollumns( GetTileCollumn());
				newTerrain->SetTileID(ID);
				newTerrain->SetTileRows( GetTileRow());
				newTerrain->SetVelX(0);
				newTerrain->SetVelY(0);
				
				m_pTerrainTiles.push_back(newTerrain);
				Corona_ObjectManager::GetInstance()->AddObject(newTerrain);


				break;
			};
		}

		if( m_pEventTiles.size() >  0 )
		{
			for(unsigned int i = 0; i < m_pEventTiles.size(); ++i )
			{
				m_pEventTiles[i]->SetActive(false);
				m_pEventTiles[i]->Release();
			}
		}
		
		for( int i = 0; i < GetWorldCollumn()*GetWorldRow(); ++i )
		{
			
			//in.read( (char*)&size, sizeof(char));
			in.read( (char*)&Type , sizeof(int));
			//in.read( (char*)&size, sizeof(char));
			in.read( (char*)&data , sizeof(int));

			switch(Type)
			{
			default:

				
				break;

			};
			


		}


		delete version;
		in.close();
	}
}
