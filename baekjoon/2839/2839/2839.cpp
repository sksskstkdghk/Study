// 2839.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>

using namespace std;

int main()
{
    int a;

    cin >> a;

    int result = 0, b = 0;

    if (a % 3 != 0)
    {
        b = 3 - a % 3;
        a -= b * 5;
    }

    if (a >= 0)
    {
        result = a / 3;
        result = (result / 5) * 3 + (result % 5);
        result += b;
    }
    else
        result = -1;

    cout << result << "\n";

}