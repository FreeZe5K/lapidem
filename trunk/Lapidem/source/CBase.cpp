//////////////////////////////////////////////////////////////////////////
//  File Name   :   "CBase.cpp"
//
//  Author Name :   Jonathan Caro
//
//  Purpose     :   To encapsulate all related game 
//                  object data and functionality.
//////////////////////////////////////////////////////////////////////////
#include "CBase.h"
#include "CGame.h"
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
	{
		CSGD_TextureManager::GetInstance( )->Draw( GetImage( ), 
			int( GetPosX( ) - CCamera::GetCamera( )->GetXOffset( ) ),
			int( GetPosY( ) - CCamera::GetCamera( )->GetYOffset( ) ) );
	}

	if( CGame::GetInstance( )->GetDebugMode( ) )
	{
		CSGD_Direct3D::GetInstance( )->DrawLine( DebugCollisionRect( ).left, 
			DebugCollisionRect( ).top, DebugCollisionRect( ).left, 
			DebugCollisionRect( ).bottom, 255, 0, 0 );

		CSGD_Direct3D::GetInstance( )->DrawLine( DebugCollisionRect( ).left, 
			DebugCollisionRect( ).top, DebugCollisionRect( ).right, 
			DebugCollisionRect( ).top, 255, 0, 0 );

		CSGD_Direct3D::GetInstance( )->DrawLine( DebugCollisionRect( ).right, 
			DebugCollisionRect( ).top, DebugCollisionRect( ).right, 
			DebugCollisionRect( ).bottom, 255, 0, 0 );

		CSGD_Direct3D::GetInstance( )->DrawLine( DebugCollisionRect( ).right, 
			DebugCollisionRect( ).bottom, DebugCollisionRect( ).left, 
			DebugCollisionRect( ).bottom, 255, 0, 0 );
	}
}

RECT CBase::DebugCollisionRect( )
{
	RECT rBuffer;
	SetRect( &rBuffer, int( GetPosX( ) + 5 ), int( GetPosY( ) ), 
		int( GetPosX( ) + GetWidth( ) + 5 ), int( GetPosY( ) + GetHeight( ) + 5 ) );
	return rBuffer;
}

RECT CBase::GetCollisionRect( float fElapsedTime )
{
	RECT tempRect = { 
		LONG( GetPosX( ) + GetVelX( ) * fElapsedTime ),
		LONG( GetPosY( ) + GetVelY( ) * fElapsedTime ) , 
		LONG( GetPosX( ) + GetVelX( ) * fElapsedTime ) + GetWidth( ),
		LONG( GetPosY( ) + GetVelY( ) * fElapsedTime ) + GetHeight( ) };

		return tempRect;
}

bool CBase::CheckCollision( float fElapsedTime, CBase* pBase )
{
	RECT tempRect;

	if( IntersectRect( &tempRect, &GetCollisionRect( fElapsedTime ), 
		&pBase->GetCollisionRect( fElapsedTime ) ) )
		return true;
	return false;
}

void CBase::HandleCollision( CBase* _base )
{
}

void CBase::MoveOutOf( CBase* pSolidObject )
{
	RECT r;
	IntersectRect( &r, &this->GetCollisionRect( 0.0167f ), 
		&pSolidObject->GetCollisionRect( 0.0167f ) );

	int nRectWidth( r.right -r.left );
	int nRectHeight( r.bottom - r.top );

	if( nRectHeight > nRectWidth )
	{
		if( this->GetPosX( ) > pSolidObject->GetPosX( ) )
			SetPosX( GetPosX( ) + nRectWidth );
		if( this->GetPosX( ) < pSolidObject->GetPosX( ) )
			SetPosX( GetPosX( ) - nRectWidth );
	}
	else if( nRectHeight < nRectWidth ) 
	{			
		if( this->GetPosY( ) > pSolidObject->GetPosY( ) )
			SetPosY( GetPosY( ) + nRectHeight );
		if( this->GetPosY( ) < pSolidObject->GetPosY( ) )
			SetPosY( GetPosY( ) - nRectHeight );
	}
}

void CBase::ClampToScreen()
{
	CCamera* tempCam = CCamera::GetCamera();

	if(GetPosX() < tempCam->GetXOffset())
		SetPosX(tempCam->GetXOffset());
	else if(GetPosX() + GetWidth() > tempCam->GetWidth())
		SetPosX(tempCam->GetWidth() - GetWidth());


	if(GetPosY() < tempCam->GetYOffset())
		SetPosY(tempCam->GetYOffset());
	else if(GetPosY() + GetHeight() > tempCam->GetHeight())
		SetPosY(tempCam->GetHeight() - GetHeight());

}