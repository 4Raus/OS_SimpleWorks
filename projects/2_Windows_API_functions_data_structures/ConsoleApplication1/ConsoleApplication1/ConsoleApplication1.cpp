#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <tchar.h>

using namespace std;

int main() {
    setlocale(LC_ALL, "");
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    int choice;
    DWORD pid;
    HANDLE hProcess; // Объявляем переменную hProcess здесь

    cout << "Выберите способ создания процесса:\n";
    cout << "1 - CreateProcess\n";
    cout << "2 - WinExec\n";
    cout << "3 - ShellExecute\n";

    cin >> choice;

    switch (choice) {
    case 1:
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        if (!CreateProcess(_T("C:\\Windows\\System32\\notepad.exe"), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            cout << "Ошибка CreateProcess, код: " << GetLastError() << endl;
            return 1;
        }

        cout << "Идентификатор процесса: " << pi.dwProcessId << endl;

        // Получаем временные характеристики
        FILETIME creationTime, exitTime, kernelTime, userTime;
        GetProcessTimes(pi.hProcess, &creationTime, &exitTime, &kernelTime, &userTime);

        // Завершаем процесс
        TerminateProcess(pi.hProcess, 0);

        // Закрываем дескрипторы
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);

        break;

    case 2:
        // Запускаем notepad через WinExec
        pid = (DWORD)WinExec("notepad.exe", SW_SHOW);

        if (pid <= 32) {
            cout << "Ошибка WinExec, код: " << pid << endl;
            return 1;
        }

        cout << "Введите идентификатор процесса: ";
        cin >> pid;

        // Открываем процесс по идентификатору
        hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);

        if (hProcess == NULL) {
            cout << "Ошибка OpenProcess, код: " << GetLastError() << endl;
            return 1;
        }

        // Получаем временные характеристики
        GetProcessTimes(hProcess, &creationTime, &exitTime, &kernelTime, &userTime);

        // Завершаем процесс
        TerminateProcess(hProcess, 0);

        // Закрываем дескриптор
        CloseHandle(hProcess);

        break;

    case 3:

        // Запускаем notepad через ShellExecute
        if ((int)ShellExecute(NULL, _T("open"), _T("notepad.exe"), NULL, NULL, SW_SHOW) <= 32) {
            cout << "Ошибка ShellExecute, код: " << GetLastError() << endl;
            return 1;
        }

        cout << "Введите идентификатор процесса: ";
        cin >> pid;

        // Открываем процесс по идентификатору
        hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);

        if (hProcess == NULL) {
            cout << "Ошибка OpenProcess, код: " << GetLastError() << endl;
            return 1;
        }

        // Получаем временные характеристики
        GetProcessTimes(hProcess, &creationTime, &exitTime, &kernelTime, &userTime);

        // Завершаем процесс
        TerminateProcess(hProcess, 0);

        // Закрываем дескриптор
        CloseHandle(hProcess);

        break;

    default:
        cout << "Неверный выбор!" << endl;
    }

    return 0;
}
