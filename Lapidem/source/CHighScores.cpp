#include "CHighScores.h"
#include "CGame.h"
#include <string.h>

#include <fstream>
using namespace std;


CHighScores::CHighScores()
{
	
	for( int i = 0; i < MAX_SCORES; ++i )
	{
		sprintf_s(m_Scores[i].name, "AAA");
		m_Scores[i].score = 0;

	}

}


void CHighScores::Load(char* szFilename )
{
	ifstream in( szFilename, ios::in|ios::binary );

	if( in.is_open() )
	{
		in.read( (char*)&m_Scores, sizeof(Score)*MAX_SCORES );

	}else
	{
		for( int i = 0; i < MAX_SCORES; ++i )
	{
		sprintf_s(m_Scores[i].name, "AAA");
		m_Scores[i].score = 0;

	}

	}

}
void CHighScores::Save(char* szFilename )
{
	ofstream out( szFilename, ios::out| ios::binary );

	if( out.is_open() )
	{
		out.write( (char*)&m_Scores, sizeof(Score)*MAX_SCORES );
	}

}

bool CHighScores::Try( int nScore)
{
	for( int i = 0; i < MAX_SCORES; ++i )
	{
		if( nScore >= m_Scores[i].score )
			return true;
	}

	return false;

}

void CHighScores::Add( int nScore, char* szName )
{
	int i = 0;

	for(;i<MAX_SCORES;++i)
		if( nScore >= m_Scores[i].score )
			break;

	for( int j = MAX_SCORES-1; j> i; --j )
	{
		strcpy_s( m_Scores[j].name, sizeof(char)*4, m_Scores[j-1].name );
		m_Scores[j].score = m_Scores[j-1].score;
	}

	strcpy_s( m_Scores[i].name, sizeof(char)*4, szName );
	m_Scores[i].score = nScore;




}


void CHighScores::Render()
{
	
		int _ScoreHeight( 100 );
		char _ScoreBuffer[16];

	for( int i = 0; i < MAX_SCORES; ++i )
		{
			CGame::GetInstance( )->GetFont( )->Draw( m_Scores[i].name , 30, 
				_ScoreHeight, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			sprintf_s( _ScoreBuffer, "%i", m_Scores[i].score  );
			CGame::GetInstance( )->GetFont( )->Draw( _ScoreBuffer, 150, 
				_ScoreHeight, 0.8f, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

			_ScoreHeight = _ScoreHeight + 30;
		}

}