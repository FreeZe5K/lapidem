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
#include "CEnemySpawner.h"
#include "CCamera.h"
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
	m_szLevelFileName = 0;
}

CLevel::~CLevel()
{
	if(m_szNextLevelFileName)
	{
		delete	m_szNextLevelFileName;
		m_szNextLevelFileName = 0;
	}

	if(m_szLevelFileName)
	{
		delete	m_szLevelFileName;
		m_szLevelFileName = 0;
	}

	Clear();
}

void CLevel::Clear( )
{
	if( m_szNextLevelFileName )
	{
		delete m_szNextLevelFileName;
		m_szNextLevelFileName = 0;
	}

	if( m_pEventTiles.size( ) > 0 )
	{
		for( UINT i = 0; i < m_pEventTiles.size( ); ++i )
		{
			m_pEventTiles[i]->SetActive( false );
			m_pEventTiles[i]->Release( );
		}

		m_pEventTiles.clear( );
	}

	if( m_pTerrainTiles.size( ) > 0 )
	{
		for( UINT i = 0; i < m_pTerrainTiles.size( ) ; ++i )
		{
			m_pTerrainTiles[i]->SetActive( false );
			m_pTerrainTiles[i]->Release( );
		}

		m_pTerrainTiles.clear( );
	}

	if( GetTileSet( ) != -1 )
		CSGD_TextureManager::GetInstance( )->UnloadTexture( GetTileSet( ) );
	SetTileSetID( -1 );

	if( GetBackGroundImage( ) != -1 )
		CSGD_TextureManager::GetInstance()->UnloadTexture( GetBackGroundImage( ) );
	SetBackGroundImage( -1 );
}

