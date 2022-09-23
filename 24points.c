#include <stdio.h>
#include <stdlib.h>
int order[25][5] = {{0}};             // 4个数所有的排序可能
int flag = 0;                         //标记是否有解
char mark[4] = {'+', '-', '*', '/'};  //存储四个运算符
float cal(float x, float y, int mark) //用于计算简单的四则运算
{
    switch (mark)
    {
    case 0:
        return x + y;
    case 1:
        return x - y;
    case 2:
        return x * y;
    case 3:
        return x / y;
    }
}
float calculate_A(float a, float b, float c, float d, int mark1, int mark2, int mark3) // A类括号形式((a b)c)d
{
    float r1, r2, r3;
    r1 = cal(a, b, mark1);
    r2 = cal(r1, c, mark2);
    r3 = cal(r2, d, mark3);
    return r3;
}
float calculate_B(float a, float b, float c, float d, int mark1, int mark2, int mark3) // B类括号(a(b c))d
{
    float r1, r2, r3;
    r1 = cal(b, c, mark2);
    r2 = cal(a, r1, mark1);
    r3 = cal(r2, d, mark3);
    return r3;
}
float calculate_C(float a, float b, float c, float d, int mark1, int mark2, int mark3) // C类括号a(b(c d))
{
    float r1, r2, r3;
    r1 = cal(c, d, mark3);
    r2 = cal(b, r1, mark2);
    r3 = cal(a, r2, mark1);
    return r3;
}
float calculate_D(float a, float b, float c, float d, int mark1, int mark2, int mark3) // D类括号a((b c)d)
{
    float r1, r2, r3;
    r1 = cal(b, c, mark2);
    r2 = cal(r1, d, mark3);
    r3 = cal(a, r2, mark1);
    return r3;
}
float calculate_E(float a, float b, float c, float d, int mark1, int mark2, int mark3) // E类括号(a b) (c d)
{
    float r1, r2, r3;
    r1 = cal(a, b, mark1);
    r2 = cal(c, d, mark3);
    r3 = cal(r1, r2, mark2);
    return r3;
}
void get(int a, int b, int c, int d) //对4个操作数进行计算，循环每一种计算符号的可能性
{
    int mark1, mark2, mark3;
    for (mark1 = 0; mark1 < 4; mark1++)
    {
        for (mark2 = 0; mark2 < 4; mark2++)
        {
            for (mark3 = 0; mark3 < 4; mark3++)
            {
                if (calculate_A(a, b, c, d, mark1, mark2, mark3) == 24)
                {
                    printf("((%d%c%d)%c%d)%c%d=24\n", a, mark[mark1], b, mark[mark2], c, mark[mark3], d);
                    flag = 1;
                }
                if (calculate_B(a, b, c, d, mark1, mark2, mark3) == 24)
                {
                    printf("(%d%c(%d%c%d))%c%d=24\n", a, mark[mark1], b, mark[mark2], c, mark[mark3], d);
                    flag = 1;
                }
                if (calculate_C(a, b, c, d, mark1, mark2, mark3) == 24)
                {
                    printf("%d%c(%d%c(%d%c%d))=24\n", a, mark[mark1], b, mark[mark2], c, mark[mark3], d);
                    flag = 1;
                }
                if (calculate_D(a, b, c, d, mark1, mark2, mark3) == 24)
                {
                    printf("%d%c((%d%c%d)%c%d)=24\n", a, mark[mark1], b, mark[mark2], c, mark[mark3], d);
                    flag = 1;
                }
                if (calculate_E(a, b, c, d, mark1, mark2, mark3) == 24)
                {
                    printf("(%d%c%d)%c(%d%c%d)=24\n", a, mark[mark1], b, mark[mark2], c, mark[mark3], d);
                    flag = 1;
                }
            }
        }
    }
}
void sort(int a[]) //对4个数进行全排列，结果存储在order中
{
    int i, j, m, n, t = 1;
    for (i = 1; i <= 4; i++)
    {
        for (j = 1; j <= 4; j++)
        {
            if (j == i)
                continue;
            for (m = 1; m <= 4; m++)
            {
                if (m == i || m == j)
                    continue;
                for (n = 1; n <= 4; n++)
                {
                    if (n == i || n == j || n == m)
                        continue;
                    else
                    {
                        order[t][1] = a[i];
                        order[t][2] = a[j];
                        order[t][3] = a[m];
                        order[t][4] = a[n];
                        t++;
                    }
                }
            }
        }
    }
}
int main()
{
    int a, b, c, d, num[5] = {0}, f = 0;
    printf("请输入4个数字(1~20):\n");
    scanf("%d %d %d %d", &num[1], &num[2], &num[3], &num[4]);
    if ((num[1] >= 1 && num[1] <= 20) && (num[2] >= 1 && num[2] <= 20) && (num[3] >= 1 && num[3] <= 20) && (num[4] >= 1 && num[4] <= 20))
    {
        sort(num);
        for(int i = 1; i <= 24; i++) //循环每一种可能的排列顺序
        {
            a = order[i][1];
            b = order[i][2];
            c = order[i][3];
            d = order[i][4];
            get(a, b, c, d);
        }
        f = 1;
    }
    while (f == 0)
    {
        printf("输入错误，请重新输入4个数字(1~20):\n");
        scanf("%d %d %d %d", &a, &b, &c, &d);
        if ((num[1] >= 1 && num[1] <= 20) && (num[2] >= 1 && num[21] <= 20) && (num[3] >= 1 && num[3] <= 20) && (num[4] >= 1 && num[4] <= 20)) //对判断进行检验
        {
            sort(num);
            for (int i = 1; i <= 24; i++)
            {
                a = order[i][1];
                b = order[i][2];
                c = order[i][3];
                d = order[i][4];
                get(a, b, c, d);
            }
            f = 1;
        }
    }
    if (flag == 0)
        printf("算不出24点\n");
    else
        printf("以上是全部解\n");
    system("pause");
    return 0;
}
