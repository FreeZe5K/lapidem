#include "CParticle.h"

CParticle::CParticle( )
{
	m_pTM = CSGD_TextureManager::GetInstance( );

	SetPosX( 0 );
	SetPosY( 0 );

	SetStartVelX( 0 );
	SetEndVelX( 0 );

	SetStartVelY( 0 );
	SetEndVelY( 0 );

	SetLifeSpan( 0 );
	SetEndScale( 0 );
	SetStartScale( 0 );

	SetStartAlpha( 0 );
	SetStartRed( 0 );
	SetStartGreen( 0 );
	SetStartBlue( 0 );

	SetEndAlpha( 0 );
	SetEndRed( 0 );
	SetEndGreen( 0 );
	SetEndBlue( 0 );

	m_fScaleOut = 0;
}

CParticle::~CParticle( )
{
}

void CParticle::Update( float fET )
{
	SetAge( GetAge( ) + fET );

	//if( GetAge( ) < m_fLifeSpan )
	//{
		float _a        = float( m_nEndAlpha - m_nStartAlpha );
		float _r        = float( m_nEndRed - m_nStartRed );
		float _g        = float( m_nEndGreen - m_nStartGreen );
		float _b        = float( m_nEndBlue - m_nStartBlue );

		m_fVelXStart    = m_fVelXStart + GetAccelX( ) * fET;
		m_fVelXEnd      = m_fVelXEnd + GetAccelX( ) * fET;
		m_fVelYStart    = m_fVelYStart + GetAccelY( ) * fET;
		m_fVelYEnd      = m_fVelYEnd + GetAccelY( ) * fET;

		SetAlpha(  int( float( _a * ( GetAge( ) / m_fLifeSpan ) ) + m_nStartAlpha ) );
		SetRed(    int( float( _r * ( GetAge( ) / m_fLifeSpan ) ) + m_nStartRed ) );
		SetGreen(  int( float( _g * ( GetAge( ) / m_fLifeSpan ) ) + m_nStartGreen ) );
		SetBlue(   int( float( _b * ( GetAge( ) / m_fLifeSpan ) ) + m_nStartBlue ) );

		SetVelX( ( ( m_fVelXEnd - m_fVelXStart ) * ( GetAge( ) / m_fLifeSpan ) ) + m_fVelXStart );
		SetVelY( ( ( m_fVelYEnd - m_fVelYStart ) * ( GetAge( ) / m_fLifeSpan ) ) + m_fVelYStart );

		m_fScaleOut     = ( ( m_fScaleEnd - m_fScaleStart ) * 
			( GetAge( ) / m_fLifeSpan ) ) + m_fScaleStart;
	//}

	m_fCurrentPosX      = m_fCurrentPosX + ( GetVelX( ) * fET );
	m_fCurrentPosY      = m_fCurrentPosY + ( GetVelY( ) * fET );
}

void CParticle::Render( )
{
	if( GetImageID( ) != -1 )
		m_pTM->Draw( 1, int( m_fCurrentPosX - 
		CCamera::GetCamera( )->GetXOffset( ) ), 
		int( m_fCurrentPosY - CCamera::GetCamera( )->GetYOffset( ) ),
		m_fScaleOut, m_fScaleOut, NULL, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB
		( 255, GetRed( ), GetGreen( ), GetBlue( ) ) );
}