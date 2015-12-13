#include "CFlock.h"
#include "CEnemy.h"
#include "CPlayer.h"
#include "AIStateWind.h"
#include "Corona_ObjectManager.h"
#include <cmath>

CFlock::CFlock()
{
	m_fInfluence = 0.0f;
	SetType(OBJ_FLOCK);
	m_bAttacking = false;
	m_pAttackTarget = NULL;
}

CFlock::~CFlock()
{ }

void CFlock::AddMember(CEnemy* addme)
{
	m_vFlock.push_back(addme);
	if(m_vFlock.size() == 1)
	{
		m_pLeader = addme;
		SetPosX(addme->GetPosX());
		SetPosY(addme->GetPosY());
	}

	((AIStateWind*)addme->GetState())->SetFlock(this);

	m_fInfluence += 75;

}

void CFlock::RemoveMember(CEnemy* removeme)
{
	if(removeme == NULL)
		return;

	if(m_vFlock.size() > 0)
	{
		vector<CEnemy*>::iterator iter = m_vFlock.begin();

		while(iter != m_vFlock.end())
		{
			if(*iter == removeme)
			{
				if(removeme == m_pLeader)
				{
					ChangeLeader();
					m_fShockTimer = 0.5f;
				}
				m_fInfluence -= 75;
				((AIStateWind*)removeme->GetState())->SetFlock(NULL);
				m_vFlock.erase(iter);
				return;
			}
			++iter;
		}
	}
	else 
	{
		SetActive(false);
	}
}

float CFlock::CalculateDistance(float OrigPosX, float OrigPosY, float NewPosX, float NewPosY)
{
	NewPosX = OrigPosX - NewPosX;
	NewPosX = NewPosX * NewPosX;

	NewPosY = OrigPosY - NewPosY;
	NewPosY = NewPosY * NewPosY;

	return sqrt(NewPosX + NewPosY);

}
void CFlock::ChangeLeader()
{
	if(m_vFlock.size() >1)
	{
		float closestDist= 0;

		int closestID =0;

		for(unsigned int i = 0; i < m_vFlock.size(); i++)
		{
			if(m_vFlock[i] == m_pLeader)
			{
				continue;
			}	
			float distance = CalculateDistance(m_pLeader->GetPosX(), m_pLeader->GetPosY(),m_vFlock[i]->GetPosX(), m_vFlock[i]->GetPosY());
			if(closestDist ==0)
			{
				closestDist = distance;
				closestID = i;
			}
			else if(distance < closestDist)
			{
				closestDist = distance;
				closestID = i;
			}
		}
		m_pLeader = m_vFlock[closestID];
	}
}

void CFlock::Update(float fElapsedTime)
{
	if(m_vFlock.size() == 0)
	{
		SetActive(false);
		return;
	}

	SetPosX(m_pLeader->GetPosX());
	SetPosY(m_pLeader->GetPosY());
	SetWidth((int)m_fInfluence);
	SetHeight((int)m_fInfluence);
	if(m_fShockTimer > 0)
	{
		m_fShockTimer -= fElapsedTime;
	}

}

bool CFlock::GetShocked(void)
{
	if(m_fShockTimer >0)
	{
		return true;
	}
	return false;
}

bool CFlock::InRadius(CEnemy* pBird)
{
	
	if(CalculateDistance(GetPosX(), GetPosY(), pBird->GetPosX(), pBird->GetPosY()) < m_fInfluence)
		return true;

	return false;
}

float CFlock::GetAverageXVel(float nposx, float nposy, float radius)
{
	float total = 0;
	float numinradius =0;

	for(unsigned int i =0; i < m_vFlock.size(); i++)
	{
		if(CalculateDistance(nposx, nposy, m_vFlock[i]->GetPosX(), m_vFlock[i]->GetPosY()) < radius)
		{
			total += m_vFlock[i]->GetVelX();
			numinradius++;
		}
	}
	return (float)(total/numinradius);
}

float CFlock::GetAverageYVel(float nposx, float nposy, float radius)
{
	float total = 0;
	float numinradius = 0;
	
	for(unsigned int i =0; i < m_vFlock.size(); i++)
	{
		if(CalculateDistance(nposx, nposy, m_vFlock[i]->GetPosX(), m_vFlock[i]->GetPosY()) < radius)
		{
			total += m_vFlock[i]->GetVelY();
			numinradius++;
		}
	}
	return (float)(total/numinradius);
}

void CFlock::HandleCollision(CBase* pObject)
{
	if(pObject->GetType() < 0)
		return;


	if(pObject->GetType() == OBJ_FLOCK)
	{

		CFlock* collideFlock = (CFlock*)pObject;

		if(CalculateDistance(GetPosX(),GetPosY(), pObject->GetPosX() , pObject->GetPosY()) < m_fInfluence)
		{

			if(m_bAttacking && !collideFlock->IsAttacking())
			{
				MergeFlocks(this, collideFlock);
			}
			else if(!m_bAttacking && collideFlock->IsAttacking())
			{
				MergeFlocks(collideFlock, this);
			}
			else
			{
				if(m_vFlock.size() > collideFlock->GetFlock().size())
				{
					MergeFlocks(this,collideFlock);
				}
				else
				{
					MergeFlocks(collideFlock,this);
				}
			}
		}
	}
}

void CFlock::MergeFlocks(CFlock* pMergeFlock, CFlock* pDestroyedFlock)
{
	vector<CEnemy*> destroyedFlock = pDestroyedFlock->GetFlock();

	vector<CEnemy*> newFlock = pMergeFlock->GetFlock();

	for(;destroyedFlock.size();)
	{
		newFlock.push_back(destroyedFlock[0]);
		// write a get aistate function in cenemy... create a set flock function in aistate... adjust its flock with the new flock
		((AIStateWind*)destroyedFlock[0]->GetState())->SetFlock(pMergeFlock);
		pDestroyedFlock->RemoveMember(destroyedFlock[0]);
		destroyedFlock = pDestroyedFlock->GetFlock();
	}
	pDestroyedFlock->SetActive(false);
}

