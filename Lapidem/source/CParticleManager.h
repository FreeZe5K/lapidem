#pragma once
#include "Lapidem_Math.h"
#include "CEmitter.h"
#include "Wrappers/CSGD_Direct3D.h"
#include "Wrappers/CSGD_TextureManager.h"
#include <vector>
using namespace std;

class CParticleManager
{
private: 
	vector<CEmitter*>     m_vEmitters;

	CSGD_Direct3D         *m_pD3D;
	CSGD_TextureManager   *m_pTM;

	CParticleManager( );
	~CParticleManager( );
	CParticleManager( CParticleManager& );
	CParticleManager& operator=( const CParticleManager& );

public:
	static CParticleManager *GetInstance( );

	void UnloadAll( );
	void AddEmitter( CEmitter* );
	void Render( );
	void Update( float );
	u32 GetNumEmitters( );
};

class CEmitterFactory
{
private:
	CEmitterFactory( );
	~CEmitterFactory( );
	CEmitterFactory( const CEmitterFactory& );
	CEmitterFactory& operator=( const CEmitterFactory& );

	CSGD_TextureManager *m_pTM;

	int Load( const char*, const char* szName = NULL );
	vector<CEmitter*> m_vEmitters;

public:
	void Initialize( );
	static CEmitterFactory *GetInstance( );
	CEmitter *CreateEmitter( const char* );
	void UnloadAll( );
};