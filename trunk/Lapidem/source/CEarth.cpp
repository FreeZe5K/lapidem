#include "CEarth.h"
#include "Wrappers/CSGD_TextureManager.h"
#include <math.h>

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

}

CEarth::~CEarth()
{

}


void CEarth::Update(float fElapsedTime)
{
	SetLifespan(GetLifespan() - fElapsedTime);
	if(GetLifespan() < 0)
	{
		////////////////////////////////////////////////////////////////
		//TODO: Carona_ObjectManager->GetInstance()->RemoveObject(this);
		////////////////////////////////////////////////////////////////
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
	if(m_fTimeTillRotate >0)
	{
		SetPosX(GetPosX()+GetVelX() * fElapsedTime);
		SetPosY(GetPosY()+GetVelY() * fElapsedTime);
		m_fTimeTillRotate -= fElapsedTime;
	}
	else
	{
		
		Vector2d spin;
		spin.fX = 0;
		spin.fY = 100.0f * fElapsedTime;

		Vector2d rotated = Rotate(spin ,m_fRotate);


		SetVelY(GetVelY() - rotated.fY);
		SetVelX(GetVelX() - rotated.fX);
	}
		
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
		////////////////////////////////////////////////////////////////
		//TODO: Carona_ObjectManager->GetInstance()->RemoveObject(this);
		////////////////////////////////////////////////////////////////

		/* 
		if vel == + push them right 
		else push them left by PushBack
		*/
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