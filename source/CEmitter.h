#pragma once

#include "Lapidem_Math.h"
#include "CParticle.h"
#include <vector>
#include <string>
#include <ctime>
using std::string;
using std::vector;

class CEmitter
{
private: 
	int     m_nImageID;
	int     m_nNumOfParticles;
	int     m_nStartAlpha;
	int     m_nStartRed;
	int     m_nStartGreen;
	int     m_nStartBlue;
	int     m_nEndAlpha;
	int     m_nEndRed;
	int     m_nEndGreen;
	int     m_nEndBlue;
	int     m_nWidth;
	int     m_nHeight;
	int     m_nRadius;
	int     m_nSourceBlend;
	int     m_nDestinationBlend;

	f32     m_fPosX;
	f32     m_fPosY;
	f32     m_fGravityPosX;
	f32     m_fGravityPosY;
	f32     m_fSpreadAngle;
	f32     m_fSpreadDirection;
	f32     m_fParticleVelX;
	f32     m_fParticleVelY;
	f32     m_fVelX;
	f32     m_fVelY;
	f32     m_fGravityVelX;
	f32     m_fGravityVelY;
	f32     m_fAccX;
	f32     m_fAccY;
	f32     m_fStartScale;
	f32     m_fEndScale;
	f32     m_fSpawnRate;
	f32     m_fLifeSpan;
	f32     m_fTimeSinceLastSpawn;

	bool    m_bLooping;
	bool    m_bShape;
	bool    m_bGravity;

	string  m_szFileName;

	vector<CParticle*> m_vLiveParticles;
	vector<CParticle*> m_vDeadParticles;

public:
	CEmitter( );
	~CEmitter( );

	int GetImageID( )                         { return m_nImageID;             }
	int GetNumberOfParticles( )               { return m_nNumOfParticles;      }
	int GetWidth( )                           { return m_nWidth;               }
	int GetHeight( )                          { return m_nHeight;              }
	int GetRadius( )                          { return m_nRadius;              }
	int GetSourceBlend( )                     { return m_nSourceBlend;         }
	int GetDestinationBlend( )                { return m_nDestinationBlend;    }
	int GetStartAlpha( )                      { return m_nStartAlpha;          }
	int GetStartRed( )                        { return m_nStartRed;            }
	int GetStartGreen( )                      { return m_nStartGreen;          }
	int GetStartBlue( )                       { return m_nStartBlue;           }
	int GetEndAlpha( )                        { return m_nEndAlpha;            }
	int GetEndRed( )                          { return m_nEndRed;              }
	int GetEndGreen( )                        { return m_nEndGreen;            }
	int GetEndBlue( )                         { return m_nEndBlue;             }

	float GetSpreadAngle( )                   { return m_fSpreadAngle;         }
	float GetSpreadDirection( )               { return m_fSpreadDirection;     }
	float GetPosX( )                          { return m_fPosX;                }
	float GetPosY( )                          { return m_fPosY;                }
	float GetGravityPosX( )                   { return m_fGravityPosX;         }
	float GetGravityPosY( )                   { return m_fGravityPosY;         }
	float GetGravityVelX( )                   { return m_fGravityVelX;         }
	float GetGravityVelY( )                   { return m_fGravityVelY;         }
	float GetParticleVelX( )                  { return m_fParticleVelX;        }
	float GetParticleVelY( )                  { return m_fParticleVelY;        }
	float GetVelX( )                          { return m_fVelX;                }
	float GetVelY( )                          { return m_fVelY;                }
	float GetAccX( )                          { return m_fAccX;                }
	float GetAccY( )                          { return m_fAccY;                }
	float GetStartScale( )                    { return m_fStartScale;          }
	float GetEndScale( )                      { return m_fEndScale;            }
	float GetSpawnRate( )                     { return m_fSpawnRate;           }
	float GetLifeSpan( )                      { return m_fLifeSpan;            }

	bool GetGravity( )                        { return m_bGravity;             }
	bool GetLooping( )                        { return m_bLooping;             }
	bool GetShape( )                          { return m_bShape;               }

	string GetFileName( )                     { return m_szFileName;           }

	vector<CParticle*> GetLiveParticles( )    { return m_vLiveParticles;       }
	vector<CParticle*> GetDeadParticles( )    { return m_vDeadParticles;       }

	void SetImageID( int _i )                 { m_nImageID              = _i;  }
    void SetNumberOfParticles( int _i )       { m_nNumOfParticles       = _i;  }
    void SetWidth( int _i )                   { m_nWidth                = _i;  }
    void SetHeight( int _i )                  { m_nHeight               = _i;  }
    void SetRadius( int _i )                  { m_nRadius               = _i;  }
    void SetSourceBlend( int _i )             { m_nSourceBlend          = _i;  }
    void SetDestinationBlend( int _i )        { m_nDestinationBlend     = _i;  }
    void SetStartAlpha( int _i )              { m_nStartAlpha           = _i;  }
    void SetStartRed( int _i )                { m_nStartRed             = _i;  }
    void SetStartGreen( int _i )              { m_nStartGreen           = _i;  }
    void SetStartBlue( int _i )               { m_nStartBlue            = _i;  }
    void SetEndAlpha( int _i )                { m_nEndAlpha             = _i;  }
    void SetEndRed( int _i )                  { m_nEndRed               = _i;  }
    void SetEndGreen( int _i )                { m_nEndGreen             = _i;  }
    void SetEndBlue( int _i )                 { m_nEndBlue              = _i;  }

    void SetSpreadAngle( float _f )           { m_fSpreadAngle          = _f;  }
    void SetSpreadDirection( float _f )       { m_fSpreadDirection      = _f;  }
    void SetPosX( float _f )                  { m_fPosX                 = _f;  }
    void SetPosY( float _f )                  { m_fPosY                 = _f;  }
    void SetGravityPosX( float _f )           { m_fGravityPosX          = _f;  }
    void SetGravityPosY( float _f )           { m_fGravityPosY          = _f;  }
    void SetGravityVelX( float _f )           { m_fGravityVelX          = _f;  }
    void SetGravityVelY( float _f )           { m_fGravityVelY          = _f;  }
    void SetParticleVelX( float _f )          { m_fParticleVelX         = _f;  }
    void SetParticleVelY( float _f )          { m_fParticleVelY         = _f;  }
    void SetVelX( float _f )                  { m_fVelX                 = _f;  }
    void SetVelY( float _f )                  { m_fVelY                 = _f;  }
    void SetAccX( float _f )                  { m_fAccX                 = _f;  }
    void SetAccY( float _f )                  { m_fAccY                 = _f;  }
    void SetStartScale( float _f )            { m_fStartScale           = _f;  }
    void SetEndScale( float _f )              { m_fEndScale             = _f;  }
    void SetSpawnRate( float _f )             { m_fSpawnRate            = _f;  }
    void SetLifeSpan( float _f )              { m_fLifeSpan             = _f;  }

    void SetGravity( bool _b )                {  m_bGravity             = _b;  }
    void SetLooping( bool _b )                {  m_bLooping             = _b;  }
    void SetShape( bool _b )                  {  m_bShape               = _b;  }

    void SetLiveParticle( vector<CParticle*> _v )   { m_vLiveParticles  = _v;  }
    void SetDeadParticle( vector<CParticle*> _v )   { m_vDeadParticles  = _v;  }

	void Update( float );
	void Render( );
	void AddParticle( CParticle* );
	void UpdateParticle( CParticle*, float );
	void ClearParticles( );
};