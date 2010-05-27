#include "CWaterTerrain.h"
#include "Stdheaders.h"
#include "CEnemy.h"
#include "CEarth.h"
#include "CSpell.h"

CWaterTerrain::CWaterTerrain()
{
	m_bIsFlooding = false;
	m_bLeftUnFloodable = m_bRightUnFloodable = m_bTopUnFloodable = false;
	m_fFloodSpeed = 0.2f;
	m_fCurrTime = 0.0f;
	m_nCurrDistance = 0;
	m_nNumFlooded = 0;
	m_nDistanceToFlood = 0;
	m_pControlTile = 0;

	//Corona_EventHandler::GetInstance()->RegisterClient(this, "Flood Tile");
	//Corona_EventHandler::GetInstance()->RegisterClient(this, "Flood Level");
}

CWaterTerrain::~CWaterTerrain()
{
	//Corona_EventHandler::GetInstance()->UnregisterClient(this, "Flood Tile");
	//Corona_EventHandler::GetInstance()->UnregisterClient("Flood Level", this);
}

void CWaterTerrain::Update(float fElapsedTime)
{
		if(m_nCurrDistance > m_nDistanceToFlood)
	{
		m_bIsFlooding = false;
		/*this->SetTileID(0);
		this->SetDamage(0);*/


		this->SetActive(false);
	}

	if(m_bIsFlooding)
	{
		m_fCurrTime += fElapsedTime;

		CTerrainBase* pLeftTerrain = 0, *pRightTerrain = 0, *pTopTerrain;

		if(m_fCurrTime >= m_fFloodSpeed)
		{
			CLevel* pLevel = CGameplayState::GetInstance()->GetLevel();
			if(!m_bLeftUnFloodable)
			{
				pLeftTerrain = (CTerrainBase*)pLevel->GetTile((int)this->GetPosX() - m_nNumFlooded * this->GetWidth(), (int)this->GetPosY());
				if(pLeftTerrain->GetTypeTerrain() != T_BOUNDARY)
				{
					pLeftTerrain->SetTypeTerrain(T_WATER);
					pLeftTerrain->SetTileID(m_nWaterTileID);//m_nTileIDs[m_bIsFlooding]);
					pLeftTerrain->SetHealth(100);
					pLeftTerrain->SetDamage(1);
				}
				else
					m_bLeftUnFloodable = true;
			}
			if(!m_bRightUnFloodable)
			{
				pRightTerrain = (CTerrainBase*)pLevel->GetTile((int)this->GetPosX() + m_nNumFlooded * this->GetWidth(), (int)this->GetPosY());
				if(pRightTerrain->GetTypeTerrain() != T_BOUNDARY)
				{
					pRightTerrain->SetTypeTerrain(T_WATER);
					pRightTerrain->SetTileID(m_nWaterTileID);//m_nTileIDs[m_bIsFlooding]);
					pRightTerrain->SetHealth(100);
					pRightTerrain->SetDamage(1);
				}
				else
					m_bRightUnFloodable = true;
			}
			if(!m_bTopUnFloodable)
			{
				if(m_bLeftUnFloodable && m_bRightUnFloodable && m_nCurrDistance < m_nDistanceToFlood)
				{
					pTopTerrain = (CTerrainBase*)pLevel->GetTile((int)this->GetPosX(), (int)this->GetPosY() - this->GetHeight());
					if(pTopTerrain->GetTypeTerrain() != T_BOUNDARY)
					{
						pTopTerrain->SetTypeTerrain(T_WATER);
						pTopTerrain->SetTileID(m_nWaterTileID);
						pTopTerrain->SetHealth(100);
						pTopTerrain->SetDamage(1);

						m_bLeftUnFloodable = m_bRightUnFloodable = false;


						m_pControlTile = pTopTerrain;
						this->SetPosX(m_pControlTile->GetPosX());
						this->SetPosY(m_pControlTile->GetPosY());
						
					}
					else
						m_bTopUnFloodable = true;

					++m_nCurrDistance;
					m_nNumFlooded = 0;
					//this->SetPosY(this->GetPosY() - m_nCurrDistance * this->GetHeight());
				}
			}

			++m_nNumFlooded;
			m_fCurrTime = 0.0f;
		}	//m_fCurrTime >= m_fFloodSpeed
		if(m_bLeftUnFloodable && m_bRightUnFloodable && m_bTopUnFloodable || m_nCurrDistance >= m_nDistanceToFlood)
			m_bIsFlooding = false;


	}

	if(Corona_ObjectManager::GetInstance()->IsOnScreen(m_pControlTile) && !m_bIsFlooding)
		m_bIsFlooding = true;


}

void CWaterTerrain::HandleCollision(CBase* pBase)
{	
	if(pBase->GetType() < 0)
		return;

	if(m_bIsFlooding)
	{
		if((pBase->GetType() == OBJ_ENEMY && ((CEnemy*)pBase)->GetEleType() != OBJ_ICE))
		{
			if(this->GetPosY() <= pBase->GetPosY())
			{
				((CCharacter*)pBase)->TakeDamage(this->GetDamage());
			}
		}
	}

	if(pBase->GetType() == OBJ_SPELL)
	{
		if(((CSpell*)pBase)->GetElement() == OBJ_EARTH)
			return;
		else if( ( (CSpell*)pBase)->GetElement() == OBJ_ICE)
		{
			Corona_EventHandler::GetInstance()->SendEvent("CreateIce", (void*)(new CEarth()), (void*)this);
		}
		else SetHealth(GetHealth() - ((CSpell* )pBase)->GetDamage());
	}
}

//void CWaterTerrain::HandleEvent(CEvent* pEvent)
//{
//	if(pEvent->GetEventID() == "Flood Level")
//	{
//		if(pEvent->GetData1()
//		m_bIsFlooding = true;
//	}
//}

//void CWaterTerrain::Render()
//{
//	CCamera* theCamera = CCamera::GetCamera();
//	if(m_bIsFlooding)
//	{
//		CSGD_TextureManager::GetInstance()->Draw(this->GetImage(),
//			(int)(this->GetPosX() - theCamera->GetXOffset()),
//			(int)(this->GetPosY() - theCamera->GetYOffset()), 
//			1.0f, 1.0f, &GetRectFromAlgorithm(m_nWaterTileID));
//	}
//}
