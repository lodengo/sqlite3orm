#pragma once

#include <string>
using std::string;

struct CEmployee 
{
	CEmployee()
	{
		Gender = "��";
		Age = 25;
		Salary = 2500;
	}
	template<class T>
	void orm(T& t)
	{
		t & ORM_AINCPK(ID);
		t & ORM_COL(Name);
		t & ORM_COL(Gender);	
		t & ORM_COL(Age);
		t & ORM_COL(IDCardNo);
		t & ORM_COL(BirthPlace);
		t & ORM_COL(Salary);
	}

	int ID;//���ţ�����Ա��ְ�Ⱥ��Զ�������Ψһ�����ֱ�ű�ʶ��
	string Name;//��������Ա������������������Ա������Ϊ�ա�
	string Gender;//�Ա��л�Ů������Ϊ�ա�
	int Age;//���䣺������
	string IDCardNo;//ʡ��֤�ţ�Ψһ�Ҳ���Ϊ�ա�
	string BirthPlace;//���᣺
	double Salary;//��н��
};