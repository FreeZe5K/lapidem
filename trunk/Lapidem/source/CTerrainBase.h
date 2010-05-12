//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CTerrainBase.h"
//
//  Author Name :   Pablo Leon
//
//  Purpose     :   Contains shared information and functionality shared 
//					by the terrains but not included in CBase
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "CBase.h"

enum TerrainType  { T_EMPTY, T_BOUNDARY, T_ROCK, T_WATER, T_LAVA, T_AIR, MAX_TERRAIN };
enum EventType    { NONE, WATER_THR, ENEMY_SPW, END_POINT, ENTRY_POINT, AIR_CURRENT,POWERUP, MAX_EVENTS };

class CTerrainBase : public CBase
{
private:
	int m_nTerrainType;

	int m_nHealth;
	int m_nDamage;

	int m_nTileID;

	int m_nBaseTileID;
	int m_nTileCollumns;
	int m_nTileRows;

	bool m_bCollided;

protected:
	float m_fTransformTimer;

public:
	CTerrainBase( );
	~CTerrainBase( );

	RECT GetRectFromAlgorithm( int nTileID );
	
	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Accessors"
	//////////////////////////////////////////////////////////////////////////
	int GetHealth( )              const         { return m_nHealth;         }
	int GetDamage( )              const         { return m_nDamage;         }
	int GetTileID( )              const         { return m_nTileID;         }
	int GetTileCollumns( )        const         { return m_nTileCollumns;   }
	int GetTileRows( )            const         { return m_nTileRows;       }
	int GetBaseTile( )            const         { return m_nBaseTileID;     }
	int GetTypeTerrain( )         const         { return m_nTerrainType;    }
	bool TerrainCollided( )       const         { return m_bCollided;       }
	
	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Mutators"
	//////////////////////////////////////////////////////////////////////////
	void SetHealth( int nHealth )                  { m_nHealth         = nHealth;        }
	void SetDamage( int nDamage )                  { m_nDamage         = nDamage;        }
	void SetTileID( int nTileID )                  { m_nTileID         = nTileID;        }
	void SetTileCollumns( int nTileCollumns )      { m_nTileCollumns   = nTileCollumns;  }
	void SetBaseTileID( int nBaseTileID )          { m_nBaseTileID     = nBaseTileID;    }
	void SetTileRows( int nTileRows )              { m_nTileRows       = nTileRows;      }
	void SetTypeTerrain( int nTerrainType )        { m_nTerrainType    = nTerrainType;   }
	void SetCollided( bool bCollided )             { m_bCollided       = bCollided;      }

	//////////////////////////////////////////////////////////////////////////
	//	Function	: "HandleCollision"
	//
	//	Purpose		:	Reaction to a collision in this object in response to another
	//////////////////////////////////////////////////////////////////////////
	void HandleCollision( CBase* );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Update"
	//
	//  Purpose   :   This method is in charge of updating the object based on
	//				  its position and velocity. If Input is needed, it should
	//				  be added before update code.
	//////////////////////////////////////////////////////////////////////////
	void Update( float );

	//////////////////////////////////////////////////////////////////////////
	//  Function  :   "Render"
	//
	//  Purpose   :   This method is responsible for drawing the object to 
	//				  the screen.
	//////////////////////////////////////////////////////////////////////////
	void Render( );
};