#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <Windows.h>
#include <cstdio>

using namespace std;

int count;
int* array;

HANDLE *hWorkEvent, *hStopEvent, *hFailEvent;

DWORD WINAPI marker(int);

int main()
{
	cout << "Enter the count of array elements ";
	cin >> count;

	array = new int[count];

	for (int i = 0; i < count; i++)
		array[i] = 0;

	int countMarker;
	int maxMarker;
	cout << "Enter the count of marker threads " << endl;
	cin >> countMarker;
	maxMarker = countMarker + 1;

	HANDLE *hMarker = new HANDLE[countMarker];
	DWORD *dwMarker = new DWORD[countMarker];

	//хранение номеров потоков, они же маркеры, нумерация потоков начинается с 1
	int *index = new int[countMarker];

	//сигнал начала работы
	hWorkEvent = new HANDLE[countMarker];

	//сигнал завершения работы
	hStopEvent = new HANDLE[countMarker];

	//сигнал невозможности продолжения работы
	hFailEvent = new HANDLE[countMarker];

	for (int i = 0; i < countMarker; i++)
    {
		hWorkEvent[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		hStopEvent[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		hFailEvent[i] = CreateEvent(NULL, TRUE, FALSE, NULL);

		//нумерация порядковых номеров (они же маркеры) начинается с 1
		index[i] = i + 1;

		// запускаем countMarker экземпляров потока Marker
		hMarker[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)marker,
			(LPVOID) index[i], 0, &dwMarker[i]);
		if (hMarker[i] == NULL)
			return GetLastError();
	}
	bool b = true;

	// инициализируем критическую секцию
	CRITICAL_SECTION cs;
	InitializeCriticalSection(&cs);

	while (b)
	{
		EnterCriticalSection(&cs);

		for (int i = 0; i < countMarker; i++)
		{
			//сбросить состояние сигнала о невозможности продолжать для всех оставшихся потоков
			ResetEvent(hFailEvent[i]);

			//Дать сигнал на начало работы всех потоков marker
			//Подать сигнал на продолжение работы оставшимся потокам marker.
			SetEvent(hWorkEvent[i]);
		}

		//ждать пока все потоки marker не подадут сигналы о невозможности продолжения работы
		WaitForMultipleObjects(countMarker, hFailEvent, TRUE, INFINITE);

		LeaveCriticalSection(&cs);

		cout <<"Elements of array: ";
		for (int i = 0; i < count; i++)
			cout << array[i] << " ";
		cout << endl;

		//вывести список оставшихся маркеров
		printf("Markers still valid:");
		for (int i = 1; i <= maxMarker; i++)
			for (int j = 0; j < countMarker; j++)
				if (index[j] == i)
					cout << i << " ";
		cout << endl;

		//запросить с консоли номер потока marker, которому подадим сигнал на завершение работы
		cout << "Enter the number of the marker thread, that will be given the signal to complete its work " << endl;
		int k;
		cin >> k;

		int j = 0;
		//поиск потока по номеру
		while ((index[j] != k) && (j < countMarker))
		{
			j++;
		}

		//если номер не найден - выход
		if (j == countMarker)
			break;

		//Подать потоку marker, номер которого получен, сигнал на завершение работы.
		SetEvent(hStopEvent[j]);

		//Ждать завершение работы потока marker, которому был подан сигнал на завершение работы
		WaitForSingleObject(hMarker[j], INFINITE);

		//удалить завершённый процесс, переместив последний на его место во всех массивах
		CloseHandle(hMarker[j]);
		hMarker[j] = hMarker[countMarker - 1];
		hStopEvent[j] = hStopEvent[countMarker - 1];
		hWorkEvent[j] = hWorkEvent[countMarker - 1];
		hFailEvent[j] = hFailEvent[countMarker - 1];
		index[j] = index[countMarker - 1];
		countMarker--;

		//Вывести содержимое массива на консоль.
		cout << "Elements of array " << endl;
		for (int i = 0; i < count; i++)
			cout << array[i] << " ";
		cout << endl;

		//Завершить свою работу после завершения работы всех потоков marker.
		if (countMarker == 0)
		{
			b = false;
		}
	}

	// закрываем критическую секцию
	DeleteCriticalSection(&cs);
	getchar();
	getchar();

	return 0;
}
