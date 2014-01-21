#pragma once

#include "sqlite.h"
#include "TableSchema.h"
#include "RowUpdater.h"
#include "RowLoader.h"
#include "Table.h"
#include "Col.h"

#include <list>
#include <set>
#include <map>

class Sqlite3x
{
public:
	Sqlite3x(const string& filename)
		:_sqlite(filename)
	{

	}
	~Sqlite3x()
	{

	}
public:
	int exec(const string& sql, string& errmsg)
	{
		_ret = _sqlite.exec(sql, errmsg);
		return _ret;
	}
	int commit()
	{
		_ret = _sqlite.commit();
		return _ret;
	}
	int rollback()
	{
		_ret = _sqlite.rollback();
		return _ret;
	}
	int get_table(const string& sql, ResultTable& result, string& errmsg)
	{
		_ret = _sqlite.get_table(sql, result,errmsg);
		return _ret;
	}
public:
	template<class T>
	int createTable(const string& tablename, TableSchema<T>& tableschema)
	{
		string err;
		string sql = string("CREATE TABLE ") + tablename + tableschema.sql();
		_ret = exec(sql.c_str(), err);
		return _ret;
	}
public:
	int select(const string& sql, ResultTable& result)
	{
		string err;
		return get_table(sql, result, err);
	}
	template<class T>
	int select(const string& sql, T& ret)
	{
		sqlite3stmt st(_sqlite, sql);
		if (SQLITE_ROW == st.step())
		{
			TLoader l(&st);
			ret.orm(l);
		}
		_ret = st.how();
		return _ret;
	}
	int select(const string& sql, int& ret)
	{
		sqlite3stmt st(_sqlite, sql);
		if (SQLITE_ROW == st.step())
		{
			ret = st.columnInt(0);
		}
		_ret = st.how();
		return _ret;
	}
	int select(const string& sql, double& ret)
	{
		sqlite3stmt st(_sqlite, sql);
		if (SQLITE_ROW == st.step())
		{
			ret = st.columnDouble(0);
		}
		_ret = st.how();
		return _ret;
	}
	int select(const string& sql, bool& ret)
	{
		sqlite3stmt st(_sqlite, sql);
		if (SQLITE_ROW == st.step())
		{
			//ret = st.columnBool(0);
		}
		_ret = st.how();
		return _ret;
	}
	int select(const string& sql, string& ret)
	{
		sqlite3stmt st(_sqlite, sql);
		if (SQLITE_ROW == st.step())
		{
			ret = st.columnText(0);
		}
		_ret = st.how();
		return _ret;
	}
	//////////////////////////////////////////////////////////////////////////
	template<typename T, class A>
	int select(const string& sql, vector<T,A>& ret, int nrec = -1)
	{
		ret.clear();

		sqlite3stmt st(_sqlite, sql);
		TLoader l(&st);
		int i = 0;
		while (SQLITE_ROW == st.step() && i++ != nrec)
		{
			T e;
			e.orm(l);//st.column(0, e);
			ret.push_back(e);			
		}
		_ret = st.how();
		return _ret;
	}
	template</*typename T,*/ class A>
	int select(const string& sql, vector<int,A>& ret, int nrec = -1)
	{
		ret.clear();

		sqlite3stmt st(_sqlite, sql);
		TLoader l(&st);
		int i = 0;
		while (SQLITE_ROW == st.step() && i++ != nrec)
		{
			int e;
			st.column(0, e);
			ret.push_back(e);
		}
		_ret = st.how();
		return _ret;
	}
	template</*typename T,*/ class A>
	int select(const string& sql, vector<double,A>& ret, int nrec = -1)
	{
		ret.clear();

		sqlite3stmt st(_sqlite, sql);
		TLoader l(&st);
		int i = 0;
		while (SQLITE_ROW == st.step()  && i++ != nrec)
		{
			double e;
			st.column(0, e);
			ret.push_back(e);
		}
		_ret = st.how();
		return _ret;
	}
	template</*typename T,*/ class A>
	int select(const string& sql, vector<std::string,A>& ret, int nrec = -1)
	{
		ret.clear();

		sqlite3stmt st(_sqlite, sql);
		TLoader l(&st);
		int i = 0;
		while (SQLITE_ROW == st.step()  && i++ != nrec)
		{
			std::string e;
			st.column(0, e);
			ret.push_back(e);
		}
		_ret = st.how();
		return _ret;
	}
	template</*typename T,*/ class A>
	int select(const string& sql, vector<bool,A>& ret, int nrec = -1)
	{
		ret.clear();

		sqlite3stmt st(_sqlite, sql);
		TLoader l(&st);
		int i = 0;
		while (SQLITE_ROW == st.step()  && i++ != nrec)
		{
			bool e;
			st.column(0, e);
			ret.push_back(e);
		}
		_ret = st.how();
		return _ret;
	}
	template<typename T, class A>
	int select(const string& sql, vector<T*,A>& ret, int nrec = -1)
	{
		ret.clear();

		sqlite3stmt st(_sqlite, sql);
		TLoader l(&st);
		int i = 0;
		while (SQLITE_ROW == st.step()  && i++ != nrec)
		{
			T* e = new T;
			e->orm(l);//st.column(0, e);
			ret.push_back(e);
		}
		_ret = st.how();
		return _ret;
	}
	template<typename T, class A>
	int select(const string& sql, list<T,A>& ret, int nrec = -1)
	{
		ret.clear();

		sqlite3stmt st(_sqlite, sql);
		int i = 0;
		while (SQLITE_ROW == st.step()  && i++ != nrec)
		{
			T e;
			st.column(0, e);
			ret.push_back(e);
		}
		_ret = st.how();
		return _ret;
	}
	template<typename T, class C, class A>
	int select(const string& sql, set<T,C,A>& ret, int nrec = -1)
	{
		ret.clear();

		sqlite3stmt st(_sqlite, sql);
		int i = 0;
		while (SQLITE_ROW == st.step()  && i++ != nrec)
		{
			T e;
			st.column(0, e);
			ret.insert(e);
		}
		_ret = st.how();
		return _ret;
	}
	template<typename T1, typename T2, class A>
	int select(const string& sql, std::vector<std::pair<T1,T2>,A>& ret, int nrec = -1)
	{
		ret.clear();

		sqlite3stmt st(_sqlite, sql);
		TLoader l(&st);
		int i = 0;
		while (SQLITE_ROW == st.step()  && i++ != nrec)
		{			
			T1 t1;
			st.column(0, t1);
			T2 t2;
			st.column(1, t2);
			ret.push_back(std::pair<T1,T2>(t1,t2));
		}
		_ret = st.how();
		return _ret;
	}
	template<typename T1, typename T2>
	int select(const string& sql, std::pair<T1,T2>& ret)
	{
		sqlite3stmt st(_sqlite, sql);
		if (SQLITE_ROW == st.step())
		{		
			st.column(0, ret.first);
			st.column(1, ret.second);			
		}
		_ret = st.how();
		return _ret;
	}
	template<typename K, typename T, class C, class A>
	int select(const string& sql, map<K,T,C,A>& ret, int nrec = -1)
	{
		//ret.clear();

		sqlite3stmt st(_sqlite, sql);
		int i = 0;
		while (SQLITE_ROW == st.step()  && i++ != nrec)
		{
			K k;
			T t;
			st.column(0, k);
			st.column(1, t);

			ret.insert(std::pair<K,T>(k,t));
		}
		_ret = st.how();
		return _ret;
	}
	template<typename K, typename T, class C, class A>
	int select(const string& sql, multimap<K,T,C,A>& ret, int nrec = -1)
	{
		ret.clear();

		sqlite3stmt st(_sqlite, sql);
		int i = 0;
		while (SQLITE_ROW == st.step()  && i++ != nrec)
		{
			K k;
			T t;
			st.column(0, k);
			st.column(1, t);

			ret.insert(pair<K,T>(k,t));
		}
		_ret = st.how();
		return _ret;
	}
	//////////////////////////////////////////////////////////////////////////
	template<class T>
	int select(const string& sql, Row<T>& row)
	{
		return RowLoader<T>(_sqlite, sql).Load(row);
	}

