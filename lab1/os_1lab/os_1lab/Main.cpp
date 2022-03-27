#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include <fstream>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::fstream;

struct employee
{
	int num;			
	char name[10];		
	double hours;		
};

int main()
{
	setlocale(LC_ALL, ".1251");
	char nameBin[80];
	char buffer[10];
	cout << "Name of binary file: ";
	cin >> nameBin;
	int count;
	cout << "Count of records: ";
	cin >> count;
	cout << endl;
	char commandLine[100];
	strcpy(commandLine, "Creator.exe ");
	strcat(commandLine, nameBin);
	strcat(commandLine, " ");
	strcat(commandLine, _itoa(count, buffer, 10));

	STARTUPINFO si;
	PROCESS_INFORMATION piCom;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	if (!CreateProcess(NULL, (LPTSTR)commandLine, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si, &piCom)) {
		cout << "Creator is not created";
	}
	WaitForSingleObject(piCom.hProcess, INFINITE);
	CloseHandle(piCom.hThread);
	CloseHandle(piCom.hProcess);

	fstream bin(nameBin, ios::in);
	int countBin;
	bin.read((char*)&countBin, sizeof(int));
	employee* emp = new employee[count];
	for (int i = 0; i < count; i++)
	{
		bin.read((char*)&emp[i], sizeof(employee));
		cout << emp[i].num << " " << emp[i].name << " " << emp[i].hours << endl;
	}
	bin.close();
	cout << endl;

	char nameText[80];
	char payForHours[10];
	cout << "Name of report file: ";
	cin >> nameText;
	cout << "Pay for hours: ";
	cin >> payForHours;
	cout << endl;
	char commandLine2[100];
	strcpy(commandLine2, "Reporter.exe ");
	strcat(commandLine2, nameBin);
	strcat(commandLine2, " ");
	strcat(commandLine2, nameText);
	strcat(commandLine2, " ");
	strcat(commandLine2, payForHours);
	if(!CreateProcess(NULL, (LPTSTR)commandLine2, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si, &piCom)){
			cout<<"Reporter is not created";
	}

	WaitForSingleObject(piCom.hProcess, INFINITE);

	CloseHandle(piCom.hThread);
	CloseHandle(piCom.hProcess);

	try{
	ifstream fin("");
	char* str = new char[100];
	while (!fin.eof())
	{
		fin.getline(str, 100);
		cout << str << endl;
	}
	fin.close();
	}
	catch(FileNotFoundException e){
		cout << "The file was not found:" << e;
	}
	

	system("pause");
	return 0;
}