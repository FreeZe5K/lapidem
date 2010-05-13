#include "CIceSpear.h"
#include "CCamera.h"
#include "Wrappers/CSGD_TextureManager.h"


CIceSpear::CIceSpear()
{
	this->SetType(OBJ_SPELL);
	this->SetElement(OBJ_ICE);
}

CIceSpear::~CIceSpear()
{
}

void CIceSpear::HandleCollision(CBase* pObject)
{
}

void CIceSpear::Update(float fElapsedTime)
{
	CSpell::Update(fElapsedTime);

	if(this->GetLifespan() <= 0.0f)
		this->SetActive(false);
}

void CIceSpear::Render()
{
	CCamera* pCamera = CCamera::GetCamera();
	CSGD_TextureManager::GetInstance()->Draw(this->GetImage(), (int)(this->GetPosX() - pCamera->GetXOffset()), (int)(this->GetPosY() - pCamera->GetYOffset()));
}