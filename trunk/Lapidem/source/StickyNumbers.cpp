#include "StickyNumbers.h"
#include "stdheaders.h"

void StickyNumbers::Update(float fElapsedTime)
{
	m_fTimer -= fElapsedTime;
	if(m_fTimer <= 0)
	{
		SetActive(false);
		return;
	}

	CBase::Update(fElapsedTime);

	m_nAlpha = (int)((m_fTimer / m_fTimeStart) * 255);
}

void StickyNumbers::Render()
{
	CGame::GetInstance()->GetFont()->Draw(m_szText,  (int)(GetPosX() - CCamera::GetCamera()->GetXOffset()),
													 (int)(GetPosY() - CCamera::GetCamera()->GetYOffset()),
													 1.f, D3DCOLOR_ARGB(m_nAlpha, 255, 255, 255));
}

void StickyNumbers::SetText(const char *text)
{
	delete[] m_szText;
	int nLength = strlen(text);
	m_szText = new char[nLength + 1];
	strcpy_s(m_szText, nLength + 1, text);
}