void CLevel::LoadNewLevel( char* filename )
{
	ifstream in( filename, ios::in|ios::binary);

	if( in.is_open( ) )
	{
		m_szLevelFileName = new char[strlen( filename ) + 1];
		strcpy_s( m_szLevelFileName, strlen( filename ) + 1, filename );
		m_szLevelFileName[strlen( filename )] = 0;
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

		char version[64];// = new char[size+1];
		in.read( version, size );
		version[size] = 0;

		//in.read( (char*)&size, sizeof(char));
		in.read( ( char* )&data, sizeof( int ) );
		SetWorldCollumn(data);
		//in.read( (char*)&size, sizeof(char));
		in.read( ( char* )&data, sizeof( int ) );
		SetWorldRow(data);
		//in.read( (char*)&size, sizeof(char));
		in.read( ( char* )&data, sizeof( int ) );
		SetTileWidth(data);
		//in.read( (char*)&size, sizeof(char));
		in.read( ( char* )&data, sizeof( int ) );
		SetTileHeight(data);
		//in.read( (char*)&size, sizeof(char));
		in.read( ( char* )&data, sizeof( int ) );
		SetTileCollumn(data);
		//in.read( (char*)&size, sizeof(char));
		in.read( ( char* )&data, sizeof( int ) );
		SetTileRow( data );

		if( GetTileSet() != -1 )
			CSGD_TextureManager::GetInstance( )->UnloadTexture( GetTileSet( ) );

		SetTileSetID( -1 );
		in.read( ( char* )&size, sizeof( char ) );
		char szTileSet[64];// = new char[size];
		in.read( szTileSet, sizeof( char )*size );
		szTileSet[size] = 0;
		string szTemp;

		if( size != 1 )
		{
			szTemp = "resource\\graphics\\";
			szTemp += szTileSet;
			SetTileSetID( CSGD_TextureManager::GetInstance( )->LoadTexture(szTemp.c_str( ) ) );
		} else SetTileSetID( CSGD_TextureManager::GetInstance( )->LoadTexture( "resource/graphics/ScA_WorldTileset.PNG" ) );

		if( GetBackGroundImage( ) != -1 )
			CSGD_TextureManager::GetInstance( )->UnloadTexture( GetBackGroundImage( ) );
		SetBackGroundImage( -1 );

		in.read( ( char* )&size, sizeof( char ) );
		char szBGI[64];
		in.read( szBGI, sizeof( char )*size );
		szBGI[size] = 0;

		if( size != 1)
		{
			szTemp = "resource\\graphics\\";
			szTemp = szTemp + szBGI;
			SetBackGroundImage( CSGD_TextureManager::GetInstance( )->LoadTexture( szTemp.c_str( ) ) );
		}

		//////////////////
		// TODO: BGM
		/////////////////

		in.read( ( char* )&size, sizeof( char ) );
		char szBGM[64];
		in.read( szBGM, sizeof( char )*size );

		/*if( GetNextLevelFileName() != NULL )
		delete m_szNextLevelFileName;*/
		in.read( ( char* )&size, sizeof( char ) );
		char* szNextLvl = NULL;

		szNextLvl = new char[size+1];
		in.read( szNextLvl, sizeof( char ) *( size ) );
		szNextLvl[size] = 0;
		SetNextLevelFileName(szNextLvl);

		if( m_pTerrainTiles.size() > 0 )
		{
			//TODO STUFF TO DO HERE
			for(unsigned int i = 0; i < m_pTerrainTiles.size(); ++i )
			{
				m_pTerrainTiles[i]->SetActive(false);
				for( int j = 0; j < m_pTerrainTiles[i]->GetRefCount(); ++j )
					m_pTerrainTiles[i]->Release();
			} m_pTerrainTiles.clear();
		}

		//in.read( (char*)&size, sizeof(char));
		in.read( (char*)&data, sizeof(int));
		SetBaseTileID(data);

		int Type, ID;

		for( int i = 0; i < GetWorldCollumn( ) * GetWorldRow( ); ++i )
		{
			//in.read( (char*)&size, sizeof(char));
			in.read( ( char* )&ID, sizeof( int ) );
			//in.read( (char*)&size, sizeof(char));
			in.read( ( char* )&Type, sizeof( int ) );

			switch ( Type )
			{
			case 'a': break;

			default:
				CTerrainBase* newTerrain = new CTerrainBase( );

				newTerrain->SetType( OBJ_TERRA );
				newTerrain->SetTypeTerrain( Type );
				newTerrain->SetBaseTileID( GetBaseTileID( ) );
				newTerrain->SetDamage( 0 );
				newTerrain->SetHealth( 150 );
				newTerrain->SetHeight( GetTileHeight( ) );
				newTerrain->SetWidth( GetTileWidth( ) );
				newTerrain->SetImage( GetTileSet( ) );
				newTerrain->SetPosX( float( i % GetWorldCollumn( ) ) * GetTileWidth( ) );
				newTerrain->SetPosY( float( i / GetWorldCollumn( ) ) * GetTileHeight( ) );
				newTerrain->SetTileCollumns( GetTileCollumn( ) );
				newTerrain->SetTileID( ID );
				newTerrain->SetTileRows( GetTileRow( ) );
				newTerrain->SetVelX( 0 );
				newTerrain->SetVelY( 0 );

				m_pTerrainTiles.push_back( newTerrain );
				break;
			};
		}

		if( m_pEventTiles.size( ) >  0 )
		{
			for( UINT i = 0; i < m_pEventTiles.size( ); ++i )
			{
				m_pEventTiles[i]->SetActive( false );
				for( int j = 0; j < m_pEventTiles[i]->GetRefCount( ); ++j )
					m_pEventTiles[i]->Release( );
			} m_pEventTiles.clear( );
		}

		for( int i = 0; i < GetWorldCollumn( ) * GetWorldRow( ); ++i )
		{
			//in.read( (char*)&size, sizeof(char));
			in.read( ( char* )&Type, sizeof( int ) );
			//in.read( (char*)&size, sizeof(char));
			in.read( ( char* )&data, sizeof( int ) );

			switch(Type)
			{
			case NONE: break;

			case END_POINT:
				{
					CTerrainBase* endPt = new CTerrainBase();

					endPt->SetType( OBJ_EVENT );
					endPt->SetTypeTerrain( Type );
					endPt->SetBaseTileID( GetBaseTileID( ) );
					endPt->SetDamage( 0 );
					endPt->SetHealth( 100 );
					endPt->SetHeight( GetTileHeight( ) );
					endPt->SetWidth( GetTileWidth( ) );
					endPt->SetImage( GetTileSet( ) );
					endPt->SetPosX( float( i % GetWorldCollumn( ) ) * GetTileWidth( ) );
					endPt->SetPosY( float( i / GetWorldCollumn( ) ) * GetTileHeight( ) );
					endPt->SetTileCollumns( GetTileCollumn( ) );
					endPt->SetTileID( GetBaseTileID( ) );
					endPt->SetTileRows( GetTileRow( ) );
					endPt->SetVelX( 0 );
					endPt->SetVelY( 0 );

					//m_pEventTiles.push_back(endPt);
					Corona_ObjectManager::GetInstance( )->AddObject( endPt );
					endPt->Release( );
				} break;

			case ENEMY_SPW:
				{
					CEnemySpawner* spwn = new CEnemySpawner( );

					spwn->SetType( OBJ_EVENT );
					spwn->SetTypeTerrain( ENEMY_SPW );
					spwn->SetBaseTileID( GetBaseTileID( ) );
					spwn->SetDamage( 0 );
					spwn->SetHealth( 1 );
					spwn->SetHeight( GetTileHeight( ) );
					spwn->SetWidth( GetTileWidth( ) );
					spwn->SetImage( GetTileSet( ) );
					spwn->SetPosX( float( i % GetWorldCollumn( ) ) * GetTileWidth( ) );
					spwn->SetPosY( float( i / GetWorldCollumn( ) ) *  GetTileHeight( ) );
					spwn->SetTileCollumns( GetTileCollumn( ) );
					spwn->SetTileID( GetBaseTileID( ) );
					spwn->SetTileRows( GetTileRow( ) );
					spwn->SetVelX( 0 );
					spwn->SetVelY( 0 );

					//m_pEventTiles.push_back(spwn);
					Corona_ObjectManager::GetInstance( )->AddObject( spwn );
					spwn->Release( );

				} break;

			case WATER_THR:
				{
					CTerrainBase* newEvent( new CTerrainBase( ) );

					newEvent->SetType( OBJ_EVENT );
					newEvent->SetTypeTerrain( Type );
					newEvent->SetBaseTileID( GetBaseTileID( ) );
					newEvent->SetDamage( 0 );
					newEvent->SetHealth( 100 );
					newEvent->SetHeight( GetTileHeight( ) );
					newEvent->SetWidth( GetTileWidth( ) );
					newEvent->SetImage( GetTileSet( ) );
					newEvent->SetPosX( float( i % GetWorldCollumn( ) ) * GetTileWidth( ) );
					newEvent->SetPosY( float( i / GetWorldCollumn( ) ) * GetTileHeight( ) );
					newEvent->SetTileCollumns( GetTileCollumn());
					newEvent->SetTileID( GetBaseTileID( ) );
					newEvent->SetTileRows( GetTileRow( ) );
					newEvent->SetVelX( 0 );
					newEvent->SetVelY( 0 );

					m_pEventTiles.push_back( newEvent );
				} break;

			default:
				{
					CTerrainBase* newEvent( new CTerrainBase( ) );

					newEvent->SetType( OBJ_EVENT );
					newEvent->SetTypeTerrain( Type );
					newEvent->SetBaseTileID( GetBaseTileID( ) );
					newEvent->SetDamage( 0 );
					newEvent->SetHealth( 100 );
					newEvent->SetHeight( GetTileHeight( ) );
					newEvent->SetWidth( GetTileWidth( ) );
					newEvent->SetImage( GetTileSet( ) );
					newEvent->SetPosX( float( i % GetWorldCollumn( ) ) * GetTileWidth( ) );
					newEvent->SetPosY( float( i / GetWorldCollumn( ) ) * GetTileHeight( ) );
					newEvent->SetTileCollumns( GetTileCollumn( ) );
					newEvent->SetTileID( GetBaseTileID( ) );
					newEvent->SetTileRows( GetTileRow( ) );
					newEvent->SetVelX( 0 );
					newEvent->SetVelY( 0 );

					m_pEventTiles.push_back( newEvent );
				} break;
			};
		} in.close();
	}
}

