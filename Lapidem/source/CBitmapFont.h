//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CBitmapFont.h"
//
//  Author Name :   Mike Hatter
//
//  Purpose     :   To draw bitmaps fonts.
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "Wrappers/CSGD_TextureManager.h"

class CBitmapFont
{
private:
	int  m_nImageID;
	int  m_nCharWidth;
	int  m_nCharheight;
	int  m_nNumColumns;

	char m_cStartChar;

	RECT CellAlgorithm( int );

public:
	CBitmapFont();
	~CBitmapFont();

	void Load( const char* );
	void Draw( const char*, int, int, float, DWORD );
};