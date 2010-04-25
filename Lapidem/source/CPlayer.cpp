#include "CPlayer.h"
#include "CSpell.h"


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

	if(m_bIsJumping)
	{
		m_fJumpTimer += fElapsedTime;

		if(m_fJumpTimer <= .5)
			SetVelY(-100);
		else if(GetVelY()!= 0)
			SetVelY(150);
		else
		{
			m_bIsJumping = false;
			m_fJumpTimer = 0.0f;
		}
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

void CPlayer::Jump()
{
	if(m_bIsJumping)
		return;

	m_bIsJumping = true;
	//m_fJumpTimer = 0.0f;
}

void CPlayer::HandleCollision(CBase * collidingObject)
{
	if(collidingObject->GetType() == OBJ_TERRA)
	{
		//If we've hit the tile from above or below.
		if(GetPosX() > collidingObject->GetPosX() && GetPosX() < collidingObject->GetPosX() + collidingObject->GetWidth())
		{
			//From Above:
		if(GetPosY() < collidingObject->GetPosY())
			{
				//Set the Velocity to 0, the shuttle has landed.
				SetVelY( 0.0f );
				//Check for ground penetration, correct if needed
				if(GetPosY() + GetHeight() > collidingObject->GetPosY())
					SetPosY( collidingObject->GetPosY() - GetHeight() - 1);
			}
			//From Below
			else
				SetPosY(collidingObject->GetPosY() + collidingObject->GetHeight() + 1);

			return;

		}
		//We've hit the tile from the left or right side.
		else
		{
			



			return;
		}
	}
	else if(collidingObject->GetType() == OBJ_SPELL && !((CSpell*)collidingObject)->PlayerShot())
	{
		m_nHealth -= ((CSpell*)collidingObject)->GetDamage();
	}
}