// 11653.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <math.h>

#define SIZE 3162 //루트 천만의 값
#define PN_SIZE 446 //SIZE사이의 소수 갯수

using namespace std;

int main()
{
    bool eratos[SIZE] = {1};
    int PN[PN_SIZE], a = 0, n;

    eratos[0] = eratos[1] = false;

    for (int i = 2; i < SIZE; i++)
        eratos[i] = true;

    for (int i = 2; i < SIZE; i++)
    {
        if (eratos[i])
        {
            for (int j = i * i; j < SIZE; j+= i)
                eratos[j] = false;

            PN[a++] = i;
        }
    }

    cin >> n;

    bool temp = true;

    for (int i = 0; i < PN_SIZE; i++)
    {
        if (n < 2)
        {
            temp = false;
            break;
        }

        if (n % PN[i] == 0)
        {
            n /= PN[i];

            cout << PN[i] << endl;

            i = -1;
        }
    }

    if(temp)
        cout << n;
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
