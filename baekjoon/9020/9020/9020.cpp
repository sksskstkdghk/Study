#include <iostream>

using namespace std;

int eratos[10001] = { 0 };

int main()
{
    int n, num;

    cin >> n;

    for (int i = 2; i <= 100; i++)
        if (eratos[i] == 0)
            for (int j = i * i; j <= 10000; j += i)
                eratos[j] = 1;

    for (int i = 0; i < n; i++)
    {
        //cin >> num;
        scanf("%d", &num);

        for (int j = num / 2; j > 1; j--)
        {
            if (eratos[j] == 0 && eratos[num - j] == 0)
            {
                //cout << j << " ";
                printf("%d %d\n", j, num - j);
                break;
            }
        }
        //cout << r << endl;
    }
}