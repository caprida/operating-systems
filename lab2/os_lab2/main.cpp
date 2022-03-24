#include <iostream>
#include"thread_param.h"
#include <Windows.h>
using namespace std;
DWORD WINAPI min_max(LPVOID);
int minimum, maximum;

int main() {
    int count;
    cin >> count;
    int *array = new int[count];
    for (int i = 0; i < count; i++) {
        cin >> array[i];
    }
    thread_param param = {};
    param.arr = array;
    param.count = count;

    HANDLE hmin_max;
    DWORD IDmin_max;
    hmin_max = CreateThread(NULL, 0, min_max, (LPVOID) &param, 0, &IDmin_max);
    if (hmin_max == NULL)
        return GetLastError();
    WaitForSingleObject(hmin_max, INFINITE);
    CloseHandle(hmin_max);
    cout << minimum << " , " << maximum << endl;
    return 0;
}
