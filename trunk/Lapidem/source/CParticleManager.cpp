#include "stdheaders.h"

CParticleManager::CParticleManager( )
{ /* NOTHING TO SEE HERE */ }

CParticleManager::~CParticleManager( )
{ /* NOTHING TO SEE HERE */ }

CParticleManager* CParticleManager::GetInstance( )
{
	static CParticleManager instance;
	return &instance;
}

void CParticleManager::Update( f32 fET )
{
	for( u32 i = 0; i < m_vEmitters.size( ); i++ )
		m_vEmitters[i]->Update( fET );
}

void CParticleManager::Render( )
{
	for( u32 i = 0; i < m_vEmitters.size( ); i++ )
		m_vEmitters[i]->Render( );
}

void CParticleManager::AddEmitter( CEmitter* pEmitter )
{ m_vEmitters.push_back( pEmitter ); }

void CParticleManager::RemoveEmitter( CEmitter* _param )
{
	for( vector<CEmitter*>::iterator iter = m_vEmitters.begin( ); 
		iter != m_vEmitters.end( ); iter++ )
		if( ( *iter ) == _param )
		{
			_param->ClearParticles( );
			m_vEmitters.erase( iter );
			delete _param;
			return;
		}
}

CEmitter* CParticleManager::LoadEmitter( const char* szFileName, f32 fPosX, f32 fPosY, 
										f32 fVelX, f32 fVelY, f32 fAccX, f32 fAccY )
{
	CEmitter *_emit = new CEmitter( );
	ifstream fin( szFileName, ios_base::binary );

	if( fin.is_open( ) && fin.good( ) )
	{
		s32 _iBuff( 0 );
		f32 _fBuff( 0 );
		s1 _bBuff( 0 );
		u1 _cBuff( 0 );

		fin.read( reinterpret_cast<char*>( &_iBuff ), sizeof( s32 ) );
		fin.get( );

		char _characterBuff[128];
		ZeroMemory( _characterBuff, 128 );
		fin.read( reinterpret_cast<char*>( &_characterBuff ), sizeof( char ) * _iBuff );
		string _sz( "resource/graphics/" );
		_sz = _sz + _characterBuff;

		_emit->SetImageID( CSGD_TextureManager::GetInstance( )->LoadTexture
			( _sz.c_str( ), ARGB( 255, 0, 0, 0 ) ) );

		fin.read( reinterpret_cast<char*>( &_iBuff ), sizeof( s32 ) );
		_emit->SetNumberOfParticles( _iBuff ); 

		fin.read( reinterpret_cast<char*>( &_fBuff ), sizeof( f32 ) );
		_emit->SetSpawnRate( _fBuff ); 

		fin.read( reinterpret_cast<char*>( &_fBuff ), sizeof( f32 ) );
		_emit->SetLifeSpan( _fBuff ); 

		fin.read( reinterpret_cast<char*>( &_fBuff ), sizeof( f32 ) );
		_emit->SetPosX( _fBuff ); 

		fin.read( reinterpret_cast<char*>( &_fBuff ), sizeof( f32 ) );
		_emit->SetPosY( _fBuff ); 

		fin.read( reinterpret_cast<char*>( &_fBuff ), sizeof( f32 ) );
		_emit->SetParticleVelX( _fBuff ); 

		fin.read( reinterpret_cast<char*>( &_fBuff ), sizeof( f32 ) );
		_emit->SetParticleVelY( _fBuff ); 

		fin.read( reinterpret_cast<char*>( &_fBuff ), sizeof( f32 ) );
		_emit->SetAccX( _fBuff ); 

		fin.read( reinterpret_cast<char*>( &_fBuff ), sizeof( f32 ) );
		_emit->SetAccY( _fBuff ); 

		fin.read( reinterpret_cast<char*>( &_cBuff ), sizeof( u1 ) );
		_emit->SetStartAlpha( _cBuff ); 

		fin.read( reinterpret_cast<char*>( &_cBuff ), sizeof( u1 ) );
		_emit->SetStartRed( _cBuff ); 

		fin.read( reinterpret_cast<char*>( &_cBuff ), sizeof( u1 ) );
		_emit->SetStartGreen( _cBuff ); 

		fin.read( reinterpret_cast<char*>( &_cBuff ), sizeof( u1 ) );
		_emit->SetStartBlue( _cBuff ); 

		fin.read( reinterpret_cast<char*>( &_cBuff ), sizeof( u1 ) );
		_emit->SetEndAlpha( _cBuff ); 

		fin.read( reinterpret_cast<char*>( &_cBuff ), sizeof( u1 ) );
		_emit->SetEndRed( _cBuff ); 

		fin.read( reinterpret_cast<char*>( &_cBuff ), sizeof( u1 ) );
		_emit->SetEndGreen( _cBuff ); 

		fin.read( reinterpret_cast<char*>( &_cBuff ), sizeof( u1 ) );
		_emit->SetEndBlue( _cBuff ); 

		fin.read( reinterpret_cast<char*>( &_fBuff ), sizeof( f32 ) );
		_emit->SetSpreadAngle( _fBuff ); 

		fin.read( reinterpret_cast<char*>( &_fBuff ), sizeof( f32 ) );
		_emit->SetSpreadDirection( _fBuff ); 

		fin.read( reinterpret_cast<char*>( &_fBuff ), sizeof( f32 ) );
		_emit->SetStartScale( _fBuff ); 

		fin.read( reinterpret_cast<char*>( &_fBuff ), sizeof( f32 ) );
		_emit->SetEndScale( _fBuff ); 

		fin.read( reinterpret_cast<char*>( &_iBuff ), sizeof( int ) );
		_emit->SetSourceBlend( _iBuff ); 

		fin.read( reinterpret_cast<char*>( &_iBuff ), sizeof( int ) );
		_emit->SetDestinationBlend( _iBuff ); 

		fin.read( reinterpret_cast<char*>( &_bBuff ), sizeof( s1 ) );
		_emit->SetGravity( _bBuff ); 

		fin.read( reinterpret_cast<char*>( &_fBuff ), sizeof( f32 ) );
		_emit->SetGravityPosX( _fBuff ); 

		fin.read( reinterpret_cast<char*>( &_fBuff ), sizeof( f32 ) );
		_emit->SetGravityPosY( _fBuff ); 

		fin.read( reinterpret_cast<char*>( &_fBuff ), sizeof( f32 ) );
		_emit->SetGravityVelX( _fBuff ); 

		fin.read( reinterpret_cast<char*>( &_fBuff ), sizeof( f32 ) );
		_emit->SetGravityVelY( _fBuff ); 

		fin.read( reinterpret_cast<char*>( &_bBuff ), sizeof( s1 ) );
		_emit->SetShape( _bBuff ); 

		fin.read( reinterpret_cast<char*>( &_iBuff ), sizeof( f32 ) );
		_emit->SetWidth( _iBuff ); 

		fin.read( reinterpret_cast<char*>( &_iBuff ), sizeof( f32 ) );
		_emit->SetHeight( _iBuff ); 

		fin.read( reinterpret_cast<char*>( &_iBuff ), sizeof( f32 ) );
		_emit->SetRadius( _iBuff ); 

		fin.read( reinterpret_cast<char*>( &_bBuff ), sizeof( s1 ) );
		_emit->SetLooping( _bBuff ); 
	}

	_emit->SetGravityPosX( fPosX + ( _emit->GetGravityPosX( ) - _emit->GetPosX( ) ) );
	_emit->SetGravityPosY( fPosY + ( _emit->GetGravityPosY( ) - _emit->GetPosY( ) ) );

	_emit->SetPosX( fPosX );
	_emit->SetPosY( fPosY );
	_emit->SetVelX( fVelX );
	_emit->SetVelY( fVelY );

	return _emit;
}

void CParticleManager::ClearAll()
{
	for( u32 i = 0; i < m_vEmitters.size( ); i++ )
		m_vEmitters[i]->ClearParticles( );

	for( vector<CEmitter*>::iterator iter = m_vEmitters.begin( ); 
		iter != m_vEmitters.end( ); iter++ )
		delete ( *iter );

	m_vEmitters.clear( );
}