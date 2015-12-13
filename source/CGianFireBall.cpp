#include "CGiantFireBall.h"
#include "stdheaders.h"

#include "CEmitter.h"


CGiantFireBall::CGiantFireBall()
{
	m_fRadiusIncrease = 0.0f;
	this->SetType(OBJ_SPELL);
	this->SetElement(OBJ_FIRE);
}

CGiantFireBall::~CGiantFireBall()
{
	CParticleManager::GetInstance()->RemoveEmitter(m_pEmitter);
}

void CGiantFireBall::Update(float fElapsedTime)
{
	CSpell::Update(fElapsedTime);

	m_fRadiusIncrease = m_fRadiusIncrease + fElapsedTime;

	float fPosOffset = m_fRadiusIncrease * fElapsedTime;

	//this->SetPosX(this->GetPosX() - fPosOffset);
	//this->SetPosY(this->GetPosY() - fPosOffset);

	this->SetWidth(this->GetWidth() + int( m_fRadiusIncrease * fElapsedTime ) );
	this->SetHeight(this->GetHeight() + int( m_fRadiusIncrease * fElapsedTime ) );

	if( m_pEmitter )
	{
		m_pEmitter->SetPosX( GetPosX( ) );
		m_pEmitter->SetPosY( GetPosY( ) );
		m_pEmitter->SetWidth(this->GetWidth());
		m_pEmitter->SetHeight(this->GetHeight());
		m_pEmitter->SetRadius(this->GetHeight()>>1);
		//m_pEmitter->SetSpawnRate(m_pEmitter->GetSpawnRate() + (int)fPosOffset);
	}


	// - - - - - - - -
	// REPLACE
	// - - - - - - - - - - - - - - 
	//CEmitter* pEmitter = this->GetEmitter();
	//pEmitter->SetSpreadX(pEmitter->GetSpreadX() + m_fRadiusIncrease * fElapsedTime);
	//pEmitter->SetSpreadY(pEmitter->GetSpreadY() + m_fRadiusIncrease * fElapsedTime);

	if(this->GetLifespan() <= 0.0f)
		this->SetActive(false);
}

void CGiantFireBall::Render()
{
	CCamera* pCamera = CCamera::GetCamera();
	if(this->GetImage() != -1)
		CSGD_TextureManager::GetInstance()->Draw(this->GetImage(), (int)(this->GetPosX() - pCamera->GetXOffset()), (int)(this->GetPosY() - pCamera->GetYOffset()));
}