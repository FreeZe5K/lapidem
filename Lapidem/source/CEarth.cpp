#include "CEarth.h"
#include "Wrappers/CSGD_TextureManager.h"
#include "CCamera.h" 
#include <math.h>

struct Vector2d
{ float fX, fY; };

Vector2d Rotate( Vector2d vRotateme, float fRadian )
{
	vRotateme.fY = vRotateme.fY * -1.0f;

	Vector2d HahaRotated;
	HahaRotated.fX = ( cos(fRadian ) * vRotateme.fX ) + ( sin( fRadian ) * vRotateme.fY );
	HahaRotated.fY = ( -sin(fRadian ) * vRotateme.fX ) + ( cos( fRadian ) * vRotateme.fY );

	return HahaRotated;
}
CEarth::CEarth() : CSpell()
{
	m_fRotate            = float( PI / 4.0f );
	m_fDisplay           = m_fRotate;
	m_fTimeTillRotate    = 1.0f;
	SetImage( CSGD_TextureManager::GetInstance( )->LoadTexture( "resource/graphics/LapidemEarth.bmp", D3DCOLOR_XRGB( 0, 0, 0 ) ) );
}

CEarth::~CEarth()
{
	if( GetImage( ) >= 0 )
		CSGD_TextureManager::GetInstance( )->UnloadTexture( GetImage( ) );
}

void CEarth::Update( float fElapsedTime )
{
	SetLifespan( GetLifespan( ) - fElapsedTime );
	if( GetLifespan( ) < 0 )
		SetActive( false );

	switch( GetTier( ) )
	{
	case 1:
		{
			UpdateTier1( fElapsedTime );
			break;
		}
	case 2:
		{
			UpdateTier2( fElapsedTime );
			break;
		}
	case 3:
		{
			UpdateTier3( fElapsedTime );
			break;
		}
	}
}

void CEarth::UpdateTier1( float fElapsedTime )
{
	SetVelY( GetVelY( ) + fElapsedTime * 100 );

	if( m_fTimeTillRotate > 0 )
		m_fTimeTillRotate = m_fTimeTillRotate - fElapsedTime;
	else if( GetVelX( ) != 0 && GetVelY( ) != 0 && !collided )
	{
		m_fDisplay = m_fDisplay + fElapsedTime;

		if( m_fDisplay > PI )
			m_fDisplay = float( PI );

		Vector2d spin, currpoint;;
		spin.fX        = GetPosX( ) + GetVelX( );
		spin.fY        = GetPosY( ) + GetVelY( );
		currpoint.fX   = GetPosX( );
		currpoint.fY   = GetPosY( );

		float dot( ( spin.fX * currpoint.fX ) + ( currpoint.fY * spin.fY ) );
		float length( sqrt( ( spin.fX * spin.fX ) + ( spin.fY * spin.fY ) ) * 
			sqrt( ( currpoint.fX * currpoint.fX ) + ( currpoint.fY * currpoint.fY ) ) );

		if( length != 0.0f )
		{
			m_fRotate = float( acos( dot / length ) + PI / 4.0f );

			if( 0 == GetDirection( ) )
				m_fRotate = float( m_fRotate + PI );

			Vector2d rotate;
			rotate.fX = 0;
			rotate.fY = GetVelY( ) * fElapsedTime;

			Vector2d rotated = Rotate( rotate, m_fRotate );

			SetVelY( GetVelY( ) + rotated.fY );
			SetVelX( GetVelX( ) + rotated.fX );

			if( GetVelX( ) < 30 )
				SetVelX( 30 );
			if( GetVelY( ) > 450 )
				SetVelY( 450 );
		}

		spin.fX       = GetPosX( ) + GetVelX( );
		spin.fY       = GetPosY( ) + GetVelY( );
		currpoint.fX  = GetPosX( );
		currpoint.fY  = GetPosY( );

		dot           = ( spin.fX * currpoint.fX ) + ( currpoint.fY * spin.fY );
		length        = sqrt( ( spin.fX * spin.fX ) + ( spin.fY * spin.fY ) ) * 
			sqrt( ( currpoint.fX * currpoint.fX ) + ( currpoint.fY * currpoint.fY ) );
		m_fRotate     = float( acos( dot / length ) + PI / 4.0f );
	}

	CSpell::UpdateTier1( fElapsedTime );

	if( GetPosX( ) < 0 )
	{
		SetPosX( 0 );
		SetVelX( 0 );
	}
}

void CEarth::UpdateTier2( float fElapsedTime )
{ /* NOTHING HERE YET */ }

void CEarth::UpdateTier3( float fElapsedTime )
{ /* NOTHING HERE YET */ }

void CEarth::Render( )
{
	switch( GetTier( ) )
	{
	case 1:
		{
			RenderTier1( );
			break;
		}
	case 2:
		{
			RenderTier2( );
			break;
		}
	case 3:
		{
			RenderTier3( );
			break;
		}
	};
}

void CEarth::RenderTier1( )
{
	if( GetImage( ) != -1 )
		CSGD_TextureManager::GetInstance( )->Draw( GetImage( ), 
		int( GetPosX( ) - CCamera::GetCamera( )->GetXOffset( ) ), 
		int( GetPosY( )- CCamera::GetCamera( )->GetYOffset( ) ) );
}

void CEarth::RenderTier2( )
{ /* NOTHING HERE YET */ }

void CEarth::RenderTier3( )
{ /* NOTHING HERE YET */ }

void CEarth::HandleCollision( CBase* pObject )
{
	if( 1 == GetTier( ) )
	{
		if( pObject->GetType( ) == OBJ_TERRA || pObject->GetType( ) ==  OBJ_PLAYER  )
		{
			if( pObject->GetPosX( ) + 1 > GetPosX( ) + GetWidth( ) || 
				pObject->GetPosX( ) + GetWidth( ) - 1 < GetPosX( ) )
				SetVelX(GetVelX( ) * -0.5f );

			if( pObject->GetPosY( ) + GetHeight( ) - 1 < GetPosY( ) )
				SetVelY(GetVelX( ) * -0.4f );

			if( pObject->GetPosY( ) + 1 > GetPosY( ) + GetHeight( ) )
				SetVelY( GetVelY( ) * -0.2f );

			this->MoveOutOf( pObject );
			collided = true;

			if( pObject->GetType( ) == OBJ_PLAYER && !PlayerShot( ) )
				SetActive( false );
		}

		if( pObject->GetType( ) == OBJ_SPELL )
		{
			if( ( ( CSpell* )pObject )->GetElement( ) == OBJ_EARTH )
			{
				SetVelX( GetVelX( ) * -0.4f );
				SetVelY( GetVelY( ) * -0.2f );
			}
			pObject->MoveOutOf( this );
		}
	}
	else if( 2 == GetTier( ) )
	{ /* do stuff... like destroy... EVERYTHING */ }
	else if( 3 == GetTier( ) )
	{ /* holy crap everything go splode */ }

}