#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

int main()
{
    int n;
    int x1, y1, r1, x2, y2, r2;

    cin >> n;

    for (int i =0; i<n; i++)
    {
        cin >> x1 >> y1 >> r1 >> x2 >> y2 >> r2;

        int x, y, dis, add, sub;

        x = abs(x1 - x2);
        y = abs(y1 - y2);
        add = r1 + r2;
        add *= add;
        sub = abs(r1 - r2);
        sub *= sub;

        dis = x * x + y * y;

        if (dis == 0 && r1 == r2)
            cout << -1;
        else if (sub > dis || add < dis)
            cout << 0;
        else if (add == dis || sub == dis)
            cout << 1;
        else if (add > dis && sub < dis)
            cout << 2;

        cout << endl;
    }
}