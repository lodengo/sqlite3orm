#pragma once

#include <string>
using std::string;

struct ColSchema
{
	ColSchema(const string& colname, const string& coldef)
		:ColName(colname), ColDef(coldef)
	{

	}
	
	ColSchema(const string& colname, const string& coltype, bool notnull, bool pk, bool autoinc)
		:ColName(colname), ColType(coltype),NotNull(notnull),PrimaryKey(pk),AutoIncrement(autoinc)
	{

	}
	
	string sql()
	{
		if (! ColDef.empty())
		{
			return ColName + " " + ColDef;
		}
		string ret = ColName + " " + ColType; 		
		ret += PrimaryKey ? " PRIMARY KEY " : "";
		ret += AutoIncrement ? " AUTOINCREMENT " : "";
		ret += NotNull ? " NOT NULL " : "";

		return ret;
	}

	void parseColDef(const string& def)
	{
		PrimaryKey = def.find("PRIMARY") != -1;
		AutoIncrement = def.find("AUTOINCREMENT") != -1;
		NotNull = def.find("NOT NULL") != -1;
		Unique = def.find("UNIQUE") != -1;
		HasDefault =def.find("DEFAULT") != -1;
	}

	string ColName;
	string ColDef;

	string ColType;	
	bool PrimaryKey;
	bool AutoIncrement;	
	bool NotNull;
	bool Unique;	
	bool HasDefault;
};