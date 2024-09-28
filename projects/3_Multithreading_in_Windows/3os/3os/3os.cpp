#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <windows.h>
#include <chrono>

using namespace std;

vector<int> Data(10000);
vector<int> sortedD_1, sortedD_2;

void fillarray() {
    srand(static_cast<unsigned int>(time(0)));
    for (auto& n : Data) n = rand() % 1000;
}

void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort() {
    vector<int> temp(Data);
    auto start = chrono::high_resolution_clock::now();
    merge(temp, 0, (temp.size() - 1) / 2, temp.size() - 1);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "Merge sorting execution time: " << duration.count() << " ms" << endl;
    sortedD_1 = temp;
}

void quick(vector<int>& arr, int left, int right) {
    int i = left, j = right;
    int p = arr[(left + right) / 2];
    while (i <= j) {
        while (arr[i] < p) i++;
        while (arr[j] > p) j--;
        if (i <= j) {
            swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }
    if (left < i) quick(arr, left, j);
    if (i < right) quick(arr, i, right);
}

void quickSort() {
    vector<int> temp(Data);
    auto start = chrono::high_resolution_clock::now();
    quick(temp, 0, temp.size() - 1);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "Quick sorting execution time: " << duration.count() << " ms" << endl;
    sortedD_2 = temp;
}

int main() {
    system("chcp 1251>nul");
    fillarray();

    HANDLE thread1 = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)mergeSort, nullptr, CREATE_SUSPENDED, nullptr);
    HANDLE thread2 = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)quickSort, nullptr, CREATE_SUSPENDED, nullptr);
    cout << "Priority for Merge sort is high, for Quick sort is low\n";

    SetThreadPriority(thread1, THREAD_PRIORITY_HIGHEST);
    SetThreadPriority(thread2, THREAD_PRIORITY_LOWEST);

    ResumeThread(thread1);
    ResumeThread(thread2);

    WaitForSingleObject(thread1, INFINITE);
    WaitForSingleObject(thread2, INFINITE);
    CloseHandle(thread1);
    CloseHandle(thread2);

    cout << "Priority for Merge sort is low, for Quick sort is high\n";
    fillarray();

    HANDLE thread3 = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)mergeSort, nullptr, CREATE_SUSPENDED, nullptr);
    HANDLE thread4 = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)quickSort, nullptr, CREATE_SUSPENDED, nullptr);

    SetThreadPriority(thread3, THREAD_PRIORITY_LOWEST);
    SetThreadPriority(thread4, THREAD_PRIORITY_HIGHEST);

    ResumeThread(thread3);
    ResumeThread(thread4);

    WaitForSingleObject(thread3, INFINITE);
    WaitForSingleObject(thread4, INFINITE);
    CloseHandle(thread3);
    CloseHandle(thread4);

    cout << "Blocking dynamic changes\n";
    fillarray();

    HANDLE thread5 = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)mergeSort, nullptr, CREATE_SUSPENDED, nullptr);
    HANDLE thread6 = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)quickSort, nullptr, CREATE_SUSPENDED, nullptr);

    SetThreadPriority(thread5, THREAD_PRIORITY_NORMAL);
    SetThreadPriority(thread6, THREAD_PRIORITY_NORMAL);

    ResumeThread(thread5);
    ResumeThread(thread6);

    WaitForSingleObject(thread5, INFINITE);
    WaitForSingleObject(thread6, INFINITE);
    CloseHandle(thread5);
    CloseHandle(thread6);
    return 0;
}