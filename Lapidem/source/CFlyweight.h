#pragma once

class CFlyweight
{
private:
	float m_fVelX;
	float m_fVelY;
	float m_fAge;
	float m_fAccelX;
	float m_fAccelY;

	int m_nWidth;
	int m_nHeight;
	int m_nImageID;
	int m_nAlpha;
	int m_nRed;
	int m_nGreen;
	int m_nBlue;

public:
	CFlyweight( )
	{
		SetVelX( 0 );
		SetVelY( 0 );
		SetAccelX( 0 );
		SetAccelY( 0 );
		SetAge( 0 );

		SetWidth( 0 );
		SetHeight( 0 );
		SetImageID( 0 );
		SetAlpha( 0 );
		SetRed( 0 );
		SetGreen( 0 );
		SetBlue( 0 );
	}

	virtual ~CFlyweight( )         { /* DO NOTHING */   }

	float GetVelX( )     const     { return m_fVelX;    }
	float GetVelY( )     const     { return m_fVelY;    }
	float GetAccelX( )   const     { return m_fAccelX;  }
	float GetAccelY( )   const     { return m_fAccelY;  }
	float GetAge( )      const     { return m_fAge;     }

	int GetWidth( )      const     { return m_nWidth;   }
	int GetHeight( )     const     { return m_nHeight;  }
	int GetImageID( )    const     { return m_nImageID; }
	int GetAlpha( )      const     { return m_nAlpha;   }
	int GetRed( )        const     { return m_nRed;     }
	int GetGreen( )      const     { return m_nGreen;   }
	int GetBlue( )       const     { return m_nBlue;    }

	void SetVelX( float _f )       { m_fVelX      = _f; }
	void SetVelY( float _f )       { m_fVelY      = _f; }
	void SetAccelX( float _f )     { m_fAccelX    = _f; }
	void SetAccelY( float _f )     { m_fAccelY    = _f; }
	void SetAge( float _f )        { m_fAge       = _f; }

	void SetWidth( int _i )        { m_nWidth     = _i; }
	void SetHeight( int _i )       { m_nHeight    = _i; }
	void SetImageID( int _i )      { m_nImageID   = _i; }
	void SetAlpha( int _i )        { m_nAlpha     = _i; }
	void SetRed( int _i )          { m_nRed       = _i; }
	void SetGreen( int _i )        { m_nGreen     = _i; }
	void SetBlue( int _i )         { m_nBlue      = _i; }
};