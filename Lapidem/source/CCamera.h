#pragma once

class CBase;

class CCamera
{

	static CCamera * theCamera;

	float m_fPosX;
	float m_fPosY;

	float m_fWidth;
	float m_fHeight;

	float m_fVelocityX;
	float m_fVelocityY;

	CCamera() {};
	CCamera(const CCamera &) {};
	CCamera & operator=(const CCamera &) {};
	~CCamera() {};

	void SetVelocityX(float fVelocityX)				{m_fVelocityX = fVelocityX;}
	void SetVelocityY(float fVelocityY)				{m_fVelocityY = fVelocityY;}
	void SetCameraXOffset(float fPosX)				{m_fPosX = fPosX;}
	void SetCameraYOffset(float fPosY)				{m_fPosY = fPosY;}
	void SetCameraWidth(float fWidth)				{m_fWidth = fWidth;}
	void SetCameraHeight(float fHeight)				{m_fHeight = fHeight;}

	CBase * thePlayer;


public:

	//Creates the Camera.
	static void InitCamera(float, float, float, float, CBase*);
	void ShutDownCamera();

	//Updates the Camera's Place and hooks
	void Update(float fElapsedTime);

	//Accessors
	static CCamera* GetCamera()		 {return theCamera;}
	float GetVelocityX()			 {return m_fVelocityX; }
	float GetVelocityY()			 {return m_fVelocityY; }
	float GetXOffset()				 {return m_fPosX;}
	float GetYOffset()				 {return m_fPosY;}
	float GetWidth()				 {return m_fPosX + m_fWidth;}
	float GetHeight()				 {return m_fPosY + m_fHeight;}
	
	






};