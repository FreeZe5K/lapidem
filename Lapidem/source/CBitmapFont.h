//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CBitmapFont.h"
//
//  Author Name :   Mike Hatter
//
//  Purpose     :   To draw bitmaps fonts.
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "Wrappers/CSGD_TextureManager.h"
#include <vector>
#include <fstream>
using std::vector;
using std::ios_base;
using std::ifstream;

class CBitmapFont
{
private:
	int  m_nImageID;
	int  m_nCharWidth;
	int  m_nCharheight;
	int  m_nNumColumns;
	int  m_nSpaceSize;

	char m_cStartChar;

	vector<int> m_vChars;

	RECT CellAlgorithm( int );

public:
	CBitmapFont();
	~CBitmapFont();

	bool Load( const char*, const char* );
	void EachLetter( int, int, int, float, float, DWORD );
	void Draw( const char*, int, int, float, DWORD, int nSpace = 0 );
};