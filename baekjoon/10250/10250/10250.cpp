#include <iostream>

using namespace std;

int main()
{
    int a, H, W, N;

    cin >> a;

    for (int i = 0; i < a; i++)
    {
        cin >> H >> W >> N;

        int roomYY, roomXX;

        if (N % H == 0)
        {
            roomYY = H;
            roomXX = N / H;
        }
        else
        {
            roomXX = (N / H) + 1;
            roomYY = N % H;
        }

        cout << roomYY;

        if (roomXX < 10) cout << 0;

        cout << roomXX << "\n";
    }
}