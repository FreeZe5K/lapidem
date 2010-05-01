#include "CParticleManager.h"
#include "CGame.h"
#include <fstream>
using std::ifstream;
using std::ios_base;

// - - - - - - - - - - - - - - - - - - - -
// Particle manager.
// - - - - - - - - - - - - - - - - - - - -

CParticleManager *CParticleManager::GetInstance( )
{
	static CParticleManager instance;
	return &instance;
}

CParticleManager::CParticleManager( )
{
	m_pD3D  = CSGD_Direct3D::GetInstance( );
	m_pTM   = CSGD_TextureManager::GetInstance( );
}

CParticleManager::~CParticleManager( )
{
	m_pD3D  = NULL;
	m_pTM   = NULL;
}

void CParticleManager::UnloadAll( )
{
	for( u32 u = 0; u < m_vEmitters.size( ); u++ )
		m_vEmitters[u]->ClearParticles( );

	for( vector<CEmitter*>::iterator _it = m_vEmitters.begin( );
		_it != m_vEmitters.end( ); _it++ )
		delete *_it;

	m_vEmitters.clear( );
}

void CParticleManager::Render( )
{
	for( u32 u = 0; u < m_vEmitters.size( ); u++ )
	{
		//if( m_vEmitters[u]->GetPosX( ) > CCamera::GetCamera( )->GetXOffset( ) 
		//	&& m_vEmitters[u]->GetPosY( ) > CCamera::GetCamera( )->GetYOffset( ) 
		//	&& m_vEmitters[u]->GetPosX( ) < ( CCamera::GetCamera( )->GetXOffset( ) 
		//	+ CGame::GetInstance( )->GetScreenWidth( ) ) && m_vEmitters[u]->GetPosY( ) 
		//	< ( CCamera::GetCamera( )->GetYOffset( ) + CGame::GetInstance( )->GetScreenHeight( ) ) ) 
		//{
			ul32 _source;
			ul32 _destination;

			m_pD3D->GetDirect3DDevice( )->GetRenderState( D3DRS_SRCBLEND, &_source );
			m_pD3D->GetDirect3DDevice( )->GetRenderState( D3DRS_DESTBLEND, &_destination );

			m_pD3D->GetDirect3DDevice( )->SetRenderState( D3DRS_SRCBLEND, m_vEmitters[u]->GetSourceBlend( ) );
			m_pD3D->GetDirect3DDevice( )->SetRenderState( D3DRS_DESTBLEND, m_vEmitters[u]->GetDestinationBlend( ) );

			m_vEmitters[u]->Render( );

			m_pD3D->GetDirect3DDevice( )->SetRenderState( D3DRS_SRCBLEND, _source );
			m_pD3D->GetDirect3DDevice( )->SetRenderState( D3DRS_DESTBLEND, _destination );
		//}
	}
}

void CParticleManager::Update( float fET )
{
	for( u32 u = 0; u < m_vEmitters.size( ); u++ )
	{
		m_vEmitters[u]->Update( fET );

		if( m_vEmitters[u]->EmitterDone( ) )
		{
			m_vEmitters[u]->ClearParticles( );
			delete m_vEmitters[u];
			m_vEmitters.erase( m_vEmitters.begin( ) + u );
		}
	}
}

void CParticleManager::AddEmitter( CEmitter *_emit )
{ m_vEmitters.push_back( _emit ); }

u32 CParticleManager::GetNumEmitters( )
{ return m_vEmitters.size( ); }

// - - - - - - - - - - - - - - - - - - - -
// Emitter factory.
// - - - - - - - - - - - - - - - - - - - -
CEmitterFactory::CEmitterFactory( )
{ /* DO NOTHING */ }

CEmitterFactory::~CEmitterFactory( )
{ /* DO NOTHING */ }

CEmitterFactory *CEmitterFactory::GetInstance( )
{
	static CEmitterFactory instance;
	return &instance;
}

