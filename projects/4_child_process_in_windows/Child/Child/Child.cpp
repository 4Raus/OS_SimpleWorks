#include <iostream>
#include <Windows.h>
#include <cmath>

using namespace std;

int sum(int num1, int num2);
int product(int num1, int num2);
int difference(int num1, int num2);
int quotient(int num1, int num2);
int power(int num1, int num2);

int main() {

    int num_1 = 0, num_2 = 0;
    DWORD bytesRead1,  bytesRead2;
    HANDLE hStdin1 = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hStdin2 = GetStdHandle(STD_INPUT_HANDLE);

    if (!ReadFile(hStdin1, &num_1, sizeof(num_1), &bytesRead1, NULL)) {
        cerr << "////ReadFile FAILED////" << endl;
        return 1;
    }

    if (!ReadFile(hStdin2, &num_2, sizeof(num_2), &bytesRead2, NULL)) {
        cerr << "////ReadFile FAILED////" << endl;
        return 1;
    }

    cout << "Sum: " << sum(num_1, num_2) << endl;
    cout << "Product: " << product(num_1, num_2) << endl;
    cout << "Difference: " << difference(num_1, num_2) << endl;
    cout << "Quotient: " << quotient(num_1, num_2) << endl;
    cout << "Power: " << power(num_1, num_2) << endl;

    return 0;
}

int sum(int num1, int num2) { return num1 + num2; }
int product(int num1, int num2) { return num1 * num2; }
int difference(int num1, int num2) { return num1 - num2; }
int quotient(int num1, int num2) { int q; num2 != 0 ? q = num1 / num2 : q = 0; return q; }
int power(int num1, int num2) { return pow(num1, num2); }