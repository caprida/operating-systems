#include <iostream>
#include"thread_param.h"
using namespace std;

int main() {
    int count;
    cin >> count;
    int *array = new int[count];
    for (int i = 0; i < count; i++) {
        cin >> array[i];
    }
    for (int i = 0; i < count; i++) {
        cout << array[i] << " " << endl;
    }
    thread_param param = {};
    param.arr = array;
    param.count = count;
    
    return 0;
}
