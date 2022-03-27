//
// Created by даша on 27.03.2022.
//
#include <iostream>
#include"thread_param.h"
#include <Windows.h>
using namespace std;
extern int averageElement;

DWORD WINAPI average(LPVOID param){
    thread_param *_param = (thread_param*) param;
    int *arr = _param->arr;
    int n = _param->count;
    averageElement = 0;
    for (int i = 0; i < n; i++)
    {
        averageElement += arr[i];
        Sleep(12);
    }
    averageElement /= n;
    return 0;
}
