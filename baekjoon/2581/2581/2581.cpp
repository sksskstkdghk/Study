#include <iostream>

using namespace std;

int main()
{
    int min = 0, sum = 0;
    int a, b;
    bool eratos[10001] = {};

    cin >> a >> b;

    for (int i = 0; i <= b; i++)
        eratos[i] = true;

    for (int i = 2; i <= b; i++)
    {
        if (eratos[i])
            for (int j = i * i; j <= b; j += i)
                eratos[j] = false;
    }

    for (int i = a; i <= b; i++)
    {
        if (i < 2)
            continue;

        if (eratos[i])
        {
            sum += i;

            if (min <= 0)
                min = i;
        }
    }

    if (sum <= 0)
        cout << -1;
    else
        cout << sum << "\n" << min << endl;
}