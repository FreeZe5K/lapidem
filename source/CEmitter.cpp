#include "CEmitter.h"
#include "Wrappers/CSGD_Direct3D.h"

CEmitter::CEmitter( )
{
	srand( u32( time( 0 ) ) );
	m_fTimeSinceLastSpawn = 0.0f;
}

CEmitter::~CEmitter( )
{ /* NOTHING TO SEE HERE */ }

void CEmitter::Update( float fElapsedTime )
{
	m_fTimeSinceLastSpawn = m_fTimeSinceLastSpawn + fElapsedTime;

	if( m_fTimeSinceLastSpawn > m_fSpawnRate )
	{
		for( int i = 0; i < m_fTimeSinceLastSpawn / m_fSpawnRate; i++ )
		{
			if( m_vLiveParticles.size( ) >= u32( m_nNumOfParticles) )
			{
				m_fTimeSinceLastSpawn = 0.0f;
				break;
			}

			if( m_vDeadParticles.size( ) > 0 )
			{
				AddParticle( m_vDeadParticles[m_vDeadParticles.size( ) - 1] );
				m_vDeadParticles.pop_back();
			}
			else
			{
				CParticle* _temp = new CParticle( );
				AddParticle( _temp );
			}
		}

		m_fTimeSinceLastSpawn = 0.0f;
	}

	for( u32 i = 0; i < m_vLiveParticles.size( ); i++ )
	{
		if( GetParticleVelX() )
		m_vLiveParticles[i]->SetVelX( GetParticleVelX());
		if( GetParticleVelY() )
		m_vLiveParticles[i]->SetVelY( GetParticleVelY());

		m_vLiveParticles[i]->Update( fElapsedTime );

		if( m_vLiveParticles[i]->GetAge( ) >= m_fLifeSpan )
			if( m_bLooping )
			{
				m_vDeadParticles.push_back( m_vLiveParticles[i] );
				m_vLiveParticles.erase( m_vLiveParticles.begin( ) + i );
				i--;
			} else m_vLiveParticles[i]->SetCurrScale( 0.0f ); 
		else UpdateParticle( m_vLiveParticles[i], fElapsedTime ); 
	}
}

void CEmitter::Render( )
{

	DWORD nSource, nDestination;

	CSGD_Direct3D::GetInstance( )->GetDirect3DDevice( )->GetRenderState( D3DRS_SRCBLEND, &nSource );
	CSGD_Direct3D::GetInstance( )->GetDirect3DDevice( )->GetRenderState( D3DRS_DESTBLEND, &nDestination );

	CSGD_Direct3D::GetInstance( )->SpriteEnd( );
	CSGD_Direct3D::GetInstance( )->SpriteBegin( );

	CSGD_Direct3D::GetInstance( )->GetDirect3DDevice( )->SetRenderState( D3DRS_SRCBLEND, m_nSourceBlend );
	CSGD_Direct3D::GetInstance( )->GetDirect3DDevice( )->SetRenderState( D3DRS_DESTBLEND, m_nDestinationBlend );

	for( u32 i = 0; i < m_vLiveParticles.size( ); i++ )
		m_vLiveParticles[i]->Render( m_nImageID );

	CSGD_Direct3D::GetInstance( )->SpriteEnd( );
	CSGD_Direct3D::GetInstance( )->SpriteBegin( );

	CSGD_Direct3D::GetInstance( )->GetDirect3DDevice( )->SetRenderState( D3DRS_SRCBLEND, nSource );
	CSGD_Direct3D::GetInstance( )->GetDirect3DDevice( )->SetRenderState( D3DRS_DESTBLEND, nDestination );

}

void CEmitter::UpdateParticle( CParticle *_part, f32 fET )
{
	float fAgePercentage( _part->GetAge( ) / m_fLifeSpan );

	_part->SetCurrScale( m_fStartScale + ( ( m_fEndScale - m_fStartScale ) * fAgePercentage ) );

	_part->SetAlpha( int( m_nStartAlpha + ( ( m_nEndAlpha - m_nStartAlpha ) * fAgePercentage ) ) );
	_part->SetRed( int( m_nStartRed + ( ( m_nEndRed - m_nStartRed) * fAgePercentage ) ) );
	_part->SetGreen( int( m_nStartGreen + ( ( m_nEndGreen - m_nStartGreen ) * fAgePercentage ) ) );
	_part->SetBlue( int( m_nStartBlue + ( ( m_nEndBlue - m_nStartBlue ) * fAgePercentage ) ) );

	if( m_bGravity )
	{
		f32 fGravityX = m_fGravityPosX - _part->GetPosX( );
		f32 fGravityY = m_fGravityPosY - _part->GetPosY( );

		f32 fMagnitude = sqrt( ( fGravityX * fGravityX ) + ( fGravityY * fGravityY ) );
		fGravityX = fGravityX / fMagnitude;
		fGravityY = fGravityY / fMagnitude;

		_part->SetVelX( _part->GetVelX( ) + ( m_fGravityVelX * fET ) * fGravityX );
		_part->SetVelY( _part->GetVelY( ) + ( m_fGravityVelY * fET ) * fGravityY );
	}
}

void CEmitter::AddParticle( CParticle* _new )
{
	_new->SetAccX( m_fAccX );
	_new->SetAccY( m_fAccY );
	_new->SetAge( 0.0f );
	_new->SetCurrScale( m_fStartScale );
	_new->SetAlpha( m_nStartAlpha );
	_new->SetRed( m_nStartRed );
	_new->SetGreen( m_nStartGreen );
	_new->SetBlue( m_nStartBlue );

	double _deg( 3.14 * ( ( ( rand( ) % int( m_fSpreadAngle ) - 
		m_fSpreadAngle / 2 ) + m_fSpreadDirection ) / 180 ) );

	_new->SetVelX( f32( m_fParticleVelX * sin( _deg ) ) );
	_new->SetVelY( f32( m_fParticleVelY * cos( _deg ) ) );

	if( m_bShape )
	{
		_new->SetPosX( m_fPosX + ( rand( ) % m_nWidth - ( m_nWidth / 2 ) ) );
		_new->SetPosY( m_fPosY + ( rand( ) % m_nHeight - ( m_nHeight / 2 ) ) );
	}
	else
	{
		_deg = rand( ) % 361;
		_new->SetPosX( f32( m_fPosX + ( m_nRadius * sin( _deg ) ) ) );
		_new->SetPosY( f32( m_fPosY + ( m_nRadius * cos( _deg ) ) ) );
	}

	m_vLiveParticles.push_back( _new );
}

void CEmitter::ClearParticles( )
{
	for( vector<CParticle*>::iterator iter = m_vLiveParticles.begin( ); 
		iter != m_vLiveParticles.end( ); iter++ )
		delete ( *iter );

	for( vector<CParticle*>::iterator iter = m_vDeadParticles.begin( ); 
		iter != m_vDeadParticles.end( ); iter++ )
		delete ( *iter );

	m_vLiveParticles.clear( );
	m_vDeadParticles.clear( );
}