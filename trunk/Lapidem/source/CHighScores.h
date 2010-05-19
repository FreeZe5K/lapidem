#pragma once

struct Score
{
	char name[4];
	int	 score;
};

#define MAX_SCORES 10

class CHighScores
{
	Score m_Scores[MAX_SCORES];

public:
	CHighScores();

	void Load(char* szFilename );
	void Save(char* szFilename );

	bool Try( int score);
	void Add( int score, char* name );


	void Render();

};