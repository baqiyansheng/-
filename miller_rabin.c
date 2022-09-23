#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define MAX 512
int dividend[512] = {0}, m1[512] = {2}, m2[512] = {7}, m3[512] = {1, 6}, result1[512] = {0}, result2[512] = {0}, ans[512] = {0};
// dividend是2^p-1,m1,m2,m3是选取的底数
int min_0[512] = {0}, min_1[512] = {1};
int prime[512] = {0}, t1[512] = {0}, t2[512] = {0}, x1[512] = {0}, len0 = 0, len1 = 0, len2 = 0, len3 = 1;
// prime存储1000以内所有素数,t1暂存被除数，t2暂存除数。
// len0是被除数最高位在数组中的下标，len1,len2,len3是底数的最高位下标
void Big_num(int p[]) //计算2^p，每次乘2
{
    int j = 0, t = 0;           // j是算到第几位,len0是2^p的最高位下标
    while (j <= len0 || t == 1) //还未算到最高位或者是没有完成进位
    {
        if (2 * (p[j]) >= 10) //该位乘2后要进位
        {
            p[j] = 2 * (p[j]) % 10; //乘2取余
            if (t == 1)             //上一位进位
            {
                p[j] += 1;
                t = 0; //除去进位标记
            }
            t = 1; //标记进位
            if (j + 1 > len0)
                len0++; //乘2后位数变多，len1++
        }
        else //乘2不进位
        {
            if (p[j] == 0)
                p[j] = 0;
            p[j] = p[j] * 2;
            if (t == 1) //需要进位
            {
                p[j] += 1;
                t = 0;
            }
        }
        j++;
    }
}
int Prime_num(int s[]) //用于确定1000以内的所有奇素数
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
    return k - 1; //返回最大奇素数的下标
}
int Max(int t1[], int t2[], int len1, int len2) //判断t1是不是比t2大
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
void ShiftRight(int q[], int t2[], int len, int n) //将除数右移n位，即乘以10的n次方，结果放到t2里。
{
    int i;
    memset(t2, 0, 512 * sizeof(int));
    for (i = len; i >= 0; i--)
        t2[i + n] = q[i];
}
int Substract(int t1[], int t2[], int len1, int len2) //大整数t1减去t2。只需要运算len2位
{
    int high_pos = 0; //记录相减后的最高位
    int f = 0;        //标记是否够减
    for (int i = 0; i <= len1; i++)
    {
        t1[i] -= t2[i]; //逐位相减
        if (t1[i] < 0)  //看是否要借位
        {
            t1[i] += 10;
            t1[i + 1]--; //借位
        }
        if (t1[i])
            high_pos = i; //记录最高位的位置
    }
    return high_pos;
}
void Division(int p[], int x) //偶数对应的数组除以2
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
void Remainder(int p[], int q[], int r[], int x, int y) // 大整数除法,返回余数
{
    memset(r, 0, 512 * sizeof(int));
    int i, n = x - y, h = 0;  // n是需要移动的位数,h是相减后的最高位
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
void Minus_1(int p[], int x) //减一
{
    int f = 0, i = 0; //标记是否够减
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
void Plus_big(int p[], int q[], int result[], int x, int y) //大整数加法，result是结果
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
void Multiplication(int p[], int q[], int result[], int x, int y) //大整数乘法
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
void Q_mul(int p1[], int p2[], int mod[], int ans[], int x, int y, int z) //快速计算 (e*f) % mod,ans是结果
{
    int tem1[512] = {0}, tem2[512] = {0};
    memset(ans, 0, 512 * sizeof(int));   //重置
    memcpy(tem1, p1, 512 * sizeof(int)); //暂存
    memcpy(tem2, p2, 512 * sizeof(int)); //暂存
    while (Max(tem2, min_0, y, 0) == 1)  // p2>0
    {
        if (tem2[0] % 2 == 1) // p2是奇数
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
void Q_pow(int c[], int d[], int mod[], int ans[], int x, int y, int z) //快速计算 (c^d) % mod，ans是结果
{
    memcpy(ans, min_1, 512 * sizeof(int)); //重置为1
    int tem1[512] = {0}, tem2[512] = {0};  //暂存
    memcpy(tem1, c, 512 * sizeof(int));
    memcpy(tem2, d, 512 * sizeof(int));
    while (Max(tem2, min_0, len0, 0) == 1) // d>0
    {
        if (tem2[0] % 2 == 1) // d是奇数
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
int Witness(int a[], int n[], int x1[], int len) // Miller_rabin算法的精华,用检验算子a来检验n是不是素数
{

    int tem[512] = {0}, i, copy[512] = {0};

    memcpy(tem, n, 512 * sizeof(int)); //将n-1存入tem，作为Miller_rabin算法中的指数
    tem[0]--;
    memcpy(copy, tem, 512 * sizeof(int));
    int j = 0;
    while (tem[0] % 2 == 0) //将n-1拆分为a^r * s
    {
        Division(tem, len0);
        j++;
    }
    Q_pow(a, tem, n, x1, len, len0, len0); //得到a^tem% n
    if (Max(x1, min_1, len0, 0) == 0 || Max(x1, copy, len0, len0) == 0)
        return 1; //余数为1则为素数
    while (j--)   //否则试验条件2看是否有满足的 j
    {
        int t[512] = {0};
        Q_mul(x1, x1, n, t, len0, len0, len0);
        memcpy(x1, t, 512 * sizeof(int));
        if (Max(x1, copy, len0, len0) == 0)
            return 1;
    }
    return 0;
}
int Miller_rabin(int n[]) //检验n是否是素数
{
    int i;
    //下面依次用m1,m2,m3检验mod
    if (!Witness(m1, n, x1, 0)) //用m1检验n是否是素数
        return 0;
    else
    {
        if (Max(n, m2, len0, 0) == 1)
        {
            if (!Witness(m2, n, x1, 0)) //用m2检验n是否是素数
                return 0;
            else
            {
                if (Max(n, m3, len0, 1) == 1)
                {
                    if (!Witness(m3, n, x1, 1)) //用m3检验n是否是素数
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
    int i, p, len2 = 0, f = 0, j = 0, k; // p是指数
    dividend[0] = 2;                     // 2^1
    k = Prime_num(prime);                //找出1000以内的素数
    for (p = 2; p < prime[k]; p++)
    {
        f = 1;
        Big_num(dividend); //无论p是否是素数，先乘2得到被除数
        if (p == 2)
        {
            printf("2\n3\n\n"); //对偶素数2特判
            continue;
        }
        if (p == prime[j]) //先筛选出所有的奇素数p在进行检验
        {
            j++; //准备下一次的比对，以找到下一个素数
            int mod[512] = {0};
            memcpy(mod, dividend, 512 * sizeof(int)); //将2^p-1存入mod，作为Miller_rabin算法中被检验的素数p
            mod[0]--;
            if (Miller_rabin(mod)) // p满足条件，进行打印
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
