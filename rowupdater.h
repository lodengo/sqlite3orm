#pragma once

#include "TableSchema.h"
#include "sqlite3stmt.h"
#include "Col.h"
#include "Row.h"

#include <string>
using namespace std;

struct PKBinder
{
	PKBinder(sqlite3stmt* st, int& bindidx)
		:bindIndex(bindidx),pStmt(st)
	{

	}
	template<typename T>
	PKBinder& operator&(Col<T> & col)
	{
		if (col.PrimaryKey)
		{
			pStmt->bind(bindIndex++, col.ColValue);
		}
			
		return *this;
	}
	int bindIndex;
	sqlite3stmt* pStmt;
};

struct PKSetter
{
	PKSetter(sqlite_int64 rid)
		:pkid(rid)
	{

	}
	template<typename T>
	PKSetter& operator&(Col<T> & col)
	{
		if (col.PrimaryKey && col.AutoIncrement)
		{
			col.ColValue = (int)pkid; 
		}

		return *this;
	}
	sqlite_int64 pkid;
};

template<class T>
class RowUpdater
{
public:
	RowUpdater(const string& tablename, sqlite& sqli)
		:TableName(tablename), _sqlite(sqli)
	{

	}

	int Insert(Row<T>& row)
	{
		if (row.RowId != NullRowId)
		{
			return 0;
		}

		TableSchema<T> tableschem(row);

		string sql = "INSERT INTO " + TableName;

		sql += string("(") + RowIdColName + ",";

		for (size_t i = 0; i < tableschem.ColSchemas.size() - 1; i++)
		{
			sql += tableschem.ColSchemas[i].ColName + ",";
		}
		sql += tableschem.ColSchemas.back().ColName + ")";

		sql += " VALUES (NULL,";
		for (size_t i = 0; i < tableschem.ColSchemas.size() - 1; i++)
		{
			sql += "?,";
		}
		sql += "?)";

		sqlite3stmt st(_sqlite, sql.c_str());
		pStmt = &st;

		bindIndex = 1;		
		row.orm(*this);

		int ret = st.step();
		ret = _sqlite.commit();
		row.RowId = _sqlite.last_insert_rowid();
		return ret;
	}
	int Insert(T& row)
	{
		TableSchema<T> tableschem(row);
				
		string sql = "INSERT INTO " + TableName + "(";	
		
		for (size_t i = 0; i < tableschem.ColSchemas.size() - 1; i++)
		{
			sql += tableschem.ColSchemas[i].ColName + ",";
		}
		sql += tableschem.ColSchemas.back().ColName + ")";

		sql += " VALUES (";
		for (size_t i = 0; i < tableschem.ColSchemas.size() - 1; i++)
		{
			sql += "?,";
		}
		sql += "?)";

		sqlite3stmt st(_sqlite, sql);
		pStmt = &st;

		bindIndex = 1;		
		row.orm(*this);

		int ret = st.step();
		ret = _sqlite.commit();

		if (tableschem.isPKdefed())
		{
			PKSetter pkset(_sqlite.last_insert_rowid());
			row.orm(pkset);
		}
		
		return ret;
	}
	
	int Update(Row<T>& row)
	{
		if (row.RowId == NullRowId)
		{
			return 0;
		}

		TableSchema<T> tableschem(row);

		string sql = "UPDATE " + TableName + " SET ";

		for (size_t i = 0; i < tableschem.ColSchemas.size() - 1; i++)
		{
			sql += tableschem.ColSchemas[i].ColName + " = ?,";
		}
		sql += tableschem.ColSchemas.back().ColName + " = ?";

		sql += string(" WHERE ") + RowIdColName + " = ?";

		sqlite3stmt st(_sqlite, sql);
		pStmt = &st;

		bindIndex = 1;		
		row.orm(*this);
		st.bind(bindIndex++, row.RowId);

		int ret = st.step();
		ret = _sqlite.commit();
		return ret;
	}

	int Update(T& row)
	{
		TableSchema<T> tableschem(row);
		if (! tableschem.isPKdefed())
		{
			return -1;
		}

		string sql = "UPDATE " + TableName + " SET ";

		for (size_t i = 0; i < tableschem.ColSchemas.size() - 1; i++)
		{
			sql += tableschem.ColSchemas[i].ColName + " = ?,";
		}
		sql += tableschem.ColSchemas.back().ColName + " = ?";

		sql += string(" WHERE ");
		
		for (size_t i = 0; i < tableschem.ColSchemas.size(); i++)
		{
			if (tableschem.ColSchemas[i].PrimaryKey)
			{
				sql += tableschem.ColSchemas[i].ColName + " = ?";
				break;
			}
		}
		
		sqlite3stmt st(_sqlite, sql);
		pStmt = &st;

		bindIndex = 1;		
		row.orm(*this);
		
		PKBinder pkb(pStmt, bindIndex);
		row.orm(pkb);

		int ret = st.step();
		ret = _sqlite.commit();
		return ret;
	}

	int Delete(Row<T>& row)
	{
		if (row.RowId == NullRowId)
		{
			return 0;
		}

		string sql = "DELETE FROM " + TableName + " WHERE " + RowIdColName + " = ?";
		
		sqlite3stmt st(_sqlite, sql);

		int ret = st.bind(1, row.RowId);
		ret = st.step();

		ret = _sqlite.commit();
		return ret;		
	}
	int Delete(T& row)
	{
		TableSchema<T> tableschem(row);
		if (! tableschem.isPKdefed())
		{
			return -1;
		}

		string sql = "DELETE FROM " + TableName + " WHERE ";

		for (size_t i = 0; i < tableschem.ColSchemas.size(); i++)
		{
			if (tableschem.ColSchemas[i].PrimaryKey)
			{
				sql += tableschem.ColSchemas[i].ColName + " = ?";
				break;
			}
		}
		
		sqlite3stmt st(_sqlite, sql);
		
		bindIndex = 1;	
		PKBinder pkb(&st, bindIndex);
		row.orm(pkb);

		int ret = st.step();

		ret = _sqlite.commit();
		return ret;		
	}

	template<typename T>
	RowUpdater& operator&(Col<T> & col)
	{
		if (col.AutoIncrement || ColIsNull(col))
		{
			pStmt->bind(bindIndex++);
		}		
		else
		{
			pStmt->bind(bindIndex++, col.ColValue);
		}

		return *this;
	}
private:
	const string& TableName;
	sqlite& _sqlite;
	
	int bindIndex;
	sqlite3stmt* pStmt;
};
