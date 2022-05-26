#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <fstream>
#include <cstdio>
using namespace std;
int main()
{
    char nameBin[80];
    int countOfRecords;

    cout << "Name of the binary file: ";
    cin >> nameBin;
    cout <<"Number of records: ";
    cin >>countOfRecords;

    std::ofstream output(nameBin, std::ios::trunc);
    output.close();

    // кол-во процессов Sender
    int numberOfProcess;
    cout << "Enter the number of processes ";
    cin >> numberOfProcess;

    // командная строка для запуска процесса с параметром
    char commandLine[100];
    sprintf(commandLine, "%s %s %d", "Sender.exe", nameBin, countOfRecords);

    // сигнал готовности к работе
    HANDLE hWorkEvent;
    CHAR EventName[] = "ServerIsReady";
    hWorkEvent = CreateEvent(NULL, FALSE, FALSE, EventName);

    // мьютекс для совместного доступа к бинарному файлу
    HANDLE	hMutex;
    hMutex = CreateMutex(NULL, FALSE, "Sender");

    std::fstream fin, fout;

    STARTUPINFO *si = new STARTUPINFO[numberOfProcess];
    PROCESS_INFORMATION *piCom = new PROCESS_INFORMATION[numberOfProcess];

    // запускаем заданное количество процессов
    for (int i = 0; i < numberOfProcess; i++)
    {
        ZeroMemory(&si[i], sizeof(STARTUPINFO));
        si[i].cb = sizeof(STARTUPINFO);

        // создаем новый консольный процесс
        CreateProcess(NULL, (LPTSTR)commandLine, NULL, NULL, FALSE,
                      CREATE_NEW_CONSOLE, NULL, NULL, &si[i], &piCom[i]);
    }

    // ждем сигнал от всех о готовности к работе
    for (int i = 0; i < numberOfProcess; i++)
    {
        WaitForSingleObject(hWorkEvent, INFINITE);
    }
    CloseHandle(hWorkEvent);

    bool b = true;
    int fileSize;
    int k;
    while (b)
    {
        cout << "What do you want to do? \n1. To read message from binary file \n2. To finish work\n" << endl;
        cin >> k;

        switch (k)
        {
            case 1:
                fileSize = 0;
                while (fileSize == 0)
                {
                    WaitForSingleObject(hMutex, INFINITE);
                    fin.open(nameBin, std::ios::binary | std::ios::in);
                    fin.seekg(0, std::ios::end);
                    fileSize = fin.tellg();
                    fin.close();
                    ReleaseMutex(hMutex);
                    if (fileSize == 0)
                    {
                        Sleep(1000);
                    }
                }
                WaitForSingleObject(hMutex, INFINITE);

                fin.open(nameBin, std::ios::binary | std::ios::in);

                char message[20];

                fin.read( message, 20 * sizeof(char));
                cout <<  message << endl;

                // создаем временный файл, чтобы удалить из файла первое сообщение
                fout.open("tmpfile", std::ios::binary | std::ios::out | std::ios::trunc);
                while (!fin.eof())
                {
                    fin.read( message, 20 * sizeof(char));

                    // во временный файл записываются все сообщения из основного файла, начиная со второго
                    if (!fin.eof())
                    {
                        fout.write( message, 20 * sizeof(char));
                    }
                }
                fin.close();
                fout.close();

                // снова открываем временный файл для того, чтобы скопировать его в основной
                fin.open("tmpfile", std::ios::binary | std::ios::in);
                fout.open(nameBin, std::ios::binary | std::ios::out | std::ios::trunc);
                while (!fin.eof())
                {
                    fin.read( message, 20 * sizeof(char));
                    // временный файл копируется в основной
                    if (!fin.eof())
                    {
                        fout.write( message, 20 * sizeof(char));
                    }
                }
                fin.close();
                fout.close();

                // удаляем временный файл
                remove("tmpfile");

                ReleaseMutex(hMutex);
                break;
            case 2:
                //завершить работу
                b = false;
                break;
            default:
                break;
        }
    }
    CloseHandle(hMutex);

    return 0;
}
