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
	DynArray<CBase *> Terrain;

	//Singleton Encapsulation:
	Corona_ObjectManager();
	Corona_ObjectManager(const Corona_ObjectManager& copy);
	Corona_ObjectManager& operator=(const Corona_ObjectManager&);
	~Corona_ObjectManager();
	

	void LoadObject(CBase*);
	//Change From CBase to CTBase
	void LoadTerrain(CBase*);
	



public:




};