#pragma once

#include "CEvent.h"

class IListener
{
public:
	IListener( )          { /* DO NOTHING HERE */ }
	virtual ~IListener( ) { /* DO NOTHING HERE */ }

	virtual void HandleEvent( CEvent* ) = 0;
};