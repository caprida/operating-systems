//
// Created by даша on 24.03.2022.
//
#include <iostream>
#include"thread_param.h"
#include <Windows.h>
using namespace std;
extern int minimum, maximum;
DWORD WINAPI min_max(LPVOID param){
    thread_param *_param = (thread_param*) param;
    int *arr = _param->arr;
    int n = _param->count;
    minimum = arr[0];
    maximum = arr[0];
    for (int i = 0; i < n; i++)
    {
        if (arr[i] < minimum)
            minimum = arr[i];
        Sleep(7);
        if (arr[i] > maximum)
            maximum = arr[i];
        Sleep(7);
    }
return 0;
}

