//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CBase.cpp"
//
//  Author Name :   Jonathan Caro
//
//  Purpose     :   To encapsulate all related game 
//                  object data and functionality.
//////////////////////////////////////////////////////////////////////////
#include "CBase.h"
#include "CCamera.h"
#include "Wrappers/CSGD_TextureManager.h" 

CBase::CBase( )
{
	SetPosX( 0 );
	SetPosY( 0 );
	SetWidth( 0 );
	SetHeight( 0 );
	SetVelX( 0 );
	SetVelY( 0 );
	SetImage( -1 );
	SetSound( -1 );

	m_nRefCount = 1;
}

CBase::~CBase( )
{
}

void CBase::Update( float fElapsedTime )
{
	SetPosX( GetPosX( ) + GetVelX( ) * fElapsedTime );
	SetPosY( GetPosY( ) + GetVelY( ) * fElapsedTime );
}

void CBase::Render( )
{
	if( GetImage( ) != -1 )
		CSGD_TextureManager::GetInstance( )->Draw( GetImage( ), ( int )(GetPosX( ) - CCamera::GetCamera()->GetXOffset()), ( int )(GetPosY( ) - CCamera::GetCamera()->GetYOffset()) );
}

RECT CBase::GetCollisionRect(float fElapsedTime)
{
	RECT tempRect = { ( LONG )(GetPosX( ) + GetVelX( ) * fElapsedTime),
					  ( LONG )(GetPosY( ) + GetVelY( ) * fElapsedTime), 
					  ( LONG )(GetPosX( ) + GetVelX( ) * fElapsedTime) + GetWidth( ),
					  ( LONG )(GetPosY( ) + GetVelY( ) * fElapsedTime) + GetHeight( )};
	
	return tempRect;
}

bool CBase::CheckCollision(float fElapsedTime, CBase* pBase )
{
	RECT tempRect;

	if( IntersectRect( &tempRect, &GetCollisionRect(fElapsedTime), &pBase->GetCollisionRect(fElapsedTime) ) )
		return true;
	
	return false;
}

void CBase::HandleCollision(CBase*)
{


}