//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CIce.h"
//
//  Author Name :   Jonathan Caro
//
//  Purpose     :	Handles the Ice objects created by the player and
//					enemies. Handles all 3 tiers of spells.
//////////////////////////////////////////////////////////////////////////
#include "CIce.h"
#include "Wrappers/CSGD_TextureManager.h"

CIce::CIce() :CSpell()
{

}

CIce::~CIce()
{

}

void CIce::Update(float fElapsedTime)
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

void CIce::UpdateTier1(float fElapsedTime)
{
	SetPosX(GetPosX()+GetVelX() * fElapsedTime);
	SetPosY(GetPosY()+GetVelY() * fElapsedTime);	
}

void CIce::UpdateTier2(float fElapsedTime)
{

}

void CIce::UpdateTier3(float fElapsedTime)
{

}

void CIce::Render()
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

void CIce::RenderTier1()
{
	if(GetImage() != -1)
	{
		CSGD_TextureManager::GetInstance()->Draw(GetImage(), (int)GetPosX(), (int)GetPosY());
	}
}

void CIce::RenderTier2()
{

}

void CIce::RenderTier3()
{

}


void CIce::HandleCollision(CBase* pObject)
{
	if(GetTier() ==1)
	{
		SetActive(false);
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

