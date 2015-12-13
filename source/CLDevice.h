#pragma once
#include "CEvent.h"

class CLDevice
{

public:

	virtual void HandleEvent(CEvent* pEvent) = 0;


};