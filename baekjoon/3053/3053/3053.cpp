#include <iostream>

using namespace std;

#define PI 3.14159265359

int main()
{
    double a;

    cin >> a;

    printf("%.6f\n", (a * a) * PI);
    printf("%.6f\n", (a * a) * 2.0f);

    //cout << (a * a) * PI << endl;
    //cout << (a * a) * 2 << endl;
}