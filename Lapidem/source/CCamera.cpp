#include "CCamera.h"
#include "CBase.h"


#define XHOOK_MIN_DISTANCE (int)theCamera->GetPlayerXhookDistance() >> 1
#define YHOOK_MIN_DISTANCE (int)theCamera->GetPlayerXhookDistance() >> 1

CCamera * CCamera::theCamera = 0;

void CCamera::InitCamera(float fPosX, float fPosY, float fWidth, float fHeight, float fXHook, float fYHook, CBase* ObjectToFollow)
{
	if(!theCamera)
	{
		theCamera = new CCamera();
		theCamera->SetCameraXOffset(fPosX);
		theCamera->SetCameraYOffset(fPosY);
		theCamera->SetCameraWidth(fWidth);
		theCamera->SetCameraHeight(fHeight);
		theCamera->SetPlayerHooks(fXHook, fYHook);
		theCamera->thePlayer = ObjectToFollow;
		theCamera->SetVelocityX(0.0f);
		theCamera->SetVelocityY(0.0f);
	}
}

void CCamera::ShutDownCamera()
{
	if(theCamera)
		delete theCamera;
}

void CCamera::Update(float fElapsedTime)
{
		float DistanceX = thePlayer->GetPosX() - theCamera->GetXOffset(); 
		float DistanceY = thePlayer->GetPosY() - theCamera->GetYOffset();

		if(DistanceX > theCamera->GetPlayerXhookDistance() || DistanceX < XHOOK_MIN_DISTANCE)
			theCamera->SetVelocityX(thePlayer->GetVelX());
		else
			theCamera->SetVelocityX(0.0f);

		if(DistanceY > theCamera->GetPlayerXhookDistance() || DistanceY < YHOOK_MIN_DISTANCE)
			theCamera->SetVelocityY(thePlayer->GetVelY());
		else
			theCamera->SetVelocityY(0.0f);

		theCamera->SetCameraXOffset(theCamera->GetXOffset() + theCamera->GetVelocityX() * fElapsedTime);
		theCamera->SetCameraYOffset(theCamera->GetYOffset() + theCamera->GetVelocityY() * fElapsedTime);
	
	
}