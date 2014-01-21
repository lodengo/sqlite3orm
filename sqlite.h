#pragma once

#include <sqlite3.h>
#include "ResultTable.h"

class sqlite
{
public:
	sqlite(const string& filename)
	{
		open(filename);
	}
	sqlite(const string& filename, int flag)
	{
		open(filename, flag);
	}
	sqlite(sqlite3* psqlite)
	{
		_sqlite3 = psqlite;
	}
	~sqlite()
	{
		close();
	}
public:
	int close()
	{
		_ret = sqlite3_close(_sqlite3);
		return _ret;
	}

	int exec(string sql, string& errmsg)
	{
		char* err;
		_ret = sqlite3_exec(_sqlite3, sql.c_str(), NULL, NULL, &err);
		if (err != NULL)
		{
			errmsg = err;
		}
		sqlite3_free(err);
		return _ret;
	}

	int commit()
	{
		string err;
		return exec("COMMIT", err);
	}
	int rollback()
	{
		string err;
		return exec("ROLLBACK", err);
	}
	
	sqlite3_int64 last_insert_rowid()
	{
		return sqlite3_last_insert_rowid(_sqlite3);
	}

	int changes()
	{
		return sqlite3_changes(_sqlite3);
	}
	int total_changes()
	{
		return sqlite3_total_changes(_sqlite3);
	}

	void interrupt()
	{
		sqlite3_interrupt(_sqlite3);
	}

	int busy_handler(int(* hand)(void*,int), void* p)
	{
		_ret = sqlite3_busy_handler(_sqlite3, hand, p);
		return _ret;
	}
	int busy_timeout(int ms)
	{
		_ret = sqlite3_busy_timeout(_sqlite3, ms);
		return _ret;
	}

	int get_table(const string& sql, ResultTable& result, string& errmsg)
	{
		char** pazResult;
		int nrow, ncol;
		char* err;

		_ret = sqlite3_get_table(_sqlite3, sql.c_str(), &pazResult, &nrow, &ncol, &err);
		if (err != NULL)
		{
			errmsg = err;
		}
		result.Fill(pazResult, nrow, ncol);

		free_table(pazResult);
		sqlite3_free(err);

		return _ret;
	}
	void free_table(char **result)
	{
		sqlite3_free_table(result);
	}

	int open(const string& filename)
	{
		_ret = sqlite3_open(filename.c_str(), &_sqlite3);
		return _ret;
	}
	int open(const string& filename, int flag)
	{
		_ret = sqlite3_open_v2(filename.c_str(), &_sqlite3, flag, NULL);
		return _ret;
	}

	int errcode()
	{
		return sqlite3_errcode(_sqlite3);
	}
	string errmsg()
	{
		return sqlite3_errmsg(_sqlite3);
	}

	bool isAutocommit()
	{
		return sqlite3_get_autocommit(_sqlite3) != 0;
	}

	void* commit_hook(int(* hook)(void*), void* p)
	{
		return sqlite3_commit_hook(_sqlite3, hook, p);
	}
	void* rollback_hook(void(* hook)(void *), void* p)
	{
		return sqlite3_rollback_hook(_sqlite3, hook, p);
	}
	void* update_hook(void(* hook)(void *,int ,char const *,char const *,sqlite3_int64),
		void* p)
	{
		return sqlite3_update_hook(_sqlite3, hook, p);
	}

	int table_column_metadata(const string& dbName, const string& tbleName, const string& colName
		,string& dataType, string& CollSeq, bool& NotNull, bool& PrimaryKey, bool& Autoinc)
	{
		int notNull, pk, autoinc;
		const char* pDataType;
		const char* pCollSeq;

		_ret = sqlite3_table_column_metadata(_sqlite3, dbName.c_str(), tbleName.c_str(), colName.c_str(),
			&pDataType, &pCollSeq, &notNull, &pk, &autoinc);
		if (pDataType != NULL)
		{
			dataType = pDataType;
		}
		if (pCollSeq != NULL)
		{
			CollSeq = pCollSeq;
		}

		sqlite3_free((void*)pDataType);
		sqlite3_free((void*)pCollSeq);

		NotNull = notNull != 0;
		PrimaryKey = pk != 0;
		Autoinc = autoinc != 0;
		return _ret;
	}

public:
	sqlite3* get_sqlite3()
	{
		return _sqlite3;
	}
private:
	sqlite3* _sqlite3;
	int _ret;
};