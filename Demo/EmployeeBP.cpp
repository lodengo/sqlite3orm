#include "stdafx.h"
#include "EmployeeBP.h"
#include "Sqlite3x.h"
#include "Employee.h"

//������
void EmployeeBP::CreateTable()
{
	Sqlite3x sql(DATABASE);
	CEmployee p;
	sql.createTable(TABLE, TableSchema<CEmployee>(p));
}

//����Ա��
void EmployeeBP::InsertEmployee(CEmployee& e)
{
	Sqlite3x sql(DATABASE);
	sql.insert(TABLE, e);
}

//�޸�Ա��
void EmployeeBP::UpdateEmployee(CEmployee& e)
{
	Sqlite3x sql(DATABASE);
	sql.update(TABLE, e);
}

//ɾ��Ա��
void EmployeeBP::DeleteEmployee(CEmployee& e)
{
	Sqlite3x sql(DATABASE);
	sql.Delete(TABLE, e);
}

//��ѯ��Ա����
int EmployeeBP::GetEmployeeCount()
{
	int ret = 0;
	Sqlite3x sql(DATABASE);
	sql.select("select count(1) from TEmployee", ret);
	return ret;
}

//��ѯ���е�Ա������
std::vector<string> EmployeeBP::GetAllEmployeeName()
{
	std::vector<string> ret;
	Sqlite3x sql(DATABASE);
	sql.select("select Name from TEmployee", ret);
	return ret;
}

//��ѯ���е�Ա�����֤�ź�����
std::map<string, string> EmployeeBP::GetAllEmployeeIDcardAndName()
{
	std::map<string, string> ret;
	Sqlite3x sql(DATABASE);
	sql.select("select IDCardNo, Name from TEmployee", ret);
	return ret;
}

//��ѯ��н��ߵ�ǰ5λ��Ա
std::vector<CEmployee> EmployeeBP::GetTop5SalaryEmployee()
{
	std::vector<CEmployee> ret;
	Sqlite3x sql(DATABASE);
	sql.select("select * from TEmployee ORDER BY Salary desc LIMIT 5", ret);
	sql.select("select * from TEmployee ORDER BY Salary desc", ret, 5);
	return ret;
}

//��ѯ���й�Ա�����м���
std::set<string> EmployeeBP::GetAllEmployeeBirthPlace()
{
	std::set<string> ret;
	Sqlite3x sql(DATABASE);
	sql.select("select DISTINCT BirthPlace from TEmployee", ret);
	sql.select("select BirthPlace from TEmployee", ret);
	return ret;
}

//����>30�Ĺ�Ա��н500
void UpdateSalary()
{
	Sqlite3x sql(DATABASE);
	string err;
	sql.exec("update TEmployee set Salary = Salary + 500 where Age > 30", err);
}