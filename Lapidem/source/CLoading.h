#pragma once

#include "IGameState.h"
#include "CGame.h"

class CLoading : public IGameState
{
private:
	CSGD_TextureManager     *m_pTM;

	int                     m_nBackgroundID;
	int                     m_nProgressID;
	int                     m_nCurrentProgress;
	int                     m_nProposedProgress;

	CLoading( )  { /* DO NOTHING */ }
	~CLoading( ) { /* DO NOTHING */ }
	CLoading( const CLoading& );
	CLoading& operator=( const CLoading& );

public:
	static CLoading* GetInstance( );

	void Enter( );
	bool Input( );
	void Update( float );
	void Render( );
	void Exit( );

	void SetProgress( int );
};