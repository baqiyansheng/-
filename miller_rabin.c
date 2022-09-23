#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define MAX 512
int dividend[512] = {0}, m1[512] = {2}, m2[512] = {7}, m3[512] = {1, 6}, result1[512] = {0}, result2[512] = {0}, ans[512] = {0};
// dividend��2^p-1,m1,m2,m3��ѡȡ�ĵ���
int min_0[512] = {0}, min_1[512] = {1};
int prime[512] = {0}, t1[512] = {0}, t2[512] = {0}, x1[512] = {0}, len0 = 0, len1 = 0, len2 = 0, len3 = 1;
// prime�洢1000������������,t1�ݴ汻������t2�ݴ������
// len0�Ǳ��������λ�������е��±꣬len1,len2,len3�ǵ��������λ�±�
void Big_num(int p[]) //����2^p��ÿ�γ�2
{
    int j = 0, t = 0;           // j���㵽�ڼ�λ,len0��2^p�����λ�±�
    while (j <= len0 || t == 1) //��δ�㵽���λ������û����ɽ�λ
    {
        if (2 * (p[j]) >= 10) //��λ��2��Ҫ��λ
        {
            p[j] = 2 * (p[j]) % 10; //��2ȡ��
            if (t == 1)             //��һλ��λ
            {
                p[j] += 1;
                t = 0; //��ȥ��λ���
            }
            t = 1; //��ǽ�λ
            if (j + 1 > len0)
                len0++; //��2��λ����࣬len1++
        }
        else //��2����λ
        {
            if (p[j] == 0)
                p[j] = 0;
            p[j] = p[j] * 2;
            if (t == 1) //��Ҫ��λ
            {
                p[j] += 1;
                t = 0;
            }
        }
        j++;
    }
}
int Prime_num(int s[]) //����ȷ��1000���ڵ�����������
{
    int n, j, k = 0, f;
    for (n = 2; n < 1000; n++)
    {
        f = 1;
        for (j = 2; j <= n / 2 + 1; j++)
            if (n % j == 0)
            {
                f = 0;
                break;
            }
        if (f)
        {
            s[k] = n;
            k++;
        }
    }
    return k - 1; //����������������±�
}
int Max(int t1[], int t2[], int len1, int len2) //�ж�t1�ǲ��Ǳ�t2��
{
    int i;
    if (len1 >= len2)
        i = len1;
    else
        i = len2;
    for (; i >= 0; i--)
    {
        if (t1[i] > t2[i])
            return 1;
        else if (t1[i] < t2[i])
            return -1;
    }
    return 0;
}
void ShiftRight(int q[], int t2[], int len, int n) //����������nλ��������10��n�η�������ŵ�t2�
{
    int i;
    memset(t2, 0, 512 * sizeof(int));
    for (i = len; i >= 0; i--)
        t2[i + n] = q[i];
}
int Substract(int t1[], int t2[], int len1, int len2) //������t1��ȥt2��ֻ��Ҫ����len2λ
{
    int high_pos = 0; //��¼���������λ
    int f = 0;        //����Ƿ񹻼�
    for (int i = 0; i <= len1; i++)
    {
        t1[i] -= t2[i]; //��λ���
        if (t1[i] < 0)  //���Ƿ�Ҫ��λ
        {
            t1[i] += 10;
            t1[i + 1]--; //��λ
        }
        if (t1[i])
            high_pos = i; //��¼���λ��λ��
    }
    return high_pos;
}
void Division(int p[], int x) //ż����Ӧ���������2
{
    int i = 0, f = 0;
    for (i = x; i >= 0; i--)
    {
        if (f == 1)
        {
            if (p[i] % 2 == 0)
                f--;
            p[i] = (10 + p[i]) / 2;
        }
        else
        {
            if (p[i] % 2 == 1)
                f = 1;
            p[i] = p[i] / 2;
        }
    }
}
void Remainder(int p[], int q[], int r[], int x, int y) // ����������,��������
{
    memset(r, 0, 512 * sizeof(int));
    int i, n = x - y, h = 0;  // n����Ҫ�ƶ���λ��,h�����������λ
    if (Max(p, q, x, y) == 1) // p>q
    {
        if (n >= 0)
            ShiftRight(q, t2, y, n);
        while (h >= 0 && n >= 0)
        {
            if (Max(p, t2, x, y) >= 0)
                h = Substract(p, t2, x, y);
            else
            {
                n--;
                if (n >= 0)
                    ShiftRight(q, t2, y, n);
            }
        }
        if ((h == 0 && n == 0) || Max(p, q, x, y) < 0)
            memcpy(r, p, 512 * sizeof(int));
    }
    else
    {
        if (Max(p, q, x, y) == 0)
            memset(p, 0, 512 * sizeof(int));
        memcpy(r, p, 512 * sizeof(int));
    }
}
void Minus_1(int p[], int x) //��һ
{
    int f = 0, i = 0; //����Ƿ񹻼�
    if (p[0] >= 1)
        p[0]--;
    else
    {
        p[0] = 9;
        i++;
        p[i]--;
        while (p[i] < 0)
        {
            p[i] += 10;
            p[i + 1]--;
            i++;
        }
    }
}
void Plus_big(int p[], int q[], int result[], int x, int y) //�������ӷ���result�ǽ��
{
    memset(result, 0, 512 * sizeof(int));
    int i;
    for (i = 0; i <= x + 1 || i <= y + 1; i++)
        result[i] = p[i] + q[i];
    for (i = 0; i <= x + 1 || i <= y + 1; i++)
        if (result[i] >= 10)
        {
            result[i + 1] += 1;
            result[i] %= 10;
        }
}
void Multiplication(int p[], int q[], int result[], int x, int y) //�������˷�
{
    memset(result, 0, 512 * sizeof(int));
    int i, j;
    for (i = 0; i <= x; i++)
        for (j = 0; j <= y; j++)
            result[i + j] += p[i] * q[j];
    for (i = 0; i <= 512; i++)
        if (result[i] >= 10)
        {
            result[i + 1] += result[i] / 10;
            result[i] %= 10;
        }
}
void Q_mul(int p1[], int p2[], int mod[], int ans[], int x, int y, int z) //���ټ��� (e*f) % mod,ans�ǽ��
{
    int tem1[512] = {0}, tem2[512] = {0};
    memset(ans, 0, 512 * sizeof(int));   //����
    memcpy(tem1, p1, 512 * sizeof(int)); //�ݴ�
    memcpy(tem2, p2, 512 * sizeof(int)); //�ݴ�
    while (Max(tem2, min_0, y, 0) == 1)  // p2>0
    {
        if (tem2[0] % 2 == 1) // p2������
        {
            Minus_1(tem2, y);
            Plus_big(ans, tem1, result1, 512, 512);
            Remainder(result1, mod, ans, x + 1, z);
        }
        Division(tem2, y);
        Plus_big(tem1, tem1, result1, x, x);
        Remainder(result1, mod, tem1, x + 1, z);
    }
}
void Q_pow(int c[], int d[], int mod[], int ans[], int x, int y, int z) //���ټ��� (c^d) % mod��ans�ǽ��
{
    memcpy(ans, min_1, 512 * sizeof(int)); //����Ϊ1
    int tem1[512] = {0}, tem2[512] = {0};  //�ݴ�
    memcpy(tem1, c, 512 * sizeof(int));
    memcpy(tem2, d, 512 * sizeof(int));
    while (Max(tem2, min_0, len0, 0) == 1) // d>0
    {
        if (tem2[0] % 2 == 1) // d������
        {
            Q_mul(ans, tem1, mod, result2, len0, len0, len0);
            memcpy(ans, result2, 512 * sizeof(int));
            Minus_1(tem2, len0);
        }
        Division(tem2, len0);
        Q_mul(tem1, tem1, mod, result2, len0, len0, len0);
        memcpy(tem1, result2, 512 * sizeof(int));
    }
}
int Witness(int a[], int n[], int x1[], int len) // Miller_rabin�㷨�ľ���,�ü�������a������n�ǲ�������
{

    int tem[512] = {0}, i, copy[512] = {0};

    memcpy(tem, n, 512 * sizeof(int)); //��n-1����tem����ΪMiller_rabin�㷨�е�ָ��
    tem[0]--;
    memcpy(copy, tem, 512 * sizeof(int));
    int j = 0;
    while (tem[0] % 2 == 0) //��n-1���Ϊa^r * s
    {
        Division(tem, len0);
        j++;
    }
    Q_pow(a, tem, n, x1, len, len0, len0); //�õ�a^tem% n
    if (Max(x1, min_1, len0, 0) == 0 || Max(x1, copy, len0, len0) == 0)
        return 1; //����Ϊ1��Ϊ����
    while (j--)   //������������2���Ƿ�������� j
    {
        int t[512] = {0};
        Q_mul(x1, x1, n, t, len0, len0, len0);
        memcpy(x1, t, 512 * sizeof(int));
        if (Max(x1, copy, len0, len0) == 0)
            return 1;
    }
    return 0;
}
int Miller_rabin(int n[]) //����n�Ƿ�������
{
    int i;
    //����������m1,m2,m3����mod
    if (!Witness(m1, n, x1, 0)) //��m1����n�Ƿ�������
        return 0;
    else
    {
        if (Max(n, m2, len0, 0) == 1)
        {
            if (!Witness(m2, n, x1, 0)) //��m2����n�Ƿ�������
                return 0;
            else
            {
                if (Max(n, m3, len0, 1) == 1)
                {
                    if (!Witness(m3, n, x1, 1)) //��m3����n�Ƿ�������
                        return 0;
                    else
                        return 1;
                }
            }
        }
    }
    return 1;
}
int main()
{
    time_t start, end;
    start = time(NULL);
    int i, p, len2 = 0, f = 0, j = 0, k; // p��ָ��
    dividend[0] = 2;                     // 2^1
    k = Prime_num(prime);                //�ҳ�1000���ڵ�����
    for (p = 2; p < prime[k]; p++)
    {
        f = 1;
        Big_num(dividend); //����p�Ƿ����������ȳ�2�õ�������
        if (p == 2)
        {
            printf("2\n3\n\n"); //��ż����2����
            continue;
        }
        if (p == prime[j]) //��ɸѡ�����е�������p�ڽ��м���
        {
            j++; //׼����һ�εıȶԣ����ҵ���һ������
            int mod[512] = {0};
            memcpy(mod, dividend, 512 * sizeof(int)); //��2^p-1����mod����ΪMiller_rabin�㷨�б����������p
            mod[0]--;
            if (Miller_rabin(mod)) // p�������������д�ӡ
            {
                printf("%d\n", p);
                for (int i = len0; i >= 0; i--)
                    if (i >= 0)
                        printf("%d", mod[i]);
                printf("\n\n");
            }
        }
    }
    end = time(NULL);
    printf("time=%d\n", difftime(end, start));
    system("pause");
    return 0;
}
