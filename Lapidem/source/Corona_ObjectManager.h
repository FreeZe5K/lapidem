//////////////////////////////////////////////////////////////
//  File Name   :   "Corona_ObjectManager.h"
//
//  Author Name :   Sam Mathis
//
//  Purpose     :   Manages all Game Objects.
//////////////////////////////////////////////////////////////
#pragma once
#include "CBase.h"
#include "MDTS/DynArray.h"


class Corona_ObjectManager
{
	unsigned Refs;
	static Corona_ObjectManager * CoMReference;
	DynArray<CBase *> Objects;
	//Change from CBase to CTBase
	DynArray<CBase *> Terrain;

	//Singleton Encapsulation:
	Corona_ObjectManager();
	Corona_ObjectManager(const Corona_ObjectManager& copy);
	Corona_ObjectManager& operator=(const Corona_ObjectManager&);
	~Corona_ObjectManager();
	

	void LoadObject(CBase*);
	//Change From CBase to CTBase
	void LoadTerrain(CBase*);

/*
	Multithreading Tools:

	bool continue_thread;
	
	struct CollisionThread
	{
		bool* continue_thread;
		DynArray<CBase*> * Objects;
		DynArray<CTBase*> * Terrain;
	};



*/



public:




};