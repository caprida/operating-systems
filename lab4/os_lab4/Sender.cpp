//
// Created by даша on 23.05.2022.
//
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include <fstream>
#include <cstdio>
using namespace std;

int main(int argc, char *argv[])
{
    HANDLE hWorkEvent;
    CHAR EventName[] = "ServerIsReady";
    hWorkEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, EventName);

    // отправить сигнал о готовности
    SetEvent(hWorkEvent);

    // имя файла из командной строки
    char nameBin[80];
    sprintf(nameBin, "%s", argv[1]);

    // кол-во записей в файл из командной строки
    char countOfRec[10];
    sprintf(countOfRec, "%s", argv[2]);
    int countOfRecords = atoi(countOfRec);

    int maxFileSize = 20 * countOfRecords;

    std::fstream fout;

    // мьютекс для совместного доступа к бинарному файлу
    HANDLE	hMutex;
    hMutex = OpenMutex(SYNCHRONIZE, FALSE, "Sender");

    int k;
    bool b = true;
    int fileSize;
    char message[20];

    while (b)
    {
        cout << "What do you want to do? \n1. To send message to Receiver process \n2. To finish work\n" << endl;
        cin >> k;

        switch (k)
        {
            case 1:
                cout << "Enter the message: " << endl;
                cin >> message;
                fileSize = maxFileSize;

                while (fileSize >= maxFileSize)
                {
                    WaitForSingleObject(hMutex, INFINITE);

                    // смотрим размер файла
                    fout.open(nameBin, std::ios::binary | std::ios::in);
                    fout.seekg(0, std::ios::end);
                    fileSize = fout.tellg();
                    fout.close();

                    ReleaseMutex(hMutex);

                    if (fileSize >= maxFileSize)
                    {
                        Sleep(1000);
                    }
                }
                WaitForSingleObject(hMutex, INFINITE);

                // записываем сообщение в файл
                fout.open(nameBin, std::ios::out | std::ios::app | std::ios::binary);
                fout.write(message, 20 * sizeof(char));
                fout.close();

                ReleaseMutex(hMutex);
                break;
            case 2:
                //прекратить работу
                b = false;
                break;
            default:
                break;
        }
    }
    return 0;
}

