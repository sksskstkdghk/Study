// 10757.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <math.h>
#include <string>
#include <cstring>

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

#define ll long long
#define CIPHER_CNT 19

static const ll CIPHER = pow(10, CIPHER_CNT);

typedef struct _tagBigInter
{
    ll nums[556] = {};
    int numSize[556] = {};
    int size = 0;
}BI;

void strcopy(char* dest, const char* source, int size)
{
    for (int i = 0; i < size; i++)
    {
        dest[i] = source[i];
    }
}

BI str_to_bi(string str)
{
    BI bi;
    int index = 0;

    for (int i = 0; i < str.size(); i += 18)
    {
        char temp[19] = {};
        int a;

        if (i + 17 < str.size())
            a = 18;
        else
            a = str.size() - i;

        strcopy(temp, str.c_str() + i, a);

        bi.nums[index] = stoll(temp);

        ll b = bi.nums[index], c = 0;
        while (b != 0)
        {
            b /= 10;
            c++;
        }

        bi.numSize[index] = a - c;

        index++;
    }

    bi.size = index;

    return bi;
}

BI Bi_plus_Bi(BI a, BI b)
{
    BI bi;
    int i = 0;

    while (i < a.size || i < b.size)
    {
        bi.nums[i] += a.nums[i] + b.nums[i];
        bi.numSize[i] = (a.numSize[i] < b.numSize[i]) ? b.numSize[i] : a.numSize[i];

        bi.nums[i + 1] += bi.nums[i] / CIPHER;
        bi.nums[i] %= CIPHER;

        ll bb = bi.nums[i], c = 0;
        while (bb != 0)
        {
            bb /= 10;
            c++;
        }

        bi.numSize[i] -= c;

        i++;
    }

    bi.size = i;

    return bi;
}

int main()
{
    char str1[10002] = {}, str2[10002] = {  }, strResult[10002] = {  };

    /*string a, b;
    BI abi, bbi, result;

    cin >> a >> b;

    abi = str_to_bi(a);
    bbi = str_to_bi(b);

    result = Bi_plus_Bi(abi, bbi);*/

    int str1Len, str2Len;

    cin >> str1 + 1 >> str2 + 1;

    str1[0] = str2[0] = '0';
    int max, plus = 0;
    bool temp = false;

    max = (str1Len > str2Len) ? str1Len : str2Len;

    for (int i = 1; i <= max; i++)
    {
        int a = 0, b = 0, result = 0;

        if (str1Len - i > 0)
            a = str1[str1Len - i] - '0';

        if (str2Len - i > 0)
            b = str2[str2Len - i] - '0';

        result = a + b + plus;

        plus = result / 10;
        result %= 10;

        strResult[max - i] = result + '0';
    }

    int str3Len = strlen(strResult);

    for (int i = 0; i < str3Len; i++)
    {
        if (strResult[i] == '0' && !temp)
            continue;

        cout << strResult[i];
        temp = true;
    }

    cout << "\n";
}