CEmitter *CEmitterFactory::CreateEmitter( const char *szName )
{
	for( u32 u = 0; u < m_vEmitters.size( ); u++ )
	{
		if( _stricmp( m_vEmitters[u]->GetName( ), szName ) == 0 )
		{
			CEmitter *_emit = new CEmitter( );
			*_emit = *m_vEmitters[u];
			return _emit;
		}
	}

	return NULL;
}

int CEmitterFactory::Load( const char* szFileName, const char* szEmitterName )
{
	char* _path = "resource/graphics/";

	int _numParticles;
	int _srcBlend;
	int _destBlend;
	int _emitRate;

	float _startScale;
	float _endScale;
	float _angle;
	float _lifeSpan;
	float _emitPosX;
	float _emitPosY;
	float _emitVelX;
	float _emitVelY;
	float _startVelX;
	float _startVelY;
	float _endVelX;
	float _endVelY;
	float _accelX;
	float _accelY;
	//float _spreadX;
	//float _spreadY;

	u1 _startAlpha;
	u1 _startRed;
	u1 _startGreen;
	u1 _startBlue;
	u1 _endAlpha;
	u1 _endRed;
	u1 _endGreen;
	u1 _endBlue;

	char _stringSize;

	bool _looping;

	char* _name;
	char* _image;

	CEmitter *m_pEmitter = new CEmitter( );

	ifstream fin( szFileName, ios::in | ios::binary );

	if( fin.is_open( ) )
	{		
		fin.read( ( char* )&_stringSize, sizeof( char ) );
		char name[32];
		fin.read( name, _stringSize );
		name[_stringSize ] =0;

		fin.read( ( char* )&_numParticles, sizeof( int ) );

		fin.read( ( char* )&_startAlpha, sizeof( u1 ) );
		fin.read( ( char* )&_startRed, sizeof( u1 ) );
		fin.read( ( char* )&_startGreen, sizeof( u1 ) );
		fin.read( ( char* )&_startBlue, sizeof( u1 ) );

		fin.read( ( char* )&_endAlpha, sizeof( u1 ) );
		fin.read( ( char* )&_endRed, sizeof( u1 ) );
		fin.read( ( char* )&_endGreen, sizeof( u1 ) );
		fin.read( ( char* )&_endBlue, sizeof( u1 ) );

		fin.read( ( char* )&_startScale, sizeof( f32 ) );
		fin.read( ( char* )&_endScale, sizeof( f32 ) );

		fin.read( ( char* )&_emitRate, sizeof( int ) );
		fin.read( ( char* )&_angle, sizeof( f32 ) );
		fin.read( ( char* )&_looping, sizeof( bool ) );
		fin.read( ( char* )&_lifeSpan, sizeof( f32 ) );

		fin.read( ( char* )&_emitPosX, sizeof( f32 ) );
		fin.read( ( char* )&_emitPosY, sizeof( f32 ) );

		fin.read( ( char* )&_emitVelX, sizeof( f32 ) );
		fin.read( ( char* )&_emitVelY, sizeof( f32 ) );

		fin.read( ( char* )&_startVelX, sizeof( f32 ) );
		fin.read( ( char* )&_startVelY, sizeof( f32 ) );

		fin.read( ( char* )&_endVelX, sizeof( f32 ) );
		fin.read( ( char* )&_endVelY, sizeof( f32 ) );

		fin.read( ( char* )&_accelX, sizeof( f32 ) );
		fin.read( ( char* )&_accelY, sizeof( f32 ) );

		// - - - - - - - - - - - - - - - -
		// Fix reading this in . . .
		// - - - - - - - - - - - - - - - -
		fin.read( ( char* )&_srcBlend, sizeof( int ) );
		fin.read( ( char* )&_destBlend, sizeof( int ) );
		
		fin.close( );

		m_pEmitter->SetNumParticles( _numParticles );
		m_pEmitter->GetParticle( )->SetStartAlpha( _startAlpha );
		m_pEmitter->GetParticle( )->SetStartRed( _startRed );
		m_pEmitter->GetParticle( )->SetStartGreen( _startGreen );
		m_pEmitter->GetParticle( )->SetStartBlue( _startBlue );

		if( szEmitterName )
			m_pEmitter->SetName( ( char* )szEmitterName ); 
		else m_pEmitter->SetName( name );

		m_pEmitter->SetEmitRate( _emitRate );

		m_pEmitter->GetParticle( )->SetEndAlpha( _endAlpha );
		m_pEmitter->GetParticle( )->SetEndRed( _endRed );
		m_pEmitter->GetParticle( )->SetEndGreen( _endGreen );
		m_pEmitter->GetParticle( )->SetEndBlue( _endBlue );

		m_pEmitter->GetParticle( )->SetStartScale( _startScale );
		m_pEmitter->GetParticle( )->SetEndScale( _endScale );

		m_pEmitter->SetAngle( _angle );
		m_pEmitter->SetLooping( _looping );
		m_pEmitter->GetParticle( )->SetLifeSpan( _lifeSpan / 2 );
		m_pEmitter->SetLifeSpan( _lifeSpan / 2 );

		m_pEmitter->SetVelX( _emitVelX );
		m_pEmitter->SetVelY( _emitVelY );
		m_pEmitter->SetPosX( 0 );
		m_pEmitter->SetPosY( 0 );
		m_pEmitter->GetParticle( )->SetPosX( 0 );
		m_pEmitter->GetParticle( )->SetPosY( 0 );

		m_pEmitter->GetParticle( )->SetStartVelX( _startVelX );
		m_pEmitter->GetParticle( )->SetStartVelY( _startVelY );

		m_pEmitter->GetParticle( )->SetEndVelX( _endVelX );
		m_pEmitter->GetParticle( )->SetEndVelY( _endVelY );

		m_pEmitter->GetParticle( )->SetAccelX( _accelX );
		m_pEmitter->GetParticle( )->SetAccelY( _accelY );

		m_pEmitter->SetEmitTimer( 0.0f );
		m_pEmitter->SetSpawnTime( 0.1f );

		m_pEmitter->SetSpreadX( 0.3f ); // _spreadX
		m_pEmitter->SetSpreadY( 0.3f ); // _spreadY

		m_pEmitter->SetSourceBlend( 5 );//_srcBlend
		m_pEmitter->SetDestinationBlend( 7 );//_destBlend

		char* _fullPath = new char[int( strlen( _path ) + strlen( name ) ) + 1];

		strcpy_s( _fullPath, ( strlen( _path ) + strlen( name ) ) + 1, _path );
		strcat_s( _fullPath, ( strlen( _path ) + strlen( name ) ) + 1, name );

		m_pEmitter->GetParticle( )->SetImageID( m_pTM->LoadTexture( 
			_fullPath, D3DCOLOR_XRGB( 0, 0, 0 ) ) );

		m_vEmitters.push_back( m_pEmitter );

		delete[] _fullPath;

		_name      = NULL;
		_image     = NULL;
	} 

	return 0;
}

void CEmitterFactory::UnloadAll( )
{
	for( u32 u = 0; u < m_vEmitters.size( ); u++ )
		m_vEmitters[u]->ClearParticles( );

	for( vector<CEmitter*>::iterator _it = m_vEmitters.begin( );
		_it != m_vEmitters.end( ); _it++ )
		delete *_it;

	m_vEmitters.clear( );
}

void CEmitterFactory::Initialize( )
{
	m_pTM = CSGD_TextureManager::GetInstance( );

	//Load( "resource/data/fireSpell.lapipt", "firespell" );
	Load( "resource/data/testFire.lapipt", "firespell" );
	Load( "resource/data/return.lapipt", "return" );
}