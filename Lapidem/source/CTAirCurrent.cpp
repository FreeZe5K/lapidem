#include "CTAirCurrent.h"
#include "CSpell.h"
#include "CEarth.h"
#include "CParticleManager.h"


CTAirCurrent::CTAirCurrent()
{
	m_emitter = CParticleManager::GetInstance()->LoadEmitter("resource/data/aircurrent.lapipt",0,0);
	CParticleManager::GetInstance()->AddEmitter( m_emitter );
	m_fDelay = 0.0f;

}
CTAirCurrent::~CTAirCurrent()
{
	CParticleManager::GetInstance()->RemoveEmitter(m_emitter);
}


void CTAirCurrent::SetDirection(int DirectionInDegrees)
{

	if(DirectionInDegrees > 360)
		DirectionInDegrees -= 360;

	m_nDirecionDeg = DirectionInDegrees;

	if(DirectionInDegrees == 0 || DirectionInDegrees == 360)
		shotDirect = RIGHT;
	else if(DirectionInDegrees == 90)
		shotDirect = UP;
	else if(DirectionInDegrees == 180)
		shotDirect = LEFT;
	else if(DirectionInDegrees == 270)
		shotDirect = DOWN;
	else if(DirectionInDegrees > 0 && DirectionInDegrees < 90)
		shotDirect = RIGHT_UP;
	else if(DirectionInDegrees > 90 && DirectionInDegrees < 180)
		shotDirect = LEFT_UP;
	else if(DirectionInDegrees > 180 && DirectionInDegrees < 270)
		shotDirect = LEFT_DOWN;
	else
		shotDirect = RIGHT_DOWN;

	old_Direct = shotDirect;


}

void CTAirCurrent::HandleCollision(CBase * collidingObject)
{

	if(collidingObject->GetType() < 0)
		return;

	if(collidingObject->GetType() == OBJ_SPELL && ((CSpell*)collidingObject)->GetElement() == OBJ_EARTH)
		m_fTransformTimer = 3.0f;
	SetCollided( false );

}

void CTAirCurrent::Update(float fElapsedTime)
{
	if(m_fTransformTimer >= 0)
		shotDirect = NO_DIRECTION;
	else
		shotDirect = old_Direct;

	if( shotDirect == NO_DIRECTION )
		m_emitter->SetLooping(false);
	else
		m_emitter->SetLooping(true);

	m_fTransformTimer -= fElapsedTime;

	if( m_fDelay >= 2.0f )
	{
		m_fDelay = 0.0f;
		m_emitter->SetPosX(GetPosX());
		m_emitter->SetPosY(GetPosY());
	}

	float fVelX = 500* (float)cos( m_nDirecionDeg * 3.14 / 180 );
	float fVelY = -1000* (float)sin( m_nDirecionDeg * 3.14 / 180 );

	m_emitter->SetParticleVelX( fVelX );
	m_emitter->SetParticleVelY( fVelY );

	m_fDelay += fElapsedTime;
	//m_emitter->SetPosX( m_emitter->GetPosX() + fVelX*fElapsedTime);
	//m_emitter->SetPosY( m_emitter->GetPosY() + fVelY*fElapsedTime);


}