#include "CLevelSwitch.h"
#include "stdheaders.h"

CLevelSwitch::CLevelSwitch()
{
	m_bIsOn = false;
	m_nSwitchModeID[0] = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/Switch1.png", D3DCOLOR_XRGB(255, 255, 255));
	m_nSwitchModeID[1] = CSGD_TextureManager::GetInstance()->LoadTexture("resource/graphics/Switch2.png", D3DCOLOR_XRGB(255, 255, 255));
}

CLevelSwitch::~CLevelSwitch()
{
	for(int i = 0; i < 2; ++i)
		CSGD_TextureManager::GetInstance()->UnloadTexture(m_nSwitchModeID[i]);
}

void CLevelSwitch::HandleCollision(float fElapsedTime, CBase* pBase)
{
	if(pBase->GetType() == OBJ_PLAYER)
		m_bIsOn = true;
}

void CLevelSwitch::Render()
{
	CCamera* theCamera = CCamera::GetCamera();
	CSGD_TextureManager::GetInstance()->Draw(m_nSwitchModeID[m_bIsOn], (int)(GetPosX() - theCamera->GetXOffset()), (int)(GetPosY() - theCamera->GetYOffset()));
}

void CLevelSwitch::Reset()
{
	m_bIsOn = false;
}

