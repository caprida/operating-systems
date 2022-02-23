#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include <fstream>

using namespace std;

struct employee
{
	int num;			// идентификационный номер сотрудника
	char name[10];		// имя сотрудника
	double hours;		// количество отработанных часов
};

int main()
{
	setlocale(LC_ALL, ".1251");
	char namebin[80];
	char buffer[10];
	cout << "Name of binary file: ";
	cin >> namebin;
	int count;
	cout << "Count of records: ";
	cin >> count;
	cout << endl;
	char commandline[100];
	strcpy(commandline, "Creator.exe ");
	strcat(commandline, namebin);
	strcat(commandline, " ");
	strcat(commandline, _itoa(count, buffer, 10));

	STARTUPINFO si;
	PROCESS_INFORMATION piCom;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	// создаем новый консольный процесс
	if (!CreateProcess(NULL, (LPTSTR)commandline, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si, &piCom)) {
		cout << ":(";
	};
	// ждем завершения созданного прцесса
	WaitForSingleObject(piCom.hProcess, INFINITE);
	// закрываем дескрипторы этого процесса в текущем процессе
	CloseHandle(piCom.hThread);
	CloseHandle(piCom.hProcess);

	fstream bin(namebin, ios::in);
	int count_bin;
	bin.read((char*)&count_bin, sizeof(int));
	employee* emp = new employee[count];
	for (int i = 0; i < count; i++)
	{
		bin.read((char*)&emp[i], sizeof(employee));
		cout << emp[i].num << " " << emp[i].name << " " << emp[i].hours << endl;
	}
	bin.close();
	cout << endl;

	char nametext[80];
	char payforhours[10];
	cout << "Name of report file: ";
	cin >> nametext;
	cout << "Pay for hours: ";
	cin >> payforhours;
	cout << endl;
	char commandline2[100];
	strcpy(commandline2, "Reporter.exe ");
	strcat(commandline2, namebin);
	strcat(commandline2, " ");
	strcat(commandline2, nametext);
	strcat(commandline2, " ");
	strcat(commandline2, payforhours);
	CreateProcess(NULL, (LPTSTR)commandline2, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si, &piCom);

	// ждем завершения созданного прoцесса
	WaitForSingleObject(piCom.hProcess, INFINITE);

	// закрываем дескрипторы этого процесса в текущем процессе
	CloseHandle(piCom.hThread);
	CloseHandle(piCom.hProcess);

	ifstream fin(nametext);
	while (!fin.eof())
	{
		char* str = new char[100];
		fin.getline(str, 100);
		cout << str << endl;
	}
	fin.close();
	system("pause");
	return 0;
}