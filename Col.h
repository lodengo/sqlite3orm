#pragma once

#include "ColSchema.h"
#include "TypeMapping.h"

template<typename T>
struct Col : public ColSchema
{
	Col(const string& colname, T& colvalue, const string& coldef)
		:ColSchema(colname, coldef),ColValue(colvalue)
	{

	}
	Col(const string& colname, T& colvalue)
		:ColSchema(colname, TypeMapping::coltype(colvalue), false,false,false),ColValue(colvalue)
	{

	}
	Col(const string& colname, T& colvalue,bool notnull, bool pk, bool autoinc)
		:ColSchema(colname, TypeMapping::coltype(colvalue), notnull, pk, autoinc),ColValue(colvalue)
	{

	}

	T& ColValue;
};

template<typename T> inline
bool ColIsNull(Col<T>& col)
{
	return false;
}

template<> inline
bool ColIsNull(Col<string>& col)
{
	return col.ColValue.empty();
}

template<class T> inline
Col<T> make_col(const char * name, T & t, const char* def)
{
	return Col<T>(name, t, def);
}
template<class T> inline
Col<T> make_col(const char * name, T & t)
{
	return Col<T>(name, t);
}
template<class T> inline
Col<T> make_col(const char * name, T & t, bool nonull, bool pk, bool ainc)
{
	return Col<T>(name, t, nonull, pk, ainc);
}

#define ORM_COL(name)                              \
	make_col(#name, name)

#define ORM_COLDEF(name, def)                              \
	make_col(#name, name, def)

#define ORM_AINCPK(name)\
	make_col(#name, name, true, true, true)

#define ORM_PK(name)\
	make_col(#name, name, true, true, false)