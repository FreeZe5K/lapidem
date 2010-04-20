//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CSpellFactory.cpp"
//
//  Author Name :   Jonathan Caro
//
//  Purpose     :   Used to create all spells. Also handles the experience
//					gains and levels of each element. After Creation, the
//					spell is put into the ObjectFactory as well as 
//					returned.
//////////////////////////////////////////////////////////////////////////
#include "CSpellFactory.h"



///////////////////////////////////////
//TODO:Remove once CCharacter is added
///////////////////////////////////////
#include "CBase.h"
///////////////////////////////////////

//////////////////////////////
//TODO: Include ObjectManager
//		Incluce CCharacter
//		Include CEarth
//		Include CWind
//////////////////////////////
#include "CFire.h"
#include "CIce.h"

CSpellFactory* CSpellFactory::m_pSF = NULL;

CSpellFactory::CSpellFactory()
{
	m_nEarthLVL = 0;
	m_nWindLVL = 0;
	m_nFireLVL = 0;
	m_nIceLVL = 0;

	m_nEarthXP = 0;
	m_nWindXP = 0;
	m_nFireXP = 0;
	m_nIceXP = 0;
}


CSpellFactory::~CSpellFactory()
{
	DeleteInstance();
}

//CEarth* CSpellFactory::CreateEarth(CBase* pShooter, int nTier)
//{
//	switch(nTier)
//	{
//		
//	}
//}

CFire* CSpellFactory::CreateFire(CBase* pShooter, int nTier)
{
	switch(nTier)
	{
	case 1: // First Tier... Basic Spell
		{
			CFire* newfire = new CFire();
			newfire->SetPosX(pShooter->GetPosX());
			newfire->SetPosY(pShooter->GetPosY());
			///////////////////////////////////////////////////////////////////////////////////
			//TODO: once direction is implemented... change vel x and y base on that direction.
			///////////////////////////////////////////////////////////////////////////////////
			newfire->SetVelX(150);
			newfire->SetVelY(0);
			///////////////////////////////////////////////////////////////////////////////////
			newfire->SetDamage(12 + (3 * m_nFireLVL));
			newfire->SetDOT(3 + (1 * (m_nFireLVL>>1)));
			newfire->SetLifespan(5.0f);
			
			newfire->SetTier(nTier);
			if(pShooter->GetType() == OBJ_PLAYER)
			{
				newfire->ShotBy(true);
			}
			else
			{
				newfire->ShotBy(false);
			}
			newfire->SetWidth(32);
			newfire->SetHeight(16);
			//TODO: newfire->SetType(OBJ_FIRE);
			
			///////////////////////////////////
			//TODO: Add to ObjectManager
			///////////////////////////////////
			return newfire;
			break;
		}
	}
	return 0;
}

CIce* CSpellFactory::CreateIce(CBase* pShooter, int nTier)
{
	switch(nTier)
	{
	case 1: // First Tier... Basic Spell
		{
			CIce* newice = new CIce();

			newice->SetPosX(pShooter->GetPosX());
			newice->SetPosY(pShooter->GetPosY());
			///////////////////////////////////////////////////////////////////////////////////
			//TODO: once direction is implemented... change vel x and y base on that direction.
			///////////////////////////////////////////////////////////////////////////////////
			newice->SetVelX(150);
			newice->SetVelY(0);
			///////////////////////////////////////////////////////////////////////////////////
			newice->SetDamage(9 + 2* m_nIceLVL);
			newice->SetSlow(10 + 2 * m_nIceLVL);
			newice->SetWidth(32);
			newice->SetHeight(16);
			//TODO: newice->SetType(OBJ_ICE);
			newice->SetLifespan(5.0f);

			if(pShooter->GetType() == OBJ_PLAYER)
			{
				newice->ShotBy(true);
			}
			else
			{
				newice->ShotBy(false);
			}

			newice->SetTier(nTier);
			
			return newice;
			break;
		}
	}	
}