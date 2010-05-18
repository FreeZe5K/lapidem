#include "CTAirCurrent.h"
#include "CSpell.h"
#include "CEarth.h"

void CTAirCurrent::SetDirection(int DirectionInDegrees)
{

	if(DirectionInDegrees > 360)
		DirectionInDegrees -= 360;

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

	m_fTransformTimer -= fElapsedTime;

}