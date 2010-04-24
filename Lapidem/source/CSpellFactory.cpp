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


//////////////////////////////
//TODO: Incluce CCharacter
//////////////////////////////
#include "Corona_ObjectManager.h"
#include "CCharacter.h"
#include "CFire.h"
#include "CIce.h"
#include "CEarth.h"
#include "CWind.h"

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

void CSpellFactory::AddWindXP(int nXP)
{
	m_nWindXP += nXP;
	if(m_nWindXP> (35*(1.5*m_nWindLVL)))
	{
		m_nWindXP -=(int)(35*(1.5f*m_nWindLVL));
		m_nWindLVL++;
	}
}
void CSpellFactory::AddEarthXP(int nXP)
{
	m_nEarthXP += nXP;
	if(m_nEarthXP > (25+(40* m_nEarthLVL * 0.5)))
	{
		m_nEarthXP -= (int)(25+(40* m_nEarthLVL * 0.5f));
		m_nEarthLVL++;
	}
}
void CSpellFactory::AddFireXP(int nXP)
{
	m_nFireXP+= nXP;

	if(m_nFireXP > 50 * (m_nFireLVL +2))
	{
		m_nFireXP -= (50* (m_nFireLVL +2));
		m_nFireLVL++;
	}

}
void CSpellFactory::AddIceXP(int nXP)
{
	m_nIceXP+= nXP;

	if(m_nIceXP > 50 * (m_nIceLVL +2))
	{
		m_nIceXP -= (50* (m_nIceLVL +2));
		m_nIceLVL++;
	}
}


void CSpellFactory::CreateEarth(CCharacter* pShooter, int nTier)
{
	switch(nTier)
	{
	case 1: // First Tier... Basic Boulder
		{
			CEarth* newearth = new CEarth();
			newearth->SetPosX(pShooter->GetPosX());
			newearth->SetPosY(pShooter->GetPosY());
			///////////////////////////////////////////////////////////////////////////////////
			//TODO: once direction is implemented... change vel x and y base on that direction.
			///////////////////////////////////////////////////////////////////////////////////
			newearth->SetVelX(125.0f);
			newearth->SetVelY(-75.0f);
			///////////////////////////////////////////////////////////////////////////////////
			newearth->SetDamage(20 + 4* m_nEarthLVL);
			newearth->SetLifespan(10.0f + 1.5f* m_nEarthLVL);
			newearth->SetActive(true);
			newearth->SetTier(nTier);
			if(pShooter->GetType() == OBJ_PLAYER)
			{
				newearth->ShotBy(true);
			}
			else
			{
				newearth->ShotBy(false);
			}
			newearth->SetType(OBJ_EARTH);
			newearth->SetHeight(32);
			newearth->SetWidth(32);	
			Corona_ObjectManager::GetInstance()->AddObject(newearth);
			newearth->Release();
			break;
		}
	}
}

void CSpellFactory::CreateFire(CCharacter* pShooter, int nTier)
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
			newfire->SetActive(true);
			
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
			newfire->SetType(OBJ_FIRE);
			Corona_ObjectManager::GetInstance()->AddObject(newfire);
			newfire->Release();
			break;
		}
	}
}

void CSpellFactory::CreateIce(CCharacter* pShooter, int nTier)
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
			newice->SetSlow(10.0f + 2.0f * m_nIceLVL);
			newice->SetWidth(32);
			newice->SetHeight(16);
			newice->SetType(OBJ_ICE);
			newice->SetActive(true);
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
			
			Corona_ObjectManager::GetInstance()->AddObject(newice);
			newice->Release();
			break;
		}
	}	
}


void CSpellFactory::CreateWind(CCharacter* pShooter, int nTier)
{
	switch(nTier)
	{
	case 1: // First Tier... Basic wind Spell
		{
			CWind* newwind = new CWind();
			newwind->SetPosX(pShooter->GetPosX());
			newwind->SetPosY(pShooter->GetPosY());
			///////////////////////////////////////////////////////////////////////////////////
			//TODO: once direction is implemented... change vel x and y base on that direction.
			///////////////////////////////////////////////////////////////////////////////////
			newwind->SetVelX(200.0f);
			newwind->SetVelY(0.0f);
			///////////////////////////////////////////////////////////////////////////////////
			
			newwind->SetActive(true);
			newwind->SetType(OBJ_WIND);
			newwind->SetDamage(9 + m_nWindLVL);
			newwind->SetPushBack(10.0f + 5 *m_nWindLVL);
			newwind->SetLifespan(5.0f);
			newwind->SetTier(nTier);
			newwind->SetWidth(32);
			newwind->SetHeight(16);

			if(pShooter->GetType() == OBJ_PLAYER)
			{
				newwind->ShotBy(true);
			}
			else
			{
				newwind->ShotBy(false);
			}
			Corona_ObjectManager::GetInstance()->AddObject(newwind);
			newwind->Release();

			break;
		}
	}	
}