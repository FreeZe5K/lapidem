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

class Corona_ObjectManager
{
	static Corona_ObjectManager * CoMReference;
	vector<CBase *> Objects;
	//Change from CBase to CTBase
	vector<CBase *> Terrain;

	//Singleton Encapsulation:
	Corona_ObjectManager() {};
	Corona_ObjectManager(const Corona_ObjectManager& copy);
	Corona_ObjectManager& operator=(const Corona_ObjectManager&);
	~Corona_ObjectManager() {};
	
	void CheckCollisions();
	void RemoveAllObjects(void);


/*
	Multithreading Tools:

	bool continue_thread;
	
	struct CollisionStruct
	{
		bool* continue_thread;
		vector<CBase*> * Objects;
		vector<CTBase*> * Terrain;
	};

	void * CheckCollision(void * CollisionStruct)
	{
		CollisionStruct * CS = &CollisionStruct

		while(*(CS->coninuethread))
		{

		for(Objects)
			for(objects)
				if(CheckCollision)
					resolvecollision

		for(terrain)
			for(objects)
			

		}
*/



public:

	static Corona_ObjectManager * GetInstance();
	void DeleteInstance();
	void UpdateObjects(float fElapsedTime);
	void RenderObjects(void);
	void AddObject(CBase* ObjectToAdd);
	void RemoveObject(CBase* ObjectToRemove);

};