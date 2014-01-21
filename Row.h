#pragma once

#include "config.h"

template<class T>
struct Row : public T
{
	Row()
		:RowId(NullRowId)
	{
		
	}

	Row(const T& t)
		:T(t),RowId(NullRowId)
	{

	}

	TRowId RowId; 
};