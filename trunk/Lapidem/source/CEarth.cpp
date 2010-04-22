#include "CEarth.h"
#include "Wrappers/CSGD_TextureManager.h"
#include <math.h>

const float PI = 3.14159f;


struct Vector2d
{
	float fX;
	float fY;
};
Vector2d Rotate(Vector2d vRotateme, float fRadian)
{
	vRotateme.fY *= -1.0f;
	
	Vector2d HahaRotated;
	HahaRotated.fX = (cos(fRadian) * vRotateme.fX) + (sin(fRadian) * vRotateme.fY);
	HahaRotated.fY = (-sin(fRadian) * vRotateme.fX) + (cos(fRadian) * vRotateme.fY);

	return HahaRotated;
}
CEarth::CEarth() : CSpell()
{
	m_fRotate = PI/4.0f;
	m_fDisplay = m_fRotate;
	m_fTimeTillRotate = 1.0f;
}

CEarth::~CEarth()
{

}


void CEarth::Update(float fElapsedTime)
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

void CEarth::UpdateTier1(float fElapsedTime)
{
	
	SetVelY(GetVelY() + 0.25f);
	if(m_fTimeTillRotate > 0)
	{
		m_fTimeTillRotate -= fElapsedTime;
	}
	else if(GetVelX() != 0 && GetVelY() !=0)
	{
		m_fDisplay += fElapsedTime;
		if(m_fDisplay > PI)
		{
			m_fDisplay = PI;
		}
		Vector2d spin;
		spin.fX = GetPosX() + GetVelX();// * fElapsedTime;
		spin.fY = GetPosY() + GetVelY();// * fElapsedTime;

		Vector2d currpoint;
		currpoint.fX = GetPosX();
		currpoint.fY = GetPosY();

		//dotproduct
		float dot = (spin.fX * currpoint.fX) + (currpoint.fY * spin.fY);
		// length * length
		float length = sqrt( (spin.fX * spin.fX) + (spin.fY * spin.fY)) * sqrt((currpoint.fX * currpoint.fX) + (currpoint.fY * currpoint.fY));

		if(length  != 0.0f)
		{
			m_fRotate = acos(dot/length) + PI /4.0f;

			Vector2d rotate;
			rotate.fX = 0;
			rotate.fY = GetVelY() * fElapsedTime;

			Vector2d rotated = Rotate(rotate , m_fRotate);
			
			SetVelY(GetVelY() + rotated.fY);
			SetVelX(GetVelX() + rotated.fX);
			if( GetVelX() <30)
			{
				SetVelX(30);
			}
			if(GetVelY() > 450)
			{
				SetVelY(450);
			}

		}
		spin.fX = GetPosX() + GetVelX();// * fElapsedTime;
		spin.fY = GetPosY() + GetVelY();// * fElapsedTime;

		currpoint.fX = GetPosX();
		currpoint.fY = GetPosY();

		//dotproduct
		dot = (spin.fX * currpoint.fX) + (currpoint.fY * spin.fY);
		// length * length
		length = sqrt( (spin.fX * spin.fX) + (spin.fY * spin.fY)) * sqrt((currpoint.fX * currpoint.fX) + (currpoint.fY * currpoint.fY));
		m_fRotate = acos(dot/length) + PI/4.0f;
	}

	CBase::Update(fElapsedTime);
	
	if(GetPosX() <0)
	{
		SetPosX(0);
		SetVelX(0);
	}
	/*else if(GetPosY() + GetHeight() > CGame::GetInstance()->GetScreenHeight())
	{
		SetPosY((float)(CGame::GetInstance()->GetScreenHeight() - GetHeight()));
		SetVelY(0);
		SetVelX(0);
		m_fDisplayrotate = SGD_PI;
	}*/
}

void CEarth::UpdateTier2(float fElapsedTime)
{

}

void CEarth::UpdateTier3(float fElapsedTime)
{

}

void CEarth::Render()
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

void CEarth::RenderTier1()
{
	if(GetImage() != -1)
	{
		RECT boulder;// = {0,0,64,64};
		CSGD_TextureManager::GetInstance()->Draw(GetImage(), (int)GetPosX(), (int)GetPosY(),1,1,&boulder,GetWidth()/2.0f,GetHeight()/2.0f,m_fRotate);
	}
}

void CEarth::RenderTier2()
{

}

void CEarth::RenderTier3()
{

}

bool CEarth::CheckCollision(CBase* pbase)
{
	if(IntersectRect(0,&GetCollisionRect(), &pbase->GetCollisionRect()))
	{
		HandleCollision(pbase);
		return true;
	}
	else
		return false;
}

void CEarth::HandleCollision(CBase* pObject)
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