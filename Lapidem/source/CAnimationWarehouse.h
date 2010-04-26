#ifndef CANIMATIONWAREHOUSE_H
#define CANIMATIONWAREHOUSE_H

#include <windows.h>
#include <vector>
using std::vector;

class CAnimation;

class CAnimationWarehouse
{
	static CAnimationWarehouse* AnimationWarehouse; // pointer to the instance of the class... only 1 should exist at a time
	vector<vector<CAnimation*>> m_vAnimationList;	// vector... of a vector... of animations. Animation set storing a bunch of Animations
	
	CAnimationWarehouse(){}							// singletoooooooonnnnn
	~CAnimationWarehouse() {DeleteInstance();}
	CAnimationWarehouse(CAnimationWarehouse& copy) {}
public:

	static CAnimationWarehouse* GetInstance(void)
	{
		if(AnimationWarehouse == NULL)
		{
			AnimationWarehouse = new CAnimationWarehouse;
		}
		return AnimationWarehouse;
	}
	
	void DeleteInstance(void)
	{
		if(AnimationWarehouse)
		{
			delete AnimationWarehouse;
			AnimationWarehouse = 0;
		}
	}
	void LoadAnimationSet(char* Filename, DWORD keycolor);

	CAnimation* GetAnimation(int nAnimationSet, int nAnimation);

	void Init(void);

};

#endif