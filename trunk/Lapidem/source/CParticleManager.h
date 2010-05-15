#pragma once

#include "Lapidem_Math.h"
#include "CEmitter.h"

class CParticleManager
{
private:
	vector<CEmitter*> m_vEmitters;

	CParticleManager( );
	CParticleManager( const CParticleManager& );
	CParticleManager &operator=( const CParticleManager& );
	~CParticleManager( );

public:
	static CParticleManager* GetInstance( );

	void Update( f32 );
	void Render( );
	void AddEmitter( CEmitter* );
	void RemoveEmitter( CEmitter* );
	CEmitter* LoadEmitter( const char* szFileName, f32 fPosX, 
		f32 fPosY, f32 fVelX = 0.0f, f32 fVelY = 0.0f, 
		f32 fAccX = 0.0f, f32 fAccY = 0.0f );
	void ClearAll( );
};