// 1929.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

//#include <time.h>

#include <iostream>
//#include <math.h>
//
using namespace std;
//
//int main()
//{
//    int m, n, a = 0;
//
//    cin >> m >> n;
//
//    //time_t t = time(NULL);
//
//    bool eratos[1001];
//    int PN[100] = {};
//    int b = (int)sqrt(n);
//
//    eratos[0] = eratos[1] = false;
//
//    for (int i = 2; i <= b; i++)
//        eratos[i] = true;
//
//    for (int i = 2; i <= b; i++)
//        if (eratos[i])
//        {
//            for (int j = i * i; j <= b; j += i)
//            {
//                eratos[j] = false;
//
//            }
//
//            PN[a++] = i;
//        }
//            
//
//    for (int i = m; i <= n; i++)
//    {
//        int j = 0;
//        bool temp = true;
//
//        while (j < a)
//        {
//            if (i % PN[j] == 0 && i / PN[j] > 1)
//            {
//                temp = false;
//                break;
//            }
//
//            j++;
//        }
//
//        if (temp)
//            printf("%d\n", i);
//            //cout << i << endl;
//    }
//}

int eratos[1000001] = { 0 };

int main()
{
    int m, n;

    cin >> m >> n;
    eratos[1] = 1;

    //time_t t = time(NULL);

    for (long long i = 2; i <= n; i++)
        if (eratos[i] == 0)
            for (long long j = i * i; j <= n; j += i)
                eratos[j] = 1;

    for (int i = m; i <= n; i++)
    {
        if (eratos[i] == 0)
            printf("%d\n", i);
    }

    //printf("%ll", t - time(NULL));
    //cout << t - time(NULL);
}