	template<typename T, class A>
	int select(const string& sql, vector<Row<T>,A>& rows)
	{
		return RowLoader<T>(_sqlite, sql).Load(rows);
	}
	//////////////////////////////////////////////////////////////////////////
	template<class T>
	int select(const string& sql, Table<T>& table)
	{
		return table.Select(sql);
	}

	template<class T>
	int select(Table<T>& table)
	{
		return table.SelectAll();
	}
	//////////////////////////////////////////////////////////////////////////
	template<class T>
	int insert(const string& tablename, Row<T>& row)
	{
		return RowUpdater<T>(tablename, _sqlite).Insert(row);
	}
	template<class T>
	int insert(const string& tablename, T& t)
	{
		return RowUpdater<T>(tablename, _sqlite).Insert(t);
	}
	//////////////////////////////////////////////////////////////////////////
	template<class T>
	int update(const string& tablename, Row<T>& row)
	{
		return RowUpdater<T>(tablename, _sqlite).Update(row);
	}

	template<class T>
	int update(const string& tablename, T& t)
	{
		return RowUpdater<T>(tablename, _sqlite).Update(t);
	}
	//////////////////////////////////////////////////////////////////////////
	template<class T>
	int Delete(const string& tablename, Row<T>& row)
	{
		return RowUpdater<T>(tablename, _sqlite).Delete(row);
	}
	template<class T>
	int Delete(const string& tablename, T& t)
	{
		return RowUpdater<T>(tablename, _sqlite).Delete(t);
	}
public:
	sqlite& get_sqlite()
	{
		return _sqlite;
	}
private:
	sqlite _sqlite;
	int _ret;	
};


