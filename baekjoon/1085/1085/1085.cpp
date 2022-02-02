#include <iostream>

using namespace std;

int main()
{
    int x, y, w, h;
    int min01, min02;

    cin >> x >> y >> w >> h;

    min01 = (w - x < x) ? w - x : x;
    min02 = (h - y < y) ? h - y : y;

    if(min01 < min02)
        cout << min01;
    else
        cout << min02;
}