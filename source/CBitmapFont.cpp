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
	m_nImageID      = -1;
	m_nNumColumns   = 16;
	m_cStartChar    = ' ';
}

CBitmapFont::~CBitmapFont( )
{
}

RECT CBitmapFont::CellAlgorithm( int nID )
{
	RECT rcell;

	rcell.left      = ( nID % m_nNumColumns ) * m_nCharWidth;
	rcell.top       = ( nID / m_nNumColumns ) * m_nCharheight;
	rcell.right     = rcell.left + m_nCharWidth;
	rcell.bottom    = rcell.top + m_nCharheight;

	return rcell;
}

bool CBitmapFont::Load( const char* szFileName, const char* szSizes )
{
	ifstream fin( szSizes, std::ios_base::in );

	int nTempData;

	while( !fin.eof( ) )
	{
		nTempData = 0;
		fin >> nTempData;
		m_vChars.push_back( nTempData );
		fin.ignore( INT_MAX, '\n' );
	} fin.close( );

	m_nCharWidth   = 32;
	m_nCharheight  = 32;
	m_nNumColumns  = 16;

	m_nImageID = CSGD_TextureManager::GetInstance( )->LoadTexture( 
		szFileName, D3DCOLOR_XRGB( 255, 0, 255 ) );

	return true;
}

void CBitmapFont::EachLetter( int id, int nPosX, int nPosY, float fScale, float fRot, DWORD dwColor )
{
	RECT rLetter = CellAlgorithm( id );
	CSGD_TextureManager::GetInstance()->Draw( m_nImageID, nPosX, nPosY, fScale, fScale, &rLetter, 0.0f, 0.0f, fRot, dwColor );
}

void CBitmapFont::Draw( const char* szTextToPrint, int nPosX, int nPosY, float fScale, DWORD dwColor, int nSpace )
{
	int len = strlen( szTextToPrint );

	for( int i = 0; i < len; i++ )
	{
		int id = szTextToPrint[i] - m_cStartChar;

		if( id == 0 || id == 64 ) nPosX = nPosX + 16;
		else
		{
			if( char( id + 32 ) == 'I' )
				nPosX = nPosX - int( 4 * fScale );
			else if( char( id + 32 ) == 'L' )
				nPosX = nPosX - int( 2 * fScale );
			else if( char( id + 32 ) == 'R' || 
				char( id + 32 ) == 'N' )
				nPosX = nPosX - int( 3 * fScale );
			else if( char( id + 32 ) == 'A' || 
				char( id + 32 ) == 'S' || 
				char( id + 32 ) == 'Y' || 
				char( id + 32 ) == 'G' || 
				char( id + 32 ) == 'B' )
				nPosX = nPosX - ( int( 2 * fScale ) );
			else if( char( id + 32 ) == 'U' || 
				char( id + 32 ) == 'X' ||
				char( id + 32 ) == 'P' )
				nPosX = nPosX - ( int( 2 * fScale ) );
			else if( char( id + 32 ) == 'm' )
				nPosX = nPosX + ( int( 3 * fScale ) );

			if( nSpace > 0 ) 
				EachLetter( id,( nPosX - nSpace ) + 
				( i * nSpace ), nPosY, fScale, 0.0f, dwColor );
			else if( nSpace == 0 ) 
				EachLetter( id, nPosX, nPosY, fScale, 0.0f, dwColor );

			nPosX = nPosX + int( m_vChars[id] * fScale );
		}
	}
}