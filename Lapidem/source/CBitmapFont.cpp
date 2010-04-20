//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CBitmapFont.cpp"
//
//  Author Name :   Mike Hatter
//
//  Purpose     :   To draw bitmaps fonts.
//////////////////////////////////////////////////////////////////////////
#include "CBitmapFont.h"

CBitmapFont::CBitmapFont( )
{
	m_cStartChar    = ' ';
	m_nNumColumns   = 16;
}

CBitmapFont::~CBitmapFont( )
{
}

RECT CBitmapFont::CellAlgorithm( int nID )
{
	RECT rcell;
	int nWidth      = 32;
	int nHeight     = 32;

	rcell.left      = ( nID % m_nNumColumns ) * nWidth;
	rcell.top       = ( nID / m_nNumColumns ) * nHeight;
	rcell.right     = rcell.left + nWidth;
	rcell.bottom    = rcell.top + nHeight;

	return rcell;
}

void CBitmapFont::Load( const char* szFileName )
{
	m_nImageID = CSGD_TextureManager::GetInstance( )->LoadTexture( 
		szFileName, D3DCOLOR_XRGB( 255, 0, 255 ) );
}

void CBitmapFont::Draw( const char* szTextToPrint, int nPosX, int nPosY, float fScale, DWORD dwColor )
{
	int len         = strlen( szTextToPrint );
	int nOriginalX  = nPosX;

	for ( int i = 0; i < len; i++ )
	{
		char ch         = szTextToPrint[i];

		if( ch == ' ' )
		{
			nPosX += ( int( 32 * fScale ) );
			continue;
		}
		else if( ch == '/n' )
		{
			nPosX = nOriginalX;
			nPosY = int( m_nCharheight * fScale );
			continue;
		}

		int id          = ch - m_cStartChar;
		RECT rLetter    = CellAlgorithm( id );

		CSGD_TextureManager::GetInstance( )->Draw( m_nImageID, nPosX, nPosY,
			fScale, fScale, &rLetter, 0.0f, 0.0f, 0.0f, dwColor );

		nPosX += ( int( 32 * fScale ) );
	}
}