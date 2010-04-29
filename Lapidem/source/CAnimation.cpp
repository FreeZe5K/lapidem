#include "CAnimation.h"



CAnimation::CAnimation()
{
	m_fTimeWaited = 0.0f;
	m_nFrame=0;
}
CAnimation::~CAnimation()
{

}

void CAnimation::Update(float fElapsedTime)
{
	m_fTimeWaited += fElapsedTime;
	if(m_fTimeWaited > m_vFrames[m_nFrame]->WaitTime)
	{
		m_nFrame++;
		m_fTimeWaited =0.0f;
		if(m_nFrame == m_vFrames.size() && m_bIsLooping)
		{
			m_nFrame =0;
		}
		else if(!m_bIsLooping)
		{
			m_nFrame = 0;
			m_bIsPlaying = false;
		}
	}
}
