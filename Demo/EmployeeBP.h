#pragma once

#include <string>
#include <vector>
#include <set>
#include <map>
using namespace std;

#define DATABASE "Demo.db"
#define TABLE "TEmployee"

struct CEmployee;
class EmployeeBP 
{
public:
	//创建表
	void CreateTable();

	//新增员工
	void InsertEmployee(CEmployee& e);

	//修改员工
	void UpdateEmployee(CEmployee& e);

	//删除员工
	void DeleteEmployee(CEmployee& e);

	//查询雇员数量
	int GetEmployeeCount();

	//查询所有的员工姓名
	std::vector<string> GetAllEmployeeName();

	//查询所有的员工身份证号和姓名
	std::map<string, string> GetAllEmployeeIDcardAndName();

	//查询月薪最高的前5位雇员
	std::vector<CEmployee> GetTop5SalaryEmployee();

	//查询所有雇员的所有籍贯
	std::set<string> GetAllEmployeeBirthPlace();

	//年龄>30的雇员加薪500
	void UpdateSalary();
};