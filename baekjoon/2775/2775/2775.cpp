#include <iostream>

using namespace std;

int Num(int h, int w)
{
    if (h == 0)
        return w;
    else if (w == 1)
        return 1;
    else
        return Num(h - 1, w) + Num(h, w - 1);
}

int main()
{
    int a, k, n;

    cin >> a;

    for (int i = 0; i < a; i++)
    {
        cin >> k >> n;

        cout << Num(k, n) << "\n";
    }
}