void CLevel::RenderBackGround()
{
	if( GetBackGroundImage() != -1 )
		CSGD_TextureManager::GetInstance()->Draw( GetBackGroundImage(), 0,0 );
}

bool CLevel::LoadNextLevel(  )
{
	if( GetNextLevelFileName( ) )
	{
		string szTemp = "resource\\data\\";
		szTemp = szTemp + GetNextLevelFileName( );
		LoadNewLevel( ( char* )szTemp.c_str( ) );
		return true;
	}

	return false;
}

CBase* CLevel::CheckCollision( CBase* pBase  )
{
	int nX( int( pBase->GetPosX( ) ) );
	int nY( int( pBase->GetPosY( ) ) );
	int nX2( nX + pBase->GetWidth( ) );
	int nY2( nY + pBase->GetHeight( ) );

	nX   = nX / GetTileWidth( );
	nY   = nY / GetTileHeight( );
	nX2  = nX2 / GetTileWidth( );
	nY2  = nY2 / GetTileHeight( );

	CTerrainBase* pTerra;

	for( int i = nX; i <= nX2; i = i + 1 )
	{
		for( int j = nY; j <= nY2; j = j + 1 )
		{
			int index( i + j * GetWorldCollumn( ) );

			if( index <0 || index > int( m_pTerrainTiles.size( ) - 1 ) )
				continue;

			pTerra = ( CTerrainBase* )m_pTerrainTiles[index];
			if( pTerra->GetTileID( ) != GetBaseTileID()  && !pTerra->TerrainCollided( ) )
				return m_pTerrainTiles[index];
		}
	}

	for( int i = nX; i <= nX2; i = i + 1 )
	{
		for( int j = nY; j <= nY2; j = j + 1 )
		{
			int index( i + j * GetWorldCollumn( ) );
			if( index < 0 || index > int( m_pTerrainTiles.size( ) - 1 ) )
				continue;

			pTerra = ( CTerrainBase* )m_pTerrainTiles[index];
			pTerra->SetCollided( false );
		}
	}

	return NULL;
}

