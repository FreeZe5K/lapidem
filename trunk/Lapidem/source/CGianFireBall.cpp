#include "CGiantFireBall.h"
#include "CEmitter.h"
#include "CCamera.h"
#include "Wrappers/CSGD_TextureManager.h"

CGiantFireBall::CGiantFireBall()
{
	m_fRadiusIncrease = 16.0f;
	this->SetType(OBJ_SPELL);
	this->SetElement(OBJ_FIRE);
}

CGiantFireBall::~CGiantFireBall()
{
}

void CGiantFireBall::Update(float fElapsedTime)
{
	CSpell::Update(fElapsedTime);

	CEmitter* pEmitter = this->GetEmitter();
	pEmitter->SetSpreadX(pEmitter->GetSpreadX() + m_fRadiusIncrease * fElapsedTime);
	pEmitter->SetSpreadY(pEmitter->GetSpreadY() + m_fRadiusIncrease * fElapsedTime);

	if(this->GetLifespan() <= 0.0f)
		this->SetActive(false);
}

void CGiantFireBall::Render()
{
	CCamera* pCamera = CCamera::GetCamera();
	CSGD_TextureManager::GetInstance()->Draw(this->GetImage(), (int)(this->GetPosX() - pCamera->GetXOffset()), (int)(this->GetPosY() - pCamera->GetYOffset()));
}