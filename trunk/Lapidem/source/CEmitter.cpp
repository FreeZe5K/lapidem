#include "CEmitter.h"
#include "Lapidem_Math.h"
#include <Windows.h>      // do we really need this?
#include "CCamera.h"

CEmitter::CEmitter( )
{
	m_pParticle       = new CParticle( );
	m_nParticlesMade  = 0;

	SetPosX( 0 );
	SetPosY( 0 );
	SetVelX( 0 );
	SetVelY( 0 );
	SetSpreadX( 0 );
	SetSpreadY( 0 );

	SetNumParticles( 0 );
	SetLifeSpan( 0 );
	SetEmitTimer( 0 );
	SetSpawnTime( 0 );
	SetLooping( 0 );
	SetAngle( 0 );
	SetSourceBlend( 0 );
	SetDestinationBlend( 0 );
	SetEmitRate( 0 );

	SetName( NULL ) ;
}

CEmitter::~CEmitter( )
{
	delete m_pParticle;
	m_pParticle = NULL;
}

void CEmitter::Render( )
{
	for( u32 u = 0; u < m_vLiveParticles.size( ); u++ )
		if( m_vLiveParticles.size( ) > 0 )
			m_vLiveParticles[u]->Render( );
}

void CEmitter::Update( float fET )
{
	m_fEmitTimer = m_fEmitTimer + fET;

	m_fPosX = m_fPosX + ( m_fVelX * fET );
	m_fPosY = m_fPosY + ( m_fVelY * fET );

	CParticle *particle;

	while( m_fEmitTimer >= m_fSpawnTime )
	{
		if( m_nParticlesMade < m_nNumParticles )
		{
			for( u32 u = 0; u < u32( m_nEmitRate ); u++ )
			{   
				if( ( m_vDeadParticles.size( ) + 
					m_vLiveParticles.size( ) ) > u32( m_nNumParticles ) )
					break;

				particle = new CParticle( );
				AddParticle( particle );
				m_nParticlesMade++;
			}
		}
		else
		{
			if( ( m_vDeadParticles.size( ) + m_vLiveParticles.size( ) ) 
				== m_nNumParticles && !m_bLooping )
				break;

			if( m_vDeadParticles.size( ) == 0 )
				m_fEmitTimer = 0;

			if( m_vDeadParticles.size() > 0 )
			{
				for( u32 u = 0; u < u32( m_nEmitRate ); u++ )
				{
					if( ( m_vDeadParticles.size( ) + 
						m_vLiveParticles.size( ) ) >= u32( m_nNumParticles ) )
						break;

					if( m_vDeadParticles.size( ) == 0 )
						break;

					AddParticle( m_vDeadParticles[0] );
					m_vDeadParticles.erase( m_vDeadParticles.begin( ) );
				}
			}
		}

		m_fEmitTimer = m_fEmitTimer - m_fSpawnTime;
	}

	for( u32 u = 0; u < m_vLiveParticles.size(); u++)
	{
		if( m_vLiveParticles[u]->GetAge( ) >= m_fLifeSpan )
		{
			m_vDeadParticles.push_back( m_vLiveParticles[u] );

			if( u < m_vLiveParticles.size() )
				m_vLiveParticles.erase( m_vLiveParticles.begin( ) + u );

			u--;
		} else m_vLiveParticles[u]->Update( fET );
	}
}

void CEmitter::AddParticle( CParticle *_p )
{
	_p->SetStartVelX( m_pParticle->GetStartVelX( ) );
	_p->SetStartVelY( m_pParticle->GetStartVelY( ) );
	_p->SetEndVelX( m_pParticle->GetEndVelX( ) );
	_p->SetEndVelY( m_pParticle->GetEndVelY( ) );

	Vector2D _angle;

	_angle._x = m_pParticle->GetStartVelX( );
	_angle._y = m_pParticle->GetStartVelY( );

	_angle = VectorRotate( _angle, DegreeToRadian( RAND_FLOAT( -m_fAngle, m_fAngle ) ) );

	_p->SetStartVelX( _angle._x );
	_p->SetStartVelY( _angle._y );

	_p->SetPosX( GetPosX( ) );
	_p->SetPosY( GetPosY( ) );

	_p->SetPosX(RAND_FLOAT( m_fPosX - m_fSpreadX, m_fPosX + m_fSpreadX ) );
	_p->SetPosY(RAND_FLOAT( m_fPosY - m_fSpreadY, m_fPosY + m_fSpreadY ) );

	_p->SetAccelX( m_pParticle->GetAccelX( ) );
	_p->SetAccelY( m_pParticle->GetAccelY( ) );

	_p->SetHeight( 16 ); // m_Particle->GetHeight()
	_p->SetWidth( 16 );  // m_Particle->GetWidth()

	_p->SetStartScale( m_pParticle->GetStartScale( ) );
	_p->SetEndScale( m_pParticle->GetEndScale( ) );

	_p->SetAge( 0.0f );
	_p->SetImageID( m_pParticle->GetImageID( ) );
	_p->SetLifeSpan( m_pParticle->GetLifeSpan( ) );

	_p->SetStartAlpha( m_pParticle->GetStartAlpha( ) );
	_p->SetStartRed( m_pParticle->GetStartRed( ) );
	_p->SetStartGreen( m_pParticle->GetStartGreen( ) );
	_p->SetStartBlue( m_pParticle->GetStartBlue( ) );

	_p->SetEndAlpha( m_pParticle->GetEndAlpha( ) );
	_p->SetEndRed( m_pParticle->GetEndRed( ) );
	_p->SetEndGreen( m_pParticle->GetEndGreen( ) );
	_p->SetEndBlue( m_pParticle->GetEndBlue( ) );

	m_vLiveParticles.push_back( _p );
	_p = NULL;
}

