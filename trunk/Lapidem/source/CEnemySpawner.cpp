#include "CEnemySpawner.h"
#include "Corona_ObjectManager.h"
#include "CEnemy.h"

void CEnemySpawner::HandleEvent(CEvent* pEvent)
{
	if(!strcmp(pEvent->GetEventID().c_str(), "OffScreenPulse") && !m_bIsOnScreen)
	{
		if(m_bIsReadyToSpawn)
		{
			m_pEnemy = new CEnemy(OBJ_EARTH);
			m_pEnemy->SetPosX(GetPosX());
			m_pEnemy->SetPosY(GetPosY());
			Corona_ObjectManager::GetInstance()->AddObject(m_pEnemy);
			m_bIsReadyToSpawn = false;
		}
	}
	else if(!strcmp(pEvent->GetEventID().c_str(), "EnemyDied") && pEvent->GetData1() == m_pEnemy)
	{
		m_pEnemy->Release();
		m_pEnemy = NULL;
		m_bIsReadyToSpawn = true;
	}
}