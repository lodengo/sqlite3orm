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
	//������
	void CreateTable();

	//����Ա��
	void InsertEmployee(CEmployee& e);

	//�޸�Ա��
	void UpdateEmployee(CEmployee& e);

	//ɾ��Ա��
	void DeleteEmployee(CEmployee& e);

	//��ѯ��Ա����
	int GetEmployeeCount();

	//��ѯ���е�Ա������
	std::vector<string> GetAllEmployeeName();

	//��ѯ���е�Ա�����֤�ź�����
	std::map<string, string> GetAllEmployeeIDcardAndName();

	//��ѯ��н��ߵ�ǰ5λ��Ա
	std::vector<CEmployee> GetTop5SalaryEmployee();

	//��ѯ���й�Ա�����м���
	std::set<string> GetAllEmployeeBirthPlace();

	//����>30�Ĺ�Ա��н500
	void UpdateSalary();
};