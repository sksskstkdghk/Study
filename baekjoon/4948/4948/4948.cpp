#include <iostream>

using namespace std;

#define SIZE 123456 * 2

int eratos[SIZE + 1] = { 0 };

int main()
{
    int n;

    eratos[1] = 1;

    for (long long i = 2; i <= SIZE; i++)
        if (eratos[i] == 0)
            for (long long j = i * i; j <= SIZE; j += i)
                eratos[j] = 1;

    do
    {
        int count = 0;
        cin >> n;

        for (int i = n + 1; i <= 2 * n; i++)
            if (eratos[i] == 0) count++;

        if (count > 0)
            cout << count << endl;
            //printf("%d\n", count);

    } while (n > 0);
}