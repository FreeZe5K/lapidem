//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CWind.h"
//
//  Author Name :   Jonathan Caro
//
//  Purpose     :	Handles the wind objects created by the player and
//					enemies. Handles all 3 tiers of spells.
//////////////////////////////////////////////////////////////////////////
#include "CWind.h"
#include "Wrappers/CSGD_TextureManager.h"
#include "CCamera.h"

CWind::CWind() : CSpell()
{
	SetImage(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/wind.bmp",D3DCOLOR_XRGB(0,0,0)));

}

CWind::~CWind()
{
	
	if( GetImage() >= 0 )
	CSGD_TextureManager::GetInstance()->UnloadTexture( GetImage() );


}


void CWind::Update(float fElapsedTime)
{
	SetLifespan(GetLifespan() - fElapsedTime);
	if(GetLifespan() < 0)
	{
		SetActive(false);
	}
	switch(GetTier())
	{
	case 1:
		{
			UpdateTier1(fElapsedTime);
			break;
		}
	case 2:
		{
			UpdateTier2(fElapsedTime);
			break;
		}
	case 3:
		{
			UpdateTier3(fElapsedTime);
			break;
		}
	}
}

void CWind::UpdateTier1(float fElapsedTime)
{
	SetPosX(GetPosX()+GetVelX() * fElapsedTime);
	SetPosY(GetPosY()+GetVelY() * fElapsedTime);	
}

void CWind::UpdateTier2(float fElapsedTime)
{

}

void CWind::UpdateTier3(float fElapsedTime)
{

}

void CWind::Render()
{
	switch(GetTier())
	{
	case 1:
		{
			RenderTier1();
			break;
		}
	case 2:
		{
			RenderTier2();
			break;
		}
	case 3:
		{
			RenderTier3();
			break;
		}
	}
}

void CWind::RenderTier1()
{
	if(GetImage() != -1)
	{
		CSGD_TextureManager::GetInstance()->Draw( GetImage(), int( GetPosX() - 
			CCamera::GetCamera()->GetXOffset() ), int( GetPosY()- CCamera::GetCamera()->GetYOffset() ) );
	}
}

void CWind::RenderTier2()
{

}

void CWind::RenderTier3()
{

}



void CWind::HandleCollision(CBase* pObject)
{
	if(GetTier() ==1)
	{
		if(pObject->GetType() == OBJ_TERRA)
		{
		//	if(((CTerrainBase*)pObject)->GetTypeTerrain() == T_ROCK);
		//	{
				//((CTerrainBase*)pObject)->SetHealth(((CTerrainBase*)pObject)->GetHealth()- GetDamage());
				SetActive(false);
		//	}
		}

		/* 
		if vel == + push them right 
		else push them left by PushBack
		*/
	}
	else if(GetTier() ==2)
	{
		//do stuff... like destroy... EVERYTHING
	}
	else if(GetTier() ==3)
	{
		// holy crap everything go splode
	}

}