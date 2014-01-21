#pragma once

template<class T>
class RowLoader
{
public:
	RowLoader(sqlite& sqli, const string& _sql)
		:_sqlite(sqli),sql(_sql)
	{

	}
	
	int Load(Row<T>& row)
	{
		sqlite3stmt st(_sqlite, sql);
		pStmt = &st;
		if (SQLITE_ROW == st.step())
		{
			this->operator & (make_col(RowIdColName, row.RowId));
			row.orm(*this);
		}

		return st.how();
	}

	
	int Load(vector< Row<T> >& rows)
	{
		sqlite3stmt st(_sqlite, sql.c_str());
		pStmt = &st;
		while (SQLITE_ROW == st.step())
		{
			Row<T> row;
			this->operator & (make_col(RowIdColName, row.RowId));
			row.orm(*this);
			rows.push_back(row);
		}

		return st.how();
	}

	template<typename T>
	RowLoader& operator&(Col<T> & col)
	{
		int iCol = pStmt->columnIndex(col.ColName.c_str());
		if (iCol != -1)
		{
			pStmt->column(iCol, col.ColValue);
		}
		
		return *this;
	}
private:
	sqlite& _sqlite;
	const string& sql;	
	sqlite3stmt* pStmt;
};

struct TLoader
{
	TLoader(sqlite3stmt* pst)
		:pStmt(pst)
	{

	}
	template<typename T>
	TLoader& operator&(Col<T> & col)
	{
		int iCol = pStmt->columnIndex(col.ColName.c_str());
		if (iCol != -1)
		{
			pStmt->column(iCol, col.ColValue);
		}

		return *this;
	}
private:
	sqlite3stmt* pStmt;
};
