#include "CAnimation.h"



CAnimation::CAnimation()
{
	m_fTimeWaited = 0.0f;
}
CAnimation::~CAnimation()
{

}

void CAnimation::Update(float fElapsedTime)
{
	m_fTimeWaited += fElapsedTime;
	if(m_fTimeWaited > m_dTimeToWait)
	{
		m_nFrame++;
		if(m_nFrame == m_vFrames.size())
		{
			m_nFrame =0;
		}
	}
}
