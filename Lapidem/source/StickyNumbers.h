#pragma once
#include "CBase.h"
#include "CBitmapFont.h"

class StickyNumbers : public CBase
{
	float m_fTimer;
	float m_fTimeStart;
	int m_nAlpha;
	char * m_szText;

public:
	StickyNumbers() { m_nAlpha = 255; m_szText = 0; }
	~StickyNumbers() { delete[] m_szText; }
	void Update(float);
	void Render();
	void SetTimer(float timer) {m_fTimeStart = m_fTimer = timer;}
	void SetText(const char * text);// { m_szText = text; }
};


