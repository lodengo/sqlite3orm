#pragma once

#include <sqlite3.h>
#pragma warning(disable:4251)

#define RowIdColName "RowPKid"
#define ParentColName "ParentId"
#define RowIdType "INTEGER"
#define ClassColType "TEXT"
typedef sqlite3_int64 TRowId;
#define NullRowId -1
