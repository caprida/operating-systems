#include <iostream>
#include <fstream>
using namespace std;

struct employee
{
	int num;			
	char name[10];		
	double hours;		
};
int comp(const void* comp1, const void* comp2){
	employee* p1 = (employee*)comp1;
	employee* p2 = (employee*)comp2;
	return p1->num - p2->num;
}

int main(int argc, char* argv[])// 1. name of the binary file. 2.name of the report file. 3.pay for hour
{
	fstream fin(argv[1], ios::in);
	ofstream fout(argv[2]);
	double payforhours = atof(argv[3]);
	int count;
	fin.read((char*)&count, sizeof(int));
	employee* emp = new employee[count];
	for (int i = 0; i < count; i++)
		fin.read((char*)&emp[i], sizeof(employee));

	qsort(emp, count, sizeof(employee), comp);
	fout << "Отчет по файлу \"" << argv[1] << "\"" << endl;
	fout << "Номер сотрудника, имя сотрудника, часы, зарплата" << endl;
	for (int i = 0; i < count; i++)
	{
		fout << emp[i].num << " " << emp[i].name << " " << emp[i].hours << " " << emp[i].hours * payforhours << endl;
	}
	fout.close();
	fin.close();
	return 0;
}