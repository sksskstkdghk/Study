#include <iostream>

using namespace std;

int main()
{
    int x, y;
    int px01 = 0, px02 = 0, py01 = 0, py02 = 0;
    int xCount = 0, yCount = 0;

    for (int i = 0; i < 3; i++)
    {
        cin >> x >> y;

        //scanf("%d%d", &x, &y);

        if (i == 0)
        {
            px01 = x;
            py01 = y;
            continue;
        }

        if (px01 != x)
        {
            xCount++;
            px02 = x;
        }
        if (py01 != y) 
        {
            yCount++;
            py02 = y;
        }
    }

    if (xCount < 2) px01 = px02;
    if (yCount < 2) py01 = py02;

    cout << px01 << " " << py01;
    //printf("%d %d", px01, py01);
}