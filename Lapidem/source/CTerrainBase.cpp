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
#include "CCamera.h"
#include "Wrappers/CSGD_TextureManager.h" 
#include "CGamePlayState.h"
#include "Corona_EventHandler.h"

CTerrainBase::CTerrainBase( )
{ SetCollided( false ); m_fTransformTimer = -2.f; }

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

void CTerrainBase::HandleCollision( CBase* pBase )
{
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