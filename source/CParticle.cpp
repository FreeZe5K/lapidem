#include "CParticle.h"
#include "Wrappers/CSGD_TextureManager.h"
#include "CCamera.h"
void CParticle::Update( float fET )
{
	m_fAge   = m_fAge + fET;
	m_fVelX  = m_fVelX + ( m_fAccX * fET );
	m_fVelY  = m_fVelY + ( m_fAccY * fET );
	m_fPosX  = m_fPosX + ( m_fVelX * fET );
	m_fPosY  = m_fPosY + ( m_fVelY * fET );
}

void CParticle::Render( int _id )
{
	if(GetPosX() > 32 && GetPosY() > 32)
	{
		int m_nWidth( CSGD_TextureManager::GetInstance( )->GetTextureWidth( _id ) );
		int m_nHeight( CSGD_TextureManager::GetInstance( )->GetTextureHeight( _id ) );

		RECT rSection;
		SetRect( &rSection, 0, 0, m_nWidth, m_nHeight );

		CSGD_TextureManager::GetInstance( )->Draw( _id, 
			int( ( m_fPosX - CCamera::GetCamera( )->GetXOffset( ) ) /** m_fCurScale*/ ),
			int( ( m_fPosY - CCamera::GetCamera( )->GetYOffset( ) ) /** m_fCurScale*/ ),
			m_fCurScale, m_fCurScale, &rSection, 0.0f, 0.0f, 0.0f, 
			ARGB( m_nAlpha, m_nRed, m_nGreen, m_nBlue ) );
	}
}