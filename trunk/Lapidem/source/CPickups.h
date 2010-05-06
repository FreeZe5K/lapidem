#pragma once
#include "CBase.h"


class CPickup : public CBase
{
	int EleType;
public:
	void SetEleType(int Eletype) {this->EleType = Eletype;}
	int	 GetEleType()			  {return EleType;}

};