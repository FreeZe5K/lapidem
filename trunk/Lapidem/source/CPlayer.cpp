#include "CPlayer.h"
#include "CSpell.h"
#include "CGame.h"


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

	SetVelY(150);

	if(m_bIsJumping)
	{
		m_fJumpTimer += fElapsedTime;

		if(m_fJumpTimer <= .75)
			SetVelY(-100);
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
		if(GetPosX() > collidingObject->GetPosX() && 
			GetPosX() + (GetWidth() *.25) < collidingObject->GetPosX() + collidingObject->GetWidth())
		{
			//From Above:
		if(GetPosY() < collidingObject->GetPosY())
			{
				SetVelY( 0.0f );
				SetPosY( collidingObject->GetPosY() - GetHeight() - 1);
				m_bIsJumping = false;
				m_fJumpTimer = 0.0f;
			}
			//From Below
			else
				SetPosY(collidingObject->GetPosY() + collidingObject->GetHeight() + 1);

			return;

		}
		//We've hit the tile from the left or right side.
		else
		{
			if(GetPosX() < collidingObject->GetPosX())
				SetPosX(collidingObject->GetPosX() - GetWidth());

			else if(GetPosY() + GetHeight() != collidingObject->GetPosY() + 1.25f)
				SetPosX(collidingObject->GetPosX() + collidingObject->GetWidth() + 1);

			return;
		}

		/*RECT collRect = {};

		IntersectRect(&collRect, &GetCollisionRect(CGame::GetInstance()->GetElapsedTime()), &collidingObject->GetCollisionRect(CGame::GetInstance()->GetElapsedTime()));*/

		//If the height is less than the top.
		//   __________
		//   |	      |
		//	 ----------
		/*if(collRect.bottom - collRect.top <= collRect.right - collRect.left)
		{
			if(collRect.top == collidingObject->GetPosY())
			{
				SetVelY( 0.0f );
				SetPosY( collidingObject->GetPosY() - GetHeight() - 1);
				m_bIsJumping = false;
				m_fJumpTimer = 0.0f;
			}
			else
				SetPosY(collidingObject->GetPosY() + collidingObject->GetHeight() + 1);

		}
		else
		{
			if(collRect.left == collidingObject->GetPosX())
				SetPosX(collidingObject->GetPosX() - GetWidth());
			else
				SetPosX(collidingObject->GetPosX() + collidingObject->GetHeight() + 1);

		}*/
	}

	else if(collidingObject->GetType() == OBJ_SPELL && !((CSpell*)collidingObject)->PlayerShot())
	{
		m_nHealth -= ((CSpell*)collidingObject)->GetDamage();
	}
}