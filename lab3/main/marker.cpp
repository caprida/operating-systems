//
// Created by даша on 23.05.2022.
//

#include <Windows.h>
#include<iostream>

using namespace std;

extern int count;
extern int *array;
extern HANDLE *hWorkEvent, *hStopEvent, *hFailEvent;

DWORD WINAPI marker(int _number)
{
    int number = (int)_number;
    HANDLE hMyEvent[3];

    //WAIT_OBJECT_0 -- завершение работы
    hMyEvent[0] = hStopEvent[number - 1];

    //WAIT_OBJECT_1 -- старт/продолжение работы
    hMyEvent[1] = hWorkEvent[number - 1];

    //Не используется в WaitForMultipleObjects, используется для хранения события hFailEvent для данного потока
    hMyEvent[2] = hFailEvent[number - 1];

    //Начать работу по сигналу от потока main.
    WaitForSingleObject(hMyEvent[1], INFINITE);

    srand(number);
    int mark = 0;
    bool b = true;

    DWORD dwWaitResult;

    while (b)
    {
        ResetEvent(hMyEvent[1]);
        int random = rand();
        int c = random % count;
        if (array[c] == 0)
        {
            Sleep(5);
            array[c] = number;
            Sleep(5);
            mark++;
        }
        else
        {
            cout << "Threads number: " << number << "; Count of mark: " << mark<< "; Impossible to mark: " << c << endl;

            //Дать сигнал потоку main на невозможность продолжения своей работы.
            SetEvent(hMyEvent[2]);

            //Ждать ответный сигнал на продолжение или завершение работы от потока main.
            dwWaitResult = WaitForMultipleObjects(2, hMyEvent, FALSE, INFINITE);

            //Если получен сигнал на завершение работы, то выполнить следующие действия:
            if (dwWaitResult == WAIT_OBJECT_0)
            {
                for (int i = 0; i < count; i++)
                {
                    //Заполнить нулями в массиве все элементы, которые он пометил.
                    if (array[i] == number)
                        array[i] = 0;
                }
               // mark = 0;

                //Завершить работу.
                b = false;
                break;
            }
                //Если получен сигнал на продолжение работы, то продолжить исполнение цикла из пункта 3.
            else
            {
                //сбросить сигнал о невозможности продолжать
                ResetEvent(hMyEvent[2]);
            }
        }
    }
    return 0;
}