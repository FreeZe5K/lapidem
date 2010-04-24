#include "CPlayer.h"


void CPlayer::Update(float fElapsedTime)
{
	CBase::Update(fElapsedTime);
	
	if( GetVelX() > 0 )
	{
		if(GetVelY() < 0)
			currDirec = RIGHT_UP;
		else if( GetVelY() > 0 )
			currDirec = RIGHT_DOWN;
		else
			currDirec = RIGHT;
	}
	else if( GetVelX() < 0)
	{
		if(GetVelY() < 0)
			currDirec = LEFT_UP;
		else if( GetVelY() > 0 )
			currDirec = LEFT_DOWN;
		else
			currDirec = LEFT;

	}


}

void CPlayer::Attack(int nTier)
{
	switch(m_SpellType)
	{
	case OBJ_EARTH:
		m_pSpells->CreateEarth(this, nTier);
		break;
	case OBJ_FIRE:
		m_pSpells->CreateFire(this, nTier);
		break;
	case OBJ_ICE:
		m_pSpells->CreateIce(this, nTier);
		break;
	case OBJ_WIND:
		m_pSpells->CreateWind(this, nTier);
		break;
	}
}