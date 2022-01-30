#include <iostream>

using namespace std;

int main()
{
    int n, result = 0;
    bool eratos[1001];

    cin >> n;

    for (int i = 0; i <= 1000; i++)
        eratos[i] = true;

    for (int i = 2; i <= 1000; i++)
    {
        if (eratos[i])
            for (int j = i * i; j <= 1000; j += i)
                eratos[j] = false;
    }

    for (int i = 0; i < n; i++)
    {
        int a;
        cin >> a;

        if (a < 2)
            continue;

        if (eratos[a])
            result++;
    }

    cout << result;
}