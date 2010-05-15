#pragma once

#include "Lapidem_Math.h"

class CParticle
{
private:
	float m_fPosX;
	float m_fPosY;
	float m_fVelX;
	float m_fVelY;
	float m_fAccX;
	float m_fAccY;
	float m_fCurScale;
	float m_fAge;

	int   m_nAlpha;
	int   m_nRed;
	int   m_nGreen;
	int   m_nBlue;

public:
	CParticle( )                   { /* NOTHING HERE */     }
	~CParticle( )                  { /* NOTHING HERE */     }

	float GetPosX( )               { return m_fPosX;        }
	float GetPosY( )               { return m_fPosY;        }
	float GetVelX( )               { return m_fVelX;        }
	float GetVelY( )               { return m_fVelY;        }
	float GetAccX( )               { return m_fAccX;        }
	float GetAccY( )               { return m_fAccY;        }
	float GetAge( )                { return m_fAge;         }
	float GetCurrScale( )          { return m_fCurScale;    }
	int GetAlpha( )                { return m_nAlpha;       }
	int GetRed( )                  { return m_nRed;         }
	int GetGreen( )                { return m_nGreen;       }
	int GetBlue( )                 { return m_nBlue;        }

	void SetPosX( float _f )       { m_fPosX       = _f;    }
	void SetPosY( float _f )       { m_fPosY       = _f;    }
	void SetVelX( float _f )       { m_fVelX       = _f;    }
	void SetVelY( float _f )       { m_fVelY       = _f;    }
	void SetAccX( float _f )       { m_fAccX       = _f;    }
	void SetAccY( float _f )       { m_fAccY       = _f;    }
	void SetAge( float _f )        { m_fAge        = _f;    }
	void SetCurrScale( float _f )  { m_fCurScale   = _f;    }

	void SetAlpha( int _i )        { m_nAlpha      = _i;    }
	void SetRed( int _i )          { m_nRed        = _i;    }
	void SetGreen( int _i )        { m_nGreen      = _i;    }
	void SetBlue( int _i )         { m_nBlue       = _i;    }

	void Update( float );
	void Render( int );
};