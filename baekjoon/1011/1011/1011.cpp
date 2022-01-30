#include <iostream>
#include <math.h>

using namespace std;

int main()
{
    int n;

    cin >> n;

    for (int i = 0; i < n; i++)
    {
        long long a, b, k;

        cin >> a >> b;

        k = (int)sqrt(b - a);

        if ((b - a) == k * k)
            cout << 2 * k - 1 << endl;
        else if (k * k < (b - a) && k * k + k >= (b - a))
            cout << 2 * k << endl;
        else if (k * k + k < (b - a) && (k + 1) * (k + 1) > (b - a))
            cout << 2 * k + 1 << endl;
    }
}