#include <iostream>	
#include <windows.h>
#include <semaphore>
#include "buffer.h"	/*Количество производителей*/	 
#define cProducers 3		 
#define cConsumers 5	/*Количество потребителей*/	 
#define BufferSize 5	/*Размер буфера*/	 

int cOperations = 100; /*Количество операций над буфером*/
HANDLE hMutex;
HANDLE hSEmpty;
HANDLE hSFull;

DWORD __stdcall getkey(void* b) {
	cin.get();
	return cOperations = 0;
}

DWORD __stdcall producer(void* b) {
	while (cOperations-- > 0) {
		WaitForSingleObject(hSFull, INFINITE);
		WaitForSingleObject(hMutex, INFINITE);
		int item = rand();
		((Buffer*)b)->PutItem(item);
		ReleaseSemaphore(hMutex, 1, NULL);
		ReleaseSemaphore(hSEmpty, 1, NULL);
		cout << "Produced: " << item << endl;
		Sleep(500 + rand() % 100);
	}
	return 0;
}

DWORD __stdcall consumer(void* b) {
	while (cOperations-- > 0) {
		WaitForSingleObject(hSEmpty, INFINITE);
		WaitForSingleObject(hMutex, INFINITE);
		cout << "Consumed: " << ((Buffer*)b)->GetItem() << endl;
		Sleep(500 + rand() % 100);
		ReleaseSemaphore(hMutex, 1, NULL);
		ReleaseSemaphore(hSFull, 1, NULL);
	}
	return 0;
}

int main() {
	Buffer* Buf = Buffer::CreateBuffer(5); /*Создание буфера*/
	HANDLE hThreads[cProducers + cConsumers];
	/*Вспомогательный поток, ожидающий нажатие клавиши*/
	hSEmpty = CreateSemaphore(NULL, 0, BufferSize, 0);
	hSFull = CreateSemaphore(NULL, BufferSize, BufferSize, 0);
	hMutex = CreateSemaphore(NULL, 1, BufferSize, 0);
	CreateThread(0, 0, getkey, 0, 0, 0);

	for (int i = 0; i < cProducers; i++)
		hThreads[i] = CreateThread(0, 0, producer, Buf, 0, 0);
	for (int i = cProducers; i < cProducers + cConsumers; i++) /*Создание потоков-потребителей*/
		hThreads[i] = CreateThread(0, 0, consumer, Buf, 0, 0);

	WaitForMultipleObjects(cProducers + cConsumers, hThreads, true, INFINITE);
	CloseHandle(hSEmpty);
	CloseHandle(hSFull);
	CloseHandle(hMutex);
	cin.get();
	return 0;
}
