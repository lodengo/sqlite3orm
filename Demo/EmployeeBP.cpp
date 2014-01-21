#include "stdafx.h"
#include "EmployeeBP.h"
#include "Sqlite3x.h"
#include "Employee.h"

//创建表
void EmployeeBP::CreateTable()
{
	Sqlite3x sql(DATABASE);
	CEmployee p;
	sql.createTable(TABLE, TableSchema<CEmployee>(p));
}

//新增员工
void EmployeeBP::InsertEmployee(CEmployee& e)
{
	Sqlite3x sql(DATABASE);
	sql.insert(TABLE, e);
}

//修改员工
void EmployeeBP::UpdateEmployee(CEmployee& e)
{
	Sqlite3x sql(DATABASE);
	sql.update(TABLE, e);
}

//删除员工
void EmployeeBP::DeleteEmployee(CEmployee& e)
{
	Sqlite3x sql(DATABASE);
	sql.Delete(TABLE, e);
}

//查询雇员数量
int EmployeeBP::GetEmployeeCount()
{
	int ret = 0;
	Sqlite3x sql(DATABASE);
	sql.select("select count(1) from TEmployee", ret);
	return ret;
}

//查询所有的员工姓名
std::vector<string> EmployeeBP::GetAllEmployeeName()
{
	std::vector<string> ret;
	Sqlite3x sql(DATABASE);
	sql.select("select Name from TEmployee", ret);
	return ret;
}

//查询所有的员工身份证号和姓名
std::map<string, string> EmployeeBP::GetAllEmployeeIDcardAndName()
{
	std::map<string, string> ret;
	Sqlite3x sql(DATABASE);
	sql.select("select IDCardNo, Name from TEmployee", ret);
	return ret;
}

//查询月薪最高的前5位雇员
std::vector<CEmployee> EmployeeBP::GetTop5SalaryEmployee()
{
	std::vector<CEmployee> ret;
	Sqlite3x sql(DATABASE);
	sql.select("select * from TEmployee ORDER BY Salary desc LIMIT 5", ret);
	sql.select("select * from TEmployee ORDER BY Salary desc", ret, 5);
	return ret;
}

//查询所有雇员的所有籍贯
std::set<string> EmployeeBP::GetAllEmployeeBirthPlace()
{
	std::set<string> ret;
	Sqlite3x sql(DATABASE);
	sql.select("select DISTINCT BirthPlace from TEmployee", ret);
	sql.select("select BirthPlace from TEmployee", ret);
	return ret;
}

//年龄>30的雇员加薪500
void UpdateSalary()
{
	Sqlite3x sql(DATABASE);
	string err;
	sql.exec("update TEmployee set Salary = Salary + 500 where Age > 30", err);
}