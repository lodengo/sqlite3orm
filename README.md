sqlite3orm
==========
Make simple things simpler
---------
应用示例  
做一个雇员管理程序，实现对雇员信息的增加、修改、删除、查询。  

雇员信息包括：  
工号：按雇员入职先后自动递增的唯一的数字编号标识。  
姓名：雇员姓名，可以有重名雇员，不能为空。  
性别：男或女，不能为空。  
年龄：整数。  
身份证号：唯一且不能为空。  
籍贯：  
月薪：  

对象分析：  
分析得出对象雇员的数据成员 
```cpp 
	struct CEmployee   
	{  
		int ID;//工号：按雇员入职先后自动递增的唯一的数字编号标识。  
		string Name;//姓名  
		string Gender;//性别  
		int Age;//年龄：整数。  
		string IDCardNo;//身份证号  
		string BirthPlace;//籍贯：  
		double Salary;//月薪：  
	}; 
``` 
数据存储：  
假设使用Sqlite3数据库的雇员表存储上述雇员对象的数据信息。  
我们会建一张Temployee表，表的字段如下：  
ID：自动递增的主键  
Name：非空的字符串字段  
Gender：非空的字符串字段  
Age：整形字段  
IDCardNo：非空、唯一的字符串字段  
BirthPlace：字符串字段  
Salary：实数字段  

关系映射：  
可以看出我们前面分析出的对象结构和数据库表结构间存在某种对应关系，一个Cemployee结构的对象对应于Temployee表中的一行记录，且对象的字段和表的列之间存在对应关系。  

操作映射：  
我们对雇员对象的操作和数据库表的增删改查之间也有对应关系：	  
雇员信息增加：创建Cemployee对象 ——Temployee插入一条记录  
雇员信息修改：修改Cemployee对象 ——Temployee修改一条记录  
雇员信息删除：销毁Cemployee对象 ——Temployee删除一条记录  
雇员信息查询：填充Cemployee对象 ——Temployee查询一条记录  

映射实现：  
实现Cemployee的映射方法如下：  
```cpp 
	template<class T>  
	void orm(T& t)  
	{  
		t & ORM_AINCPK(ID);	//主键  
		t & ORM_COL(Name);  
		t & ORM_COL(Gender);  	
		t & ORM_COL(Age);  
		t & ORM_COL(IDCardNo);  
		t & ORM_COL(BirthPlace);  
		t & ORM_COL(Salary);  
	}  
```
业务：  
```cpp 
	#define DATABASE "Demo.db"  
	#define TABLE "TEmployee"  
```
创建表  
```cpp 
	Sqlite3x sql(DATABASE);  
	CEmployee p;  
	sql.createTable(TABLE, TableSchema<CEmployee>(p)); 
``` 

插入一条记录：  
```cpp 
	void EmployeeBP::InsertEmployee(CEmployee& e)  
	{  
		Sqlite3x sql(DATABASE);  
		sql.insert(TABLE, e);  
	}  
```

修改一条记录： 
```cpp  
	void EmployeeBP::UpdateEmployee(CEmployee& e)  
	{  
		Sqlite3x sql(DATABASE);  
		sql.update(TABLE, e);  
	}  
```

删除一条记录：  
```cpp 
	void EmployeeBP::DeleteEmployee(CEmployee& e)  
	{  
		Sqlite3x sql(DATABASE);  
		sql.Delete(TABLE, e);  
	}  
```

雇员信息查询：  
//查询雇员数量 
```cpp  
	int EmployeeBP::GetEmployeeCount()  
	{  
		int ret = 0;  
		Sqlite3x sql(DATABASE);  
		sql.select("select count(1) from TEmployee", ret);  
		return ret;  
	}  
```

//查询所有的员工姓名  
```cpp 
	std::vector<string> EmployeeBP::GetAllEmployeeName()  
	{  
		std::vector<string> ret;  
		Sqlite3x sql(DATABASE);  
		sql.select("select Name from TEmployee", ret);  
		return ret;  
	}  
```

//查询所有的员工身份证号和姓名  
```cpp 
	std::map<string, string> EmployeeBP::GetAllEmployeeIDcardAndName()  
	{  
		std::map<string, string> ret;  
		Sqlite3x sql(DATABASE);  
		sql.select("select IDCardNo, Name from TEmployee", ret);  
		return ret;  
	}  
```

//查询月薪最高的前位雇员  
```cpp 
	std::vector<CEmployee> EmployeeBP::GetTop5SalaryEmployee()  
	{  
		std::vector<CEmployee> ret;  
		Sqlite3x sql(DATABASE);  
		sql.select("select * from TEmployee ORDER BY Salary desc LIMIT 5", ret);  
		//sql.select("select * from TEmployee ORDER BY Salary desc", ret, 5);  
		return ret;  
	} 
``` 

//查询所有雇员的所有籍贯 
```cpp  
	std::set<string> EmployeeBP::GetAllEmployeeBirthPlace()  
	{  
		std::set<string> ret;  
		Sqlite3x sql(DATABASE);  
		sql.select("select DISTINCT BirthPlace from TEmployee", ret);  
		//sql.select("select BirthPlace from TEmployee", ret);  
		return ret;  
	}  
```


执行Sql:  
//年龄>30的雇员加薪  
```cpp 
	void UpdateSalary()  
	{  
		Sqlite3x sql(DATABASE);  
		string err;  
		sql.exec("update TEmployee set Salary = Salary + 500 where Age > 30", err);  
	}  
```