//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CBase.cpp"
//
//  Author Name :   Jonathan Caro
//
//  Purpose     :   To encapsulate all related game 
//                  object data and functionality.
//////////////////////////////////////////////////////////////////////////
#include "CBase.h"
#include "Wrappers/CSGD_TextureManager.h" 

CBase::CBase( )
{
	SetPosX( 0 );
	SetPosY( 0 );
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
		CSGD_TextureManager::GetInstance( )->Draw( GetImage( ), ( int )GetPosX( ), ( int )GetPosY( ) );
}

RECT CBase::GetCollisionRect( )
{
	RECT tempRect = { ( LONG )GetPosX( ),( LONG )GetPosY( ), 
		( LONG )GetPosX( ) - GetWidth( ),( LONG )GetPosY() + GetHeight( ) };
	
	return tempRect;
}

bool CBase::CheckCollision( CBase* pBase )
{
	if( IntersectRect( 0, &GetCollisionRect( ), &pBase->GetCollisionRect( ) ) )
		return true;
	else return false;
}