void CEmitter::ClearParticles( )
{
	for( vector<CParticle*>::iterator _it = m_vLiveParticles.begin( );
		_it != m_vLiveParticles.end( ); _it++ )
		delete *_it;
	m_vLiveParticles.clear( );

	for( vector<CParticle*>::iterator _it = m_vDeadParticles.begin( );
		_it != m_vDeadParticles.end( ); _it++ )
		delete *_it;
	m_vDeadParticles.clear( );
}

Vector2D CEmitter::VectorRotate( Vector2D _vector, float _rad )
{
	_vector._y = _vector._y * -1.0f;

	Vector2D vRot;
	vRot._x = float( cos( double( _rad ) * _vector._y ) + 
		sin( double( _rad ) * _vector._y ) );
	vRot._y = float( ( sin( _rad ) * _vector._y ) + 
		( cos( _rad ) * _vector._y ) );

	vRot._y = vRot._y * -1.0f;
	return vRot;
}

float CEmitter::DegreeToRadian( float _deg )
{ return ( ( _deg ) * ( 3.141592654f / 180.0f ) ); }

CEmitter& CEmitter::operator=( const CEmitter& _emit )
{
	SetNumParticles( _emit.GetNumParticles( ) );
	GetParticle( )->SetStartAlpha( _emit.GetParticle( )->GetStartAlpha( ) );
	GetParticle( )->SetStartRed( _emit.GetParticle( )->GetStartRed( ) );
	GetParticle( )->SetStartGreen( _emit.GetParticle( )->GetStartGreen( ) );
	GetParticle( )->SetStartBlue( _emit.GetParticle( )->GetStartBlue( ) );

	SetName( _emit.GetName( ) );
	SetEmitRate( _emit.GetEmitRate( ) );

	GetParticle( )->SetEndAlpha( _emit.GetParticle( )->GetEndAlpha( ) );
	GetParticle( )->SetEndRed( _emit.GetParticle( )->GetEndRed( ) );
	GetParticle( )->SetEndGreen( _emit.GetParticle( )->GetEndGreen( ) );
	GetParticle( )->SetEndBlue( _emit.GetParticle( )->GetEndBlue( ) );

	GetParticle( )->SetStartScale( _emit.GetParticle( )->GetStartScale( ) );
	GetParticle( )->SetEndScale( _emit.GetParticle( )->GetEndScale( ) );

	SetAngle( _emit.GetAngle( ) );
	SetLooping( _emit.GetLooping( ) );
	GetParticle( )->SetLifeSpan( _emit.GetLifeSpan( ) );
	SetLifeSpan( _emit.GetLifeSpan( ) );

	SetVelX( _emit.GetVelX( ) );
	SetVelY( _emit.GetVelY( ) );

	GetParticle( )->SetStartVelX( _emit.GetParticle( )->GetStartVelX( ) );
	GetParticle( )->SetStartVelY( _emit.GetParticle( )->GetStartVelY( ) );

	GetParticle( )->SetEndVelX( _emit.GetParticle( )->GetEndVelX( ) );
	GetParticle( )->SetEndVelY( _emit.GetParticle( )->GetEndVelY( ) );

	GetParticle( )->SetAccelX( _emit.GetParticle( )->GetAccelX( ) );
	GetParticle( )->SetAccelY( _emit.GetParticle( )->GetAccelY( ) );

	GetParticle( )->SetAge( 0 );

	SetEmitTimer( 0.0f );
	SetSpawnTime( 0.1f );

	SetSpreadX( _emit.GetSpreadX( ) );
	SetSpreadY( _emit.GetSpreadY( ) );

	SetSourceBlend( _emit.GetSourceBlend( ) );
	SetDestinationBlend( _emit.GetDestinationBlend( ) );

	GetParticle( )->SetImageID( _emit.GetParticle( )->GetImageID( ) );

	return *this;
}

bool CEmitter::EmitterDone( )
{
	if( ( m_nParticlesMade == m_nNumParticles ) && !m_bLooping )
		if( m_vLiveParticles.size( ) == 0 )
			return true;
	return false;
}