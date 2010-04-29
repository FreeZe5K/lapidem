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


CTerrainBase::CTerrainBase()
{
	SetCollided(false);

}
CTerrainBase::~CTerrainBase()
{

}

RECT CTerrainBase::GetRectFromAlgorithm( int nTileID )
{
	RECT r;

	r.left = (nTileID % GetTileCollumns())*GetWidth();
	r.top = (nTileID / GetTileCollumns())*GetHeight();
	r.right = r.left + GetWidth();
	r.bottom = r.top + GetHeight();

	return r;
}

void CTerrainBase::Update( float fDT)
{
	CBase::Update(fDT);

	SetCollided(false);

	if( GetTypeTerrain() == T_ROCK )
	if( GetHealth() <= 0 )
	{
		SetTileID( GetBaseTile());
		SetActive(false);
	}

}


void CTerrainBase::HandleCollision(CBase* pBase)
{
	switch( pBase->GetType() )
	{
	case OBJ_SPELL:
		{
			if(((CSpell*)pBase)->GetElement() == OBJ_EARTH)
				SetHealth( GetHealth() - 0 *((CSpell*)pBase)->GetDamage() );
			else
				SetHealth( GetHealth() - ((CSpell*)pBase)->GetDamage() );

		}
		break;
	


	};
		SetCollided(true);

}


void CTerrainBase::Render( ) 
{
	if( GetImage( ) != -1  && GetTileID() != GetBaseTile() )
		CSGD_TextureManager::GetInstance( )->Draw( GetImage( ), ( int )(GetPosX( ) - CCamera::GetCamera()->GetXOffset()), ( int )(GetPosY( ) - CCamera::GetCamera()->GetYOffset()), 1.0f, 1.0f,& GetRectFromAlgorithm(GetTileID()) );

}
