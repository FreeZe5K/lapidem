//////////////////////////////////////////////////////////////
//  File Name   :   "Corona_ObjectManager.h"
//
//  Author Name :   Sam Mathis
//
//  Purpose     :   Manages all Game Objects.
//////////////////////////////////////////////////////////////
#pragma once
#include <vector>
using std::vector;

class CBase;
class CCamera;

struct Object_Type
{
	void* Item;
	int eletype;
	int   number;
	int	  Type;
};

class Corona_ObjectManager
{
private:
	static Corona_ObjectManager * CoMReference;
	float m_fTimer;

	vector<CBase *> Objects;
	vector<Object_Type> CrashHunter;
	int Number;

	//Change from CBase to CTBase
	//vector<CBase *> Terrain;

	Corona_ObjectManager( )   { m_fTimer = 0.0f; }
	~Corona_ObjectManager( )  { /* DO NOTHING */ };
	Corona_ObjectManager( const Corona_ObjectManager& );
	Corona_ObjectManager& operator=( const Corona_ObjectManager& );
	
	void CheckCollisions( float );

	CCamera * theCamera;

public:
	static Corona_ObjectManager* GetInstance( );

	void NullCamera() { theCamera = NULL;}
	void SetCamera(CCamera* pCam) { theCamera = pCam; }
	void DeleteInstance( );
	void UpdateObjects( float );
	void RenderObjects( );
	bool IsOnScreen( CBase* );
	void AddObject( CBase* );
	void RemoveObject( CBase* );
	void RemoveAllObjects( );
	void AuxFunction(void (*pFunc)(CBase* object, CBase* object2), int OBJ_TYPE, bool AllInManager, CBase* pSpell);

};