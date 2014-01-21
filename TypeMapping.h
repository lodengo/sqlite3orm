#pragma once

#include "config.h"
#include <boost/type_traits.hpp> 

class TypeMapping
{
public:
	template<class T>
	static string coltype(T& t)
	{
		return enum_coltype(t, boost::is_enum<T>());
	}
	template<class T>
	static string enum_coltype(T& t, const boost::true_type&)
	{
		return "INTEGER";
	}
	template<class T>
	static string enum_coltype(T& t, const boost::false_type&)
	{
		return ClassColType;
	}
	static string coltype(int& t)
	{
		return "INTEGER";
	}
	static string coltype(double& t)
	{
		return "REAL";
	}
	static string coltype(bool& t)
	{
		return "INTEGER";
	}
	static string coltype(string& t)
	{
		return "TEXT";
	}
};