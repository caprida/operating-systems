#include <iostream>
#include"thread_param.h"
#include <Windows.h>
using namespace std;
DWORD WINAPI min_max(LPVOID);
DWORD WINAPI average(LPVOID);
int minimum, maximum, averageElement;

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

    HANDLE hMin_max, hAverage;
    DWORD IDMin_max, IDAverage;
    hMin_max = CreateThread(NULL, 0, min_max,
                            (LPVOID) &param,0, &IDMin_max);
    if (hMin_max == NULL)
        return GetLastError();
    WaitForSingleObject(hMin_max, INFINITE);
    CloseHandle(hMin_max);
    cout << "minimum: " << minimum << " , " << "maximum: " << maximum << endl;

    hAverage = CreateThread(NULL, 0, average,
                            (LPVOID) &param,0, &IDAverage);
    if (hAverage== NULL)
        return GetLastError();
    WaitForSingleObject(hAverage, INFINITE);
    CloseHandle(hAverage);
    cout <<"average: "<< averageElement << endl;
    for(int i = 0; i < count; i++){
        if(array[i] == minimum || array[i] == maximum)
            array[i] = averageElement;
    }
    for(int i = 0; i < count; i++){
        cout << array[i] << " ";
    }

    return 0;
}
