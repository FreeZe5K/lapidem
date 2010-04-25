//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CFire.h"
//
//  Author Name :   Jonathan Caro
//
//  Purpose     :	Handles the fire objects created by the player and
//					enemies. Handles all 3 tiers of spells.
//////////////////////////////////////////////////////////////////////////
#include "CFire.h"
#include "Wrappers/CSGD_TextureManager.h"
#include "CTerrainBase.h"
#include "CCamera.h"

CFire::CFire() :CSpell()
{
	SetImage(CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/fire.bmp",D3DCOLOR_XRGB(0,0,0)));
}

CFire::~CFire()
{

}

void CFire::Update(float fElapsedTime)
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

void CFire::UpdateTier1(float fElapsedTime)
{
	SetPosX(GetPosX()+GetVelX() * fElapsedTime);
	SetPosY(GetPosY()+GetVelY() * fElapsedTime);	
}

void CFire::UpdateTier2(float fElapsedTime)
{

}

void CFire::UpdateTier3(float fElapsedTime)
{

}

void CFire::Render()
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

void CFire::RenderTier1()
{
	if(GetImage() != -1)
	{
		CSGD_TextureManager::GetInstance()->Draw(GetImage(), (int)GetPosX() - CCamera::GetCamera()->GetXOffset(), (int)GetPosY()- CCamera::GetCamera()->GetYOffset());
	}
}

void CFire::RenderTier2()
{

}

void CFire::RenderTier3()
{

}



void CFire::HandleCollision(CBase* pObject)
{
	if(GetTier() ==1)
	{
		if(pObject->GetType() == OBJ_PLAYER)
		{
		}
		else if(pObject->GetType() == OBJ_TERRA)
		{
		//	if(((CTerrainBase*)pObject)->GetTypeTerrain() == T_ROCK);
		//	{
				((CTerrainBase*)pObject)->SetHealth(((CTerrainBase*)pObject)->GetHealth()- GetDamage());
				SetActive(false);
		//	}
		}
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

