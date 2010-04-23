#pragma once

#include "CFlyweight.h"
#include "Wrappers/CSGD_TextureManager.h"

class CParticle : public CFlyweight
{
private:
	CSGD_TextureManager     *m_pTM;

	float     m_fCurrentPosX;
	float     m_fCurrentPosY;
	float     m_fVelXStart;
	float     m_fVelYStart;
	float     m_fVelXEnd;
	float     m_fVelYEnd;
	float     m_fScaleStart;
	float     m_fScaleEnd;
	float     m_fLifeSpan;
	float     m_fScaleOut;

	int       m_nStartAlpha;
	int       m_nStartRed;
	int       m_nStartGreen;
	int       m_nStartBlue;
	int       m_nEndAlpha;
	int       m_nEndRed;
	int       m_nEndGreen;
	int       m_nEndBlue;

public:
	CParticle( );
	~CParticle( );

	float GetPosX( )       const    { return m_fCurrentPosX;    }
	float GetPosY( )       const    { return m_fCurrentPosY;    }
	float GetStartVelX( )  const    { return m_fVelXStart;      }
	float GetStartVelY( )  const    { return m_fVelYStart;      }
	float GetEndVelX( )    const    { return m_fVelXEnd;        }
	float GetEndVelY( )    const    { return m_fVelYEnd;        }
	float GetLifeSpan( )   const    { return m_fLifeSpan;       }
	float GetStartScale( ) const    { return m_fScaleStart;     }
	float GetEndScale( )   const    { return m_fScaleEnd;       }

	int GetStartAlpha( )   const    { return m_nStartAlpha;     }
	int GetStartRed( )     const    { return m_nStartRed;       }
	int GetStartGreen( )   const    { return m_nStartGreen;     }
	int GetStartBlue( )    const    { return m_nStartBlue;      }
	int GetEndAlpha( )     const    { return m_nEndAlpha;       }
	int GetEndRed( )       const    { return m_nEndRed;         }
	int GetEndGreen( )     const    { return m_nEndGreen;       }
	int GetEndBlue( )      const    { return m_nEndBlue;        }

	void SetPosX( float _f )        { m_fCurrentPosX      = _f; }
	void SetPosY( float _f )        { m_fCurrentPosY      = _f; }
	void SetStartVelX( float _f )   { m_fVelXStart        = _f; }
	void SetEndVelX( float _f )     { m_fVelXEnd          = _f; }
	void SetStartVelY( float _f )   { m_fVelYStart        = _f; }
	void SetEndVelY( float _f )     { m_fVelYEnd          = _f; }
	void SetLifeSpan( float _f )    { m_fLifeSpan         = _f; }
	void SetStartScale( float _f )  { m_fScaleStart       = _f; }
	void SetEndScale( float _f )    { m_fScaleEnd         = _f; }

	void SetStartAlpha( int _i )    { m_nStartAlpha       = _i; }
	void SetStartRed( int _i )      { m_nStartRed         = _i; }
	void SetStartGreen( int _i )    { m_nStartGreen       = _i; }
	void SetStartBlue( int _i )     { m_nStartBlue        = _i; }
	void SetEndAlpha( int _i )      { m_nEndAlpha         = _i; }
	void SetEndRed( int _i )        { m_nEndRed           = _i; }
	void SetEndGreen( int _i )      { m_nEndGreen         = _i; }
	void SetEndBlue( int _i )       { m_nEndBlue          = _i; }

	void Update( float );
	void Render( );
};