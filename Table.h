#pragma once


#include "RowLoader.h"
#include "RowUpdater.h"

template<class T>
class Table
{
public:
	Table(sqlite& sqli, const string& tablename)
		:_sqlite(sqli),_tableName(tablename)
	{

	}
public:
	template<class T>
	int SelectAll()
	{
		string sql = "select * from" + _tableName;
		return RowLoader<T>(_sqlite, sql).Load(_rows);
	}

	template<class T>
	int Select(const string& sql)
	{
		return RowLoader<T>(_sqlite, sql).Load(_rows);
	}

	template<class T>
	int UpdateRow(int iRow)
	{
		return RowUpdater<T>(_tableName, _sqlite).Update(_rows[iRow]);
	}

	template<class T>
	int DeleteRow(int iRow)
	{
		int ret = RowUpdater<T>(_tableName, _sqlite).Delete(_rows[iRow]);
		_rows.erase(_rows.begin() + iRow);
		return ret;
	}

	template<class T>
	int InsertRow(Row<T>& row)
	{
		int ret = RowUpdater<T>(_tableName, _sqlite).Insert(row);
		_rows.push_back(row);
		return ret;
	}

	template<class T>
	size_t RowCount()
	{
		return _rows.size();
	}

	template<class T>
	Row<T>& operator[](int iRow)
	{
		return _rows[iRow];
	}
private:
	sqlite& _sqlite;
	string _tableName;
	vector< Row<T> > _rows;
};
//////////////////////////////////////////////////////////////////////////
