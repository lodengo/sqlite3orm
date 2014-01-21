#pragma once

#include <string>
using std::string;

struct CEmployee 
{
	CEmployee()
	{
		Gender = "男";
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

	int ID;//工号：按雇员入职先后自动递增的唯一的数字编号标识。
	string Name;//姓名：雇员姓名，可以有重名雇员，不能为空。
	string Gender;//性别：男或女，不能为空。
	int Age;//年龄：整数。
	string IDCardNo;//省份证号：唯一且不能为空。
	string BirthPlace;//籍贯：
	double Salary;//月薪：
};