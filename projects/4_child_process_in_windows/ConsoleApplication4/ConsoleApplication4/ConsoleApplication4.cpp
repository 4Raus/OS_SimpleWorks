#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>

using namespace std;

int main() {
    HANDLE hChildSINR, hChildSINW;
    SECURITY_ATTRIBUTES sa;
    PROCESS_INFORMATION pi;
    STARTUPINFO si;

    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    string fl;
    try {
        if (!CreatePipe(&hChildSINR, &hChildSINW, &sa, 0)) { throw fl = "///FailedCreatePipe///"; }

        GetStartupInfo(&si);
        si.hStdInput = hChildSINR;
        si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
        si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        si.dwFlags |= STARTF_USESTDHANDLES;

        int num1, num2;
        char end;
        cout << "Enter two integer numbers: ";

        if (scanf("%d%d%c", &num1, &num2, &end) != 3 || end != '\n') { throw fl = "///FailedDataInput///"; }

        DWORD bytesWritten1, bytesWritten2;
        WriteFile(hChildSINW, &num1, sizeof(num1), &bytesWritten1, NULL);
        WriteFile(hChildSINW, &num2, sizeof(num2), &bytesWritten2, NULL);

        if (!WriteFile(hChildSINW, &num1, sizeof(num1), NULL, NULL)) { throw fl = "///FailedWriteFile1///"; }
        if (!WriteFile(hChildSINW, &num2, sizeof(num2), NULL, NULL)) { throw fl = "///FailedWriteFile2///"; }
        if (!CreateProcess(L"Child.exe", NULL, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) { throw fl = "///FailedCreateProcess///"; }

        CloseHandle(hChildSINW);
        WaitForSingleObject(pi.hProcess, INFINITE);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        CloseHandle(hChildSINR);
    }
    catch (string fl) { cerr << fl << endl; }

    return 0;
}