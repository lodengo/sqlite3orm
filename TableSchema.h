#pragma once

#include "ColSchema.h"
#include "config.h"

template<class T>
struct TableSchema
{
	//TableSchema(){}
	TableSchema(T& t)
	{
		t.orm(*this);
	}
	
	string sql()
	{
		string ret = "(";

		if (!isPKdefed())
		{
			ColSchema RowIdCol(RowIdColName, RowIdType, true, true, true);
			ret += RowIdCol.sql() + ",";
		}
		
		for (size_t i = 0; i < ColSchemas.size() - 1; i++)
		{
			ret += ColSchemas[i].sql() + ",";
		}
		ret += ColSchemas.back().sql() + ")";

		return ret;
	}
	
	TableSchema& operator&(ColSchema & colschema)
	{
		ColSchemas.push_back(colschema);
		return *this;
	}

	bool isPKdefed()
	{
		for (size_t i = 0; i < ColSchemas.size(); i++)
		{
			if (ColSchemas[i].PrimaryKey)
			{
				return true;
			}
		}
		return false;
	}

	vector<ColSchema> ColSchemas;
};
