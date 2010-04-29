#include "CEnemySpawner.h"
#include "Corona_ObjectManager.h"
#include "CEnemy.h"

void CEnemySpawner::HandleEvent(CEvent* pEvent)
{
	if(!strcmp(pEvent->GetEventID().c_str(), "OffScreenPulse") && !m_bIsOnScreen)
	{
		if(m_bIsReadyToSpawn)
		{
			m_pEnemy = new CEnemy(OBJ_EARTH, GetPosX(), GetPosY());
			Corona_ObjectManager::GetInstance()->AddObject(m_pEnemy);
			m_bIsReadyToSpawn = false;
		}
	}
	else if(!strcmp(pEvent->GetEventID().c_str(), "EnemyDied") && pEvent->GetData1() == m_pEnemy)
	{
		if(m_pEnemy)
		m_pEnemy->Release();
		m_pEnemy = NULL;
		m_bIsReadyToSpawn = true;
	}
	else if(!strcmp(pEvent->GetEventID().c_str(), "EnemyDied") && pEvent->GetData1() == NULL)
	{
		if(m_pEnemy)
		m_pEnemy->Release();
		m_pEnemy = NULL;
	}
}