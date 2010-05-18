#include "CPickups.h"
#include "stdheaders.h"

void CPickup::Update(float fElapsedTime)
{
	m_fLifeTimer -= fElapsedTime;

	if(m_fLifeTimer <= 0)
		SetActive(false);
}
void CPickup::Render()
{
	CSGD_TextureManager::GetInstance()->Draw(GetImage(),  (int)(GetPosX() - CCamera::GetCamera()->GetXOffset()),
													      (int)(GetPosY() - CCamera::GetCamera()->GetYOffset()),
														  1.f, 1.f, NULL, 0, 0, 0,
														  D3DCOLOR_ARGB((int)(m_fLifeTimer / 15.f * 255), 255, 255, 255));

}
