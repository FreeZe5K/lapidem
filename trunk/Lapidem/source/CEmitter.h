#pragma once

#include "CParticle.h"
#include <vector>
using namespace std;

class CParticle;

struct Vector2D
{
	float _x;
	float _y;
};

class CEmitter
{
private:
	vector<CParticle*> m_vLiveParticles;
	vector<CParticle*> m_vDeadParticles;

	CParticle          *m_pParticle;

	float              m_fPosX;
	float              m_fPosY;
	float              m_fVelX;
	float              m_fVelY;
	float              m_fSpreadX;
	float              m_fSpreadY;
	float              m_fLifeSpan;
	float              m_fEmitTimer;
	float              m_fSpawnTime;
	float              m_fAngle;

	int                m_nNumParticles;
	int                m_nSourceBlend;
	int                m_nDestinationBlend;
	int                m_nParticlesMade;
	int                m_nEmitRate;

	bool               m_bLooping;

	char               *m_szName;

public:
	CEmitter( );
	~CEmitter( );
	CEmitter& operator=( const CEmitter& );

	void Render( );
	void Update( float );
	void AddParticle( CParticle* );
	void ClearParticles( );

	Vector2D VectorRotate( Vector2D, float );

	float DegreeToRadian( float );

	bool EmitterDone( );

	vector<CParticle*> GetLiveParticles( ) const { return m_vLiveParticles; }
	vector<CParticle*> GetDeadParticles( ) const { return m_vDeadParticles; }

	CParticle *GetParticle( )   const      { return m_pParticle;          }
	
	float GetPosX( )            const      { return m_fPosX;              }
	float GetPosY( )            const      { return m_fPosY;              }
	float GetVelX( )            const      { return m_fVelX;              }
	float GetVelY( )            const      { return m_fVelY;              }
	float GetSpreadX( )         const      { return m_fSpreadX;           }
	float GetSpreadY( )         const      { return m_fSpreadY;           }
	float GetLifeSpan( )        const      { return m_fLifeSpan;          }
	float GetEmitTimer( )       const      { return m_fEmitTimer;         }
	float GetSpawnTime( )       const      { return m_fSpawnTime;         }
	float GetAngle( )           const      { return m_fAngle;             }

	int GetNumParticles( )      const      { return m_nNumParticles;      }
	int GetSourceBlend( )       const      { return m_nSourceBlend;       }
	int GetDestinationBlend( )  const      { return m_nDestinationBlend;  }
	int GetEmitRate( )          const      { return m_nEmitRate;          }

	char* GetName( )            const      { return m_szName;             }

	bool GetLooping( )          const      { return m_bLooping;           }

	void SetPosX( float _f )               { m_fPosX              = _f;   }
	void SetPosY( float _f )               { m_fPosY              = _f;   }
	void SetVelX( float _f )               { m_fVelX              = _f;   }
	void SetVelY( float _f )               { m_fVelY              = _f;   }
	void SetSpreadX( float _f )            { m_fSpreadX           = _f;   }
	void SetSpreadY( float _f )            { m_fSpreadY           = _f;   }
	void SetLifeSpan( float _f)            { m_fLifeSpan          = _f;   }
	void SetEmitTimer( float _f )          { m_fEmitTimer         = _f;   }
	void SetSpawnTime( float _f )          { m_fSpawnTime         = _f;   }
	void SetAngle( float _f )              { m_fAngle             = _f;   }
	
	void SetSourceBlend( int _i )          { m_nSourceBlend       = _i;   }
	void SetDestinationBlend( int _i )     { m_nDestinationBlend  = _i;   }
	void SetNumParticles( int _i )         { m_nNumParticles      = _i;   }
	void SetEmitRate( int _i )             { m_nEmitRate          = _i;   }
	
	void SetLooping( bool _b )             { m_bLooping           = _b;   }

	void SetName( char* _sz )              { m_szName             = _sz;  }

	void UpdateParticlePos( int nPosX, int nPosY);
};