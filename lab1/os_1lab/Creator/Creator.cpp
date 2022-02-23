#include <iostream>
#include <fstream>
using namespace std;

struct employee
{
	int num;			// идентификационный номер сотрудника
	char name[10];		// имя сотрудника
	double hours;		// количество отработанных часов
};

int main(int argc, char* argv[])//1 name file, 2 count of records
{
	fstream fout(argv[1], ios::out);
	int count = atoi(argv[2]);
	fout.write((char*)&count, sizeof(int));
	employee emp;
	for (int i = 0; i < count; i++)
	{
		cout << "Enter num   ";
		cin >> emp.num;
		cout << "Enter name(no more 10 chars)   ";
		cin >> emp.name;
		cout << "Enter hours  ";
		cin >> emp.hours;
		cout << endl;
		fout.write((char*)&emp, sizeof(employee));
	}
	fout.close();
	return 0;
}