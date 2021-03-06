//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CTerrainBase.h"
//
//  Author Name :   Pablo Leon
//
//  Purpose     :   Contains shared information and functionality shared 
//					by the terrains but not included in CBase
//////////////////////////////////////////////////////////////////////////
#include "CTerrainBase.h"
#include "CSpell.h"
#include "stdheaders.h"

CTerrainBase::CTerrainBase( )
{ SetCollided( false ); m_fTransformTimer = -2.f;

	m_nTerrainType = -1;

	 m_nHealth = -1;
	 m_nDamage = -1;

	 m_nTileID = -1;

	 m_nBaseTileID = -1;
	 m_nTileCollumns = -1;
	 m_nTileRows = -1;


}

CTerrainBase::~CTerrainBase( )
{ /* NOTHING HERE YET */ }

RECT CTerrainBase::GetRectFromAlgorithm( int nTileID )
{
	RECT r;

	r.left     = ( nTileID % GetTileCollumns( ) ) * GetWidth( );
	r.top      = ( nTileID / GetTileCollumns( ) ) * GetHeight( );
	r.right    = r.left + GetWidth( );
	r.bottom   = r.top + GetHeight( );

	return r;
}

void CTerrainBase::Update( float fDT )
{
	CBase::Update( fDT );
	SetCollided( false );
	m_fTransformTimer -= fDT;
}

void CTerrainBase::HandleCollision(float fElapsedTime, CBase* pBase )
{
	if(pBase->GetType() <0)
		return;

	switch( pBase->GetType( ) )
	{
	case OBJ_SPELL:
		{

			if(GetType() ==  OBJ_TERRA)
			{
				if( ( ( CSpell* )pBase )->GetElement( ) == OBJ_EARTH )
				{
					if(GetTypeTerrain() == T_LAVA)
					{
						Corona_EventHandler::GetInstance()->SendEvent("SinkRock", (void*)pBase);
					}
					else if( ( ( CSpell *)pBase )->GetTier() == 1 && (pBase->GetVelX() > 15 || pBase->GetVelX() < -15 ))
						SetHealth(GetHealth() - ( (CSpell* )pBase )->GetDamage() );
				}
				else SetHealth( GetHealth( ) - ( ( CSpell* )pBase )->GetDamage( ) );

				if(GetHealth ( ) <= 0 && m_bIsActive)
				{
					if(GetTypeTerrain() == T_ROCK)
					{
						SetActive( false );
						SetTileID( GetBaseTile( ) );
						SetTypeTerrain(T_EMPTY);


						if( ( (CSpell*)pBase )->PlayerShot() )
						{
							if(CGameplayState::GetInstance()->GetPlayerOne()->GetPlayerCount() < 2)
								Corona_EventHandler::GetInstance()->SendEvent("TileDestroyed", (void*)CGameplayState::GetInstance()->GetPlayerOne());
							else if( ( (CSpell* )pBase )->GetElement( ) == OBJ_FIRE || ( (CSpell* )pBase )->GetElement( ) == OBJ_ICE)
								Corona_EventHandler::GetInstance()->SendEvent("TileDestroyed", (void*)CGameplayState::GetInstance()->GetPlayerOne());
							else
								Corona_EventHandler::GetInstance()->SendEvent("TileDestroyed", (void*)CGameplayState::GetInstance()->GetPlayerTwo());
						}
					}

				}
			}
		}
		break;
	};

	SetCollided( true );
}

void CTerrainBase::Render( ) 
{
	if( GetImage( ) != -1  && GetTileID( ) != GetBaseTile( ) )
		CSGD_TextureManager::GetInstance( )->Draw( GetImage( ), 
		int( GetPosX( ) - CCamera::GetCamera( )->GetXOffset( ) ), 
		int( GetPosY( ) - CCamera::GetCamera( )->GetYOffset( ) ), 
		1.0f, 1.0f,& GetRectFromAlgorithm( GetTileID( ) ) );
}