CBase* CLevel::GetEntryPoint()
{
	for( UINT i = 0; i < m_pEventTiles.size( ); ++i )
	{
		if( ( ( CTerrainBase* )m_pEventTiles[i] )->GetTypeTerrain( ) == ENTRY_POINT )
			return m_pEventTiles[i];
	}

	return NULL;
}

bool CLevel::IsOnScreen(CBase* Object)
{
	CCamera* theCamera( CCamera::GetCamera( ) );

	if( Object->GetPosX( ) + Object->GetWidth( ) >= theCamera->GetXOffset( ) 
		&& Object->GetPosX( ) <= theCamera->GetWidth( ) )
		if( Object->GetPosY( ) + Object->GetHeight( ) >= theCamera->GetYOffset( ) 
			&& Object->GetPosY( ) <= theCamera->GetHeight( ) )
			return true;
	return false;
}

void CLevel::Update( float fElapsedTime )
{
	CCamera* theCamera( CCamera::GetCamera( ) );

	RECT r;
	r.left      = LONG( theCamera->GetXOffset( ) - GetTileWidth( ) ) / GetTileWidth( );
	r.right     = LONG( theCamera->GetWidth( ) ) / GetTileWidth( );
	r.top       = LONG( theCamera->GetYOffset( ) - GetTileHeight( ) )/ GetTileHeight( );
	r.bottom    = LONG( theCamera->GetHeight( ) ) / GetTileHeight( );

	for( int i = min( r.left, 0 ); i <= min( r.right, GetWorldCollumn( ) ); i = i + 1 )
	{
		for( int j = min( r.top, 0 ); j <= min( r.bottom, GetWorldRow( ) ); j = j + 1 )
		{
			int index( i + j * GetWorldCollumn( ) );

			if( index < 0 || index > int( m_pTerrainTiles.size( ) - 1 ) )
				continue;

			if( ( ( CTerrainBase* )m_pTerrainTiles[index] )->GetTileID( ) != GetBaseTileID( ) )
				m_pTerrainTiles[index]->Update( fElapsedTime );
		}
	}
}

void CLevel::Render( )
{
	RenderBackGround( );
	CCamera* theCamera( CCamera::GetCamera( ) );
	RECT r;
	r.left     = LONG( theCamera->GetXOffset( ) - GetTileWidth( ) ) / GetTileWidth();
	r.right    = LONG( theCamera->GetWidth( ) ) / GetTileWidth( );
	r.top      = LONG( theCamera->GetYOffset( ) - GetTileHeight( ) ) / GetTileHeight( );
	r.bottom   = LONG( theCamera->GetHeight( ) ) / GetTileHeight( );

	for( int i = min( r.left, 0 ); i <= min( r.right, GetWorldCollumn( ) ); i = i + 1 )
	{
		for( int j = min( r.top, 0 ); j <= min( r.bottom, GetWorldRow( ) ); j = j + 1 )
		{
			int index( i + j * GetWorldCollumn( ) );

			if( index < 0 || index > int( m_pTerrainTiles.size( ) - 1 ) )
				continue;

			if( ( ( CTerrainBase* )m_pTerrainTiles[index] )->GetTileID( ) != GetBaseTileID( ) )
				m_pTerrainTiles[ index ]->Render( );
		}
	}
}