#pragma once

#include "sqlite.h"

#include <boost/type_traits.hpp> 
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>

class sqlite3stmt
{
public:
	sqlite3stmt(sqlite3_stmt* pst)
	{
		_pStmt = pst;
	}
	sqlite3stmt(sqlite3 *db, const string& sql)
	{
		prepare(db, sql);
	}
	sqlite3stmt(sqlite& _sqlite, const string& sql)
	{
		prepare(_sqlite.get_sqlite3(), sql);
	}
	~sqlite3stmt()
	{
		finalize();
	}
public:
	int prepare(sqlite3* db, const string& sql)
	{
		_ret = sqlite3_prepare_v2(db, sql.c_str(), -1, &_pStmt, NULL);
		return _ret;
	}

	string sql()
	{
		return sqlite3_sql(_pStmt);
	}

	int bind(int iParam, double val)
	{
		_ret = sqlite3_bind_double(_pStmt, iParam, val);
		return _ret;
	}
	int bind(int iParam, int val)
	{
		_ret = sqlite3_bind_int(_pStmt, iParam, val);
		return _ret;
	}
	int bind(int iParam, bool val)
	{
		_ret = sqlite3_bind_int(_pStmt, iParam, val ? 1 : 0);
		return _ret;
	}
	int bind(int iParam, const string& val)
	{
		_ret = sqlite3_bind_text(_pStmt, iParam,val.c_str(),-1, SQLITE_TRANSIENT);
		return _ret;
	}

	int bind(int iParam, const void* blob)
	{
		_ret = sqlite3_bind_blob(_pStmt, iParam, blob, -1, SQLITE_TRANSIENT);
		return _ret;
	}
	int bind(int iParam, sqlite_int64 val)
	{
		_ret = sqlite3_bind_int64(_pStmt, iParam, val);
		return _ret;
	}
	int bind(int iParam)
	{
		_ret = sqlite3_bind_null(_pStmt, iParam);
		return _ret;
	}
	int bind(int iParam, const sqlite3_value* val)
	{
		_ret = sqlite3_bind_value(_pStmt, iParam, val);
		return _ret;
	}

	template<class T>
	int bind(int iParam, const T& val)
	{
		return bind_enum(iParam, val, boost::is_enum<T>());
	}

	int parameterCount()
	{
		return sqlite3_bind_parameter_count(_pStmt);
	}
	string parameterName(int iParam)
	{
		return sqlite3_bind_parameter_name(_pStmt, iParam);
	}
	int parameterIndex(const string& paramName)
	{
		return sqlite3_bind_parameter_index(_pStmt, paramName.c_str());
	}
	int clearBindings()
	{
		_ret = sqlite3_clear_bindings(_pStmt);
		return _ret;
	}

	int columnCount()
	{
		return sqlite3_column_count(_pStmt);
	}
	string columnName(int iCol)
	{
		return sqlite3_column_name(_pStmt, iCol);
	}
	string columnDecltype(int iCol)
	{
		return sqlite3_column_decltype(_pStmt, iCol);
	}
	
	string column_database_name(int iCol)
	{
		return sqlite3_column_database_name(_pStmt, iCol);
	}
	string column_table_name(int iCol)
	{
		return sqlite3_column_table_name(_pStmt, iCol);
	}
	string column_origin_name(int iCol)
	{
		return sqlite3_column_origin_name(_pStmt, iCol);
	}

	int step()
	{
		_ret = sqlite3_step(_pStmt);
		return _ret;
	}

	int dataCount()
	{
		return sqlite3_data_count(_pStmt);
	}

	const void* columnBlob(int iCol)
	{
		return sqlite3_column_blob(_pStmt, iCol);
	}
	double columnDouble(int iCol)
	{
		return sqlite3_column_double(_pStmt, iCol);
	}
	int columnInt(int iCol)
	{
		return sqlite3_column_int(_pStmt, iCol);
	}
	sqlite3_int64 columnInt64(int iCol)
	{
		return sqlite3_column_int64(_pStmt, iCol);
	}
	string columnText(int iCol)
	{
		const char* p = (const char*)sqlite3_column_text(_pStmt, iCol);
		return p == NULL ? "" : p;
	}

	int columnType(int iCol)
	{
		return sqlite3_column_type(_pStmt, iCol);
	}

	int finalize()
	{
		_ret = sqlite3_finalize(_pStmt);
		return _ret;
	}
	int reset()
	{
		_ret = sqlite3_reset(_pStmt);
		return _ret;
	}

	sqlite3* db_handle()
	{
		return sqlite3_db_handle(_pStmt);
	}
	sqlite3_stmt* next_stmt()
	{
		return sqlite3_next_stmt(db_handle(), _pStmt);
	}
public:
	void column(int iCol, double& value)
	{
		value = columnDouble(iCol);
	}
	void column(int iCol, int& value)
	{
		value = columnInt(iCol);
	}
	void column(int iCol, long& value)
	{
		value = columnInt(iCol);
	}
	void column(int iCol, bool& value)
	{
		value = columnInt(iCol) != 0;
	}
	void column(int iCol, string& value)
	{
		value = columnText(iCol);
	}
	void column(int iCol, sqlite3_int64& value)
	{
		value = columnInt64(iCol);
	}

	template<class T>
	void column(int iCol, T& value)
	{
		column_enum(iCol, value, boost::is_enum<T>());
	}

	int columnIndex(const string& colname)
	{
		int n = columnCount();
		for (int i = 0; i < n; i++)
		{
			if (colname == columnName(i))
			{
				return i;
			}
		}
		return -1;
	}
public:	
	int how()
	{
		return _ret;
	}
private:
	template<class T>
	int bind_enum(int iParam, const T& val, const boost::true_type&)
	{
		return bind(iParam, (int)val);
	}
	template<class T>
	int bind_enum(int iParam, const T& val, const boost::false_type&)
	{
		std::stringstream ss;
		boost::archive::text_oarchive oa(ss);
		oa << val;

		return bind(iParam, ss.str());
	}
	template<class T>
	void column_enum(int iCol, T& value, const boost::true_type&)
	{
		int i;
		column(iCol, i);
		value = (T)i;
	}
	template<class T>
	void column_enum(int iCol, T& value, const boost::false_type&)
	{
		string s;
		column(iCol, s);
		std::stringstream ss;
		ss.str(s);
		boost::archive::text_iarchive ia(ss);	
		ia >> value;
	}

private:
	sqlite3_stmt* _pStmt;
	int _ret;
};
