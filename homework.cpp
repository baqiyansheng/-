#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define Status int
#define OK 1
#define ERROR 0
#define OVERFLOW -1
#define Maxlength 1000 //数组的最大位数，也代表着精度

typedef struct // N进制高精度数的结构体
{
    int Sign;                  //符号(正负号,0为正，1为负)
    int Base;                  //进制
    int Num_Int;               //整数位数，1开始
    int Num_Dec;               //小数位数，1开始
    int Int_String[Maxlength]; //整数部分字符串,第一位是最靠近小数点的一位
    int Dec_String[Maxlength]; //小数部分字符串
} High_Pre_Num;

int max(int a, int b) //辅助函数，返回较大值
{
    if (a >= b)
        return a;
    else
        return b;
}

High_Pre_Num Input() //输入一个n进制高精度小数的串，返回构造出来的结构体
{
    int n, f = 0; // n是进制，f标记小数点出现的下标
    printf("请输入进制: ");
    scanf("%d", &n);
    getchar(); //把换行符读取
    printf("请输入数值：");
    int i = 0, j = 0;
    char s[2 * Maxlength + 10] = { 0 }; //稍微开大一点，留足空间
    fgets(s, 256, stdin);             //读取串
    High_Pre_Num a = { 0, n, 0, 0, {0}, {0} };
    if (s[0] == '-')
    {
        a.Sign = 1; //标记为负数
        i++;
    }
    else
        a.Sign = 0;
    while (s[i] != '.' && s[i] != '\n') //用于计算整数位数，到串结束或者小数点为止，如果整数部分为0，仍然有一位整数
        i++;
    a.Num_Int = i;   //小数点下标或者换行符下标即为整数位数
    if (a.Sign == 1) //带负号会占用一个整数位的位置，所以需要位数减一
        a.Num_Int--;
    if (s[i] == '.') //即有小数部分
        f = i;       //标记小数点位置

    //从s的第一位开始记录整数部分，到小数点或者字符串尾截止，退出循环时，i为小数点或者换行符的下标
    if (a.Sign == 0) //正数，整数部分最后一位下标为Num_Int-1
    {
        for (i = a.Num_Int - 1; i >= 0; i--)
        {
            if (n > 10 && s[i] > '9') //十以上进制会出现ABC等
            {
                if (s[i] - 'A' > n - 11) //出现不合法字符
                    exit(-1);
                a.Int_String[a.Num_Int - 1 - i] = s[i] - 'A';
            }
            else
                a.Int_String[a.Num_Int - 1 - i] = s[i] - '0';
        }
    }
    else //负数，整数部分最后一位下标为Num_Int（因为有负号）
    {
        for (i = a.Num_Int; i >= 1; i--) //从s的第一位开始记录整数部分，到小数点或者字符串尾截止，退出循环时，i为小数点或者换行符的下标
        {
            if (n > 10 && s[i] > '9') //十以上进制会出现ABC等
            {
                if (s[i] - 'A' > n - 11) //出现不合法字符
                    exit(-1);
                a.Int_String[a.Num_Int - i] = s[i] - 'A';
            }
            else
                a.Int_String[a.Num_Int - i] = s[i] - '0';
        }
    }
    if (s[f] == '.') //说明有小数部分
    {
        i = f + 1;                                          // i移位到小数点后第一位,也就是小数第一位
        for (j = 0; s[i + j] != '\n' && j < Maxlength; j++) //从小数点后第一位开始，到'\0’结束，把小数部分存入串中
        {
            if (n > 10 && s[i] > '9') //十以上进制会出现ABC等
            {
                if (s[i] - 'A' > n - 11) //出现不合法字符
                    exit(-1);
                a.Dec_String[j] = s[i + j] - 'A';
            }
            else
                a.Dec_String[j] = s[i + j] - '0'; //因为题目要求中没有输入10以上进制的数，因此默认输入的字符串中只有0-9以及小数点
        }
        a.Num_Dec = j;
    }
    return a;
}

void Output(High_Pre_Num a) //输出一个高精度N进制的数
{
    int i = 0;
    printf("%d进制数为：", a.Base);
    if (a.Sign == 1) //负数
        printf("-");
    for (i = a.Num_Int - 1; i >= 0; i--) //输出整数部分
    {
        if (a.Base > 10 && a.Int_String[i] > 9)
            printf("%c", a.Int_String[i] - 10 + 'A'); //十进制以上需要输出A-F

        else
            printf("%d", a.Int_String[i]);
    }

    if (a.Num_Dec) //小数部分位数不为0，即有小数
        printf(".");
    for (i = 0; i < a.Num_Dec; i++) //输出小数部分
    {
        if (a.Base > 10 && a.Dec_String[i] > 9)
            printf("%c", a.Dec_String[i] - 10 + 'A'); //十进制以上需要输出A-F

        else
            printf("%d", a.Dec_String[i]);
    }
    printf("\n");
}

//小数部分的乘法,长度为len的小数部分串乘y,y同时也是转换的目标进制,flag标记转换是否全部完成即完成最后的进位以后其余位全部为0
int Times_Dec(int a[], int len, int x, int y, int* flag)
{
    int i, f = 0; // f为进位
    for (i = len - 1; i >= 0; i--)
    {
        a[i] *= y; //从小数最后一位开始每一位乘y
        a[i] += f; //加上来自后面一位的进位
        f = 0;
        if (a[i] >= x) //转换成x进制需要进位
        {
            f = a[i] / x;
            a[i] = a[i] % x;
        }
    }
    int j;
    for (j = 0; j < len; j++) //检测小数部分是否全部为0
    {
        if (a[j] != 0)
            break;
        else if (j == len - 1)
            *flag = 0;
    }
    return f; //最后的f就是取整结果
}

// x进制转换为y进制的思路是：整数部分从最高位开始，除以y，分别保留取整结果和余数。处理下一位时，需要加 上一位的余数乘x，再作除法
//小数部分转换则是小数部分整体乘y然后取整数作为转换后的小数位。
//例如10进制的0.25转2进制，转换方式是每次乘2取整数部分作为2进制的小数部分
//整数与小数的处理基本互为逆过程
High_Pre_Num Transform(High_Pre_Num a, int x, int y) //进制转换，把x进制高精度数a转换为y进制数
{
    int i = 0, j = 0, s = 0, f = 0; // f用来标记t中是否全部为0
    High_Pre_Num b = { a.Sign, y, 0, 0, {0}, {0} };
    if (a.Num_Int) // a的整数部分不为0
    {
        int t[Maxlength] = { 0 }, r[Maxlength] = { 0 };    // t用来存储每次除法后的产生的结果，r用来存储每一位的余数
        memcpy(t, a.Int_String, sizeof(a.Int_String)); //将整数部分的数组复制到t中
        do
        {
            for (i = a.Num_Int - 1; i >= 0; i--)
            {
                if (i != a.Num_Int - 1)
                    t[i] += r[i + 1] * x; //当正在转换的不是最高位的时候，需要加 上一位的余数*x
                r[i] = t[i] % y;          //保存余数
                t[i] /= y;                //保留除法结果
            }
            b.Int_String[j] = r[0];
            j++;
            for (i = a.Num_Int; i >= 0; i--) // f标记是否整数部分全部为0
            {
                if (t[i] != 0)
                    break;
                else if (i == 0)
                    f = 1;
            }
        } while (!f && j < Maxlength); //注意转换后的结果不能数组越界
        b.Num_Int = j;                 //记录转换后的整数位数
    }
    if (a.Num_Dec) // a的小数部分不为0
    {
        int f = 1; // f标记转换是否结束
        i = 0;
        while (f)
        {
            b.Dec_String[i] = Times_Dec(a.Dec_String, a.Num_Dec, a.Base, b.Base, &f);
            i++;
            if (i == Maxlength) //避免越界
                break;
        }
        if (b.Dec_String[0] == 0 && i == 1) //小数部分全部为0
            i = 0;
        b.Num_Dec = i; //记录转换后的小数位数
    }
    return b;
}

void Output_Result(High_Pre_Num a) //加法减法乘法的输出，需要同时输出十进制结果
{
    int N;
    printf("需要转换成几进制");
    scanf("%d", &N);
    printf("结果的"); //输出10进制结果
    Output(a);
    printf("结果的");
    Output(Transform(a, a.Base, N)); //输出N进制结果
}

High_Pre_Num reverse(High_Pre_Num a) //类似于原码变反码，但是没有符号位的变化，将负数a的数值位全部取反
{
    if (a.Sign == 1) //负数的反码需要变化
    {
        int i;
        High_Pre_Num c = a;
        for (i = 0; i < c.Num_Int; i++) //数值位取反
        {
            if (c.Int_String[i] == 0)
                c.Int_String[i] = 1;
            else
                c.Int_String[i] = 0;
        }
        for (i = 0; i < c.Num_Dec; i++) //数值位取反
        {
            if (c.Dec_String[i] == 0)
                c.Dec_String[i] = 1;
            else
                c.Dec_String[i] = 0;
        }
        return c;
    }
    else
        return a; //正数反码是自身
}

High_Pre_Num Complement(High_Pre_Num a) //将a转换为补码
{
    int f = 1, i = 0; // f用于取反后加1以及进位
    if (a.Base != 2)  //二进制才有补码
        exit(1);
    if (a.Sign == 0)
        return a; //正数补码是自身
    else
    {
        High_Pre_Num c = reverse(a); //先变反码
        i = c.Num_Dec - 1;
        while (f && i >= 0) //进位还没结束或者小数部分还没算完就需要一直计算
        {
            c.Dec_String[i]++;        //进位处理
            if (c.Dec_String[i] == 2) //产生新的进位
            {
                c.Dec_String[i] = 0; // 1+1=0
                i--;                 //算下一位
                c.Num_Dec--;         //最后一位产生进位，最后一位小数变成0，所以小数位数减一
            }
            else
                f = 0; //进位计算结束，退出循环
        }
        i = 0;
        while (f && i <= c.Num_Int) //小数进位到了整数
        {
            c.Int_String[i]++;
            if (c.Int_String[i] == 2)
            {
                c.Int_String[i] = 0;
                i++;
                if (i == c.Num_Int && c.Int_String[i] == 1) //原本的最高位就是1又产生了进位，所以整数位数加一
                    c.Num_Int++;
            }
            else
                f = 0; //进位计算结束
        }
        return c;
    }
}

High_Pre_Num True(High_Pre_Num a) //类似于从补码得到原码，但是由于数组中没有符号位所以又略有不同
{
    if (a.Sign == 0) //正数补码原码相同
        return a;
    else
    {
        High_Pre_Num c = reverse(a); //数组没有符号位，直接整体取反
        int i = c.Num_Dec - 1, f = 1;
        while (f && i >= 0) //执行加一运算
        {
            c.Dec_String[i]++;
            if (c.Dec_String[i] == 2)
            {
                c.Dec_String[i] = 0;
                i--;
                c.Num_Dec--; //最后一位小数变成0，小数位数减一
            }
            else
                f = 0; //进位结束
        }
        i = 0;
        while (f && i <= c.Num_Int) //加一运算
        {
            c.Int_String[i]++;
            if (c.Int_String[i] == 2)
            {
                c.Int_String[i] = 0;
                i++;
                if (i == c.Num_Int && c.Int_String[i] == 1) //整数最高位原本是1，这里进位后，整数位数加一
                    c.Num_Int++;
            }
            else
                f = 0; //进位结束
        }
        return c;
    }
}

High_Pre_Num Binary_Plus(High_Pre_Num a, High_Pre_Num b) //二进制a，b执行加法运算
{
    if (a.Base != b.Base || a.Base != 2) //同为二进制才能计算
        exit(2);
    else
    {
        a = Complement(a); //先取补码
        // Output(Transform(True(a), 2, 10));
        b = Complement(b); //先取补码
        // Output(Transform(True(b), 2, 10));
        int max_IntNum = max(a.Num_Int, b.Num_Int); //记录最大位数方便后续补位并且扩大位宽，负数补码前面补任意数量的1，数值不变
        int max_DecNum = max(a.Num_Dec, b.Num_Dec);

        if (a.Sign == 1) // a是负数
        {
            int i;
            for (i = a.Num_Int; i <= max_IntNum; i++)
                a.Int_String[i] = 1; //补全位数
        }
        if (b.Sign == 1) // b是负数
        {
            int i;
            for (i = b.Num_Int; i <= max_IntNum; i++)
                b.Int_String[i] = 1; //补全位数
        }
        High_Pre_Num c = a;
        c.Num_Int = max_IntNum; //暂时认为结果的最大位数就是a，b的最大整数，小数位数
        c.Num_Dec = max_DecNum;
        int i, f = 0; // f为进位
        for (i = max_DecNum - 1; i >= 0; i--)
        {
            c.Dec_String[i] += b.Dec_String[i] + f; //按位做加法，并且加上进位
            f = 0;
            if (c.Dec_String[i] > 1) //进位
            {
                c.Dec_String[i] -= 2;
                f = 1;
            }
        }
        for (i = 0; i <= max_IntNum; i++)
        {
            c.Int_String[i] += b.Int_String[i] + f; //按位做加法，加上进位
            f = 0;
            if (c.Int_String[i] > 1) //进位
            {
                c.Int_String[i] -= 2;
                f = 1;
            }
            if (i == max_IntNum) //溢出处理，a,b整数最大下标是max_IntNum-1,这里取max_IntNum相当于扩大位宽,避免溢出导致计算错误
            {
                c.Num_Int = max_IntNum + 1;         //位数加一
                c.Sign = (f + c.Sign + b.Sign) % 2; //相当于补上了符号位进行加法运算，max_IntNum+1即为符号位
                //例如-2 + -6，4位补码是1010+1110 加法结果为11000 ，溢出后为0
                //但是如果扩大位宽111010+111110，结果为1111000， 溢出后为-8
                //在本程序中，-2由高到低被存储为(1)10，-6被存储为(1)110,取补码后为(1)10,(1)010,补全位数后为(1)1110,(1)1010，c=(1+1+f)1000
                //以上的括号中的数为符号位，但是并不在整数数组中，需要单独拿出来计算
            }
        }
        c = True(c);
        return c;
    }
}

High_Pre_Num Plus(High_Pre_Num a, High_Pre_Num b) //高精度数a，b执行加法,结果存在c里
{
    if (a.Base == b.Base) //进制相同才能加
    {
        int s = a.Base;
        a = Transform(a, a.Base, 2); //先转换成二进制
        b = Transform(b, b.Base, 2);
        High_Pre_Num c = Binary_Plus(a, b); //二进制加法
        c = Transform(c, c.Base, s);        //转换回原本进制
        return c;
    }
    else
        exit(3);
}

High_Pre_Num Minus(High_Pre_Num a, High_Pre_Num b) // N进制减法
{
    if (a.Base == b.Base) //进制相同才能减
    {
        int s = a.Base;
        a = Transform(a, a.Base, 2); //先转二进制
        b = Transform(b, b.Base, 2);
        if (b.Sign == 0) //减法变加法处理，b的符号位变号
            b.Sign = 1;
        else
            b.Sign = 0;
        High_Pre_Num c = Binary_Plus(a, b); //二进制加法
        c = Transform(c, c.Base, s);        //转换回原本进制
        return c;
    }
    else
        exit(4);
}

High_Pre_Num Binary_move(High_Pre_Num a, int m) //移位实现二进制乘法运算,m为正数时为小数点右移，为负数时小数点左移
{
    if (a.Base != 2) //非二进制不能移位
        exit(5);
    if (a.Num_Int == 0 && a.Int_String[0] == 0)
        a.Num_Int = 1;
    if (m >= 0) //右移
    {
        int s[Maxlength] = { 0 };
        int i = 0;
        for (i = 0; i < m; i++) //保存小数前m位
            s[i] = a.Dec_String[i];
        while (i < a.Num_Dec)
        {
            a.Dec_String[i - m] = a.Dec_String[i]; //小数部分向左移位
            i++;
        }
        i = a.Num_Dec - m;
        while (i < a.Num_Dec)
        {
            a.Dec_String[i] = 0; //其余置零
            i++;
        }
        i = a.Num_Int - 1;
        while (i >= 0)
        {
            a.Int_String[i + m] = a.Int_String[i]; //整数部分向左移位
            i--;
        }
        i = m - 1;
        while (i >= 0) //原有的小数部分前几位变成整数部分后几位
        {
            a.Int_String[i] = s[m - 1 - i];
            i--;
        }
        a.Num_Int += m;    //位数变化
        a.Num_Dec -= m;    //位数变化
        if (a.Num_Dec < 0) //位数大于等于0
            a.Num_Dec = 0;
        if (a.Num_Int > Maxlength)
            a.Num_Int = Maxlength; //超出最大位数按最大位数算
        return a;
    }
    else //小数点左移
    {
        m = -m; //把负数变成正数
        int s[Maxlength] = { 0 };
        int i = 0;
        for (i = 0; i < m; i++) //保存整数后m位
            s[i] = a.Int_String[i];
        i = 0;
        while (i < a.Num_Int && i + m < Maxlength)
        {
            a.Int_String[i] = a.Int_String[i + m]; //整数向右移位
            i++;
        }
        if (a.Num_Int - m > 0)
            i = a.Num_Int - m;
        while (i < a.Num_Int)
        {
            a.Int_String[i] = 0; //其余置零
            i++;
        }
        i = a.Num_Dec - 1;
        if (i + m > Maxlength)
            i = Maxlength - 1 - m;
        while (i >= 0)
        {
            a.Dec_String[i + m] = a.Dec_String[i]; //小数部分右移
            i--;
        }
        i = m - 1;
        while (i >= 0) //原整数后几位变成小数前几位
        {
            a.Dec_String[i] = s[m - 1 - i];
            i--;
        }
        a.Num_Int -= m; //位数变化
        a.Num_Dec += m;
        if (a.Num_Int <= 0)
            a.Num_Int = 1;         //整数位数至少为1
        if (a.Num_Dec > Maxlength) //最大长度
            a.Num_Dec = Maxlength;
        return a;
    }
}

High_Pre_Num Binary_multi(High_Pre_Num a, High_Pre_Num b) //根据b的每一位是0还是1以及1的位置对a移位做加法来实现二进制乘法
{
    if (a.Base != 2 || b.Base != 2)
        exit(6);
    High_Pre_Num c = { 0, 2, 0, 0, {0}, {0} };

    int i = 0;
    for (i = 0; i < b.Num_Int; i++)
    {
        if (b.Int_String[i] == 1) // b整数位为1才需要移位，以执行乘法运算
        {
            High_Pre_Num t = Binary_move(a, i);
            c = Binary_Plus(c, t); //加到c上
        }
    }
    for (i = 0; i < b.Num_Dec; i++)
    {
        if (b.Dec_String[i] == 1)
        {
            High_Pre_Num t = Binary_move(a, -(i + 1)); // b的小数第一位为1则a的小数点左移1位
            c = Binary_Plus(c, t);
        }
    }
    c.Sign = a.Sign ^ b.Sign; // a,b符号相同为正，否则为负
    return c;
}

High_Pre_Num Multi(High_Pre_Num a, High_Pre_Num b) // N进制乘法
{
    if (a.Base != b.Base) //进制相同才能计算
        exit(7);
    int s = a.Base;
    a = Transform(a, a.Base, 2); //先转二进制
    b = Transform(b, b.Base, 2);
    High_Pre_Num c = Binary_multi(a, b); //二进制乘法
    c = Transform(c, 2, s);              //二进制变回原进制
    return c;
}

High_Pre_Num Trans_Frac(int a, int b) //把a/b转换为结构体
{
    High_Pre_Num c = { 0, 10, 0, 0, {0}, {0} };
    if (a < 0)
    {
        a = -a; //取正数
        c.Sign = 1;
    }
    int i = 0, x = 0;
    if (a >= b) // a比b大，转换成的结构体会有整数部分,整数部分处理与进制转换类似，每次求余数和除法结果
    {
        x = a / b;     //转换后整数部分
        a = a - b * x; //转换后小数部分对应的数
        while (x)
        {
            c.Int_String[i] = x % 10;
            x /= 10;
            i++;
        }
        c.Num_Int = i;
    }
    i = 0;
    while (a && i < Maxlength)
    {
        a *= 10;
        c.Dec_String[i] = a / b;
        a = a % b;
        i++;
    }
    c.Num_Dec = i;
    return c;
}

High_Pre_Num Polynomial(High_Pre_Num x, int a, int b, int c) //计算多项式中一项的值，a是系数的分子，b是系数的分母，c是指数,默认c>=0
{
    High_Pre_Num f = Trans_Frac(a, b);
    if (c > 0)
    {
        High_Pre_Num result = x;
        while (--c)
            result = Multi(result, x); //先算x^n
        result = Multi(result, f);     //再乘系数
        return result;
    }
    else          // c==0
        return f; //常数项
}

High_Pre_Num Input_Poly(High_Pre_Num x)
{
    char s[100] = { 0 };
    fgets(s, 100, stdin); //字符串中出现的字符可能有“+、-、0-9、x、/、^、.”
    int i = 0, max = strlen(s) - 1;
    int f = 0;               //标记系数的正负，0为正，1为负
    int flag = 0;            //标记系数是分数还是小数，0为分数，1为小数
    int a = 0, b = 0, c = 0; //依次是分子分母指数
    High_Pre_Num r = { 0, 10, 0, 0, {0}, {0} };
    while (i < max) //每次循环读取一整项
    {
        High_Pre_Num y = { f, 10, 0, 0, {0}, {0} };
        if (s[i] == '-')
        {
            f = 1; //读取负号
            i++;   //移位
        }
        else
        {
            if (s[i] == '+')
                i++;                        //移位
            if (s[i] >= '0' && s[i] <= '9') //读取分子,退出循环时，i对应分子最后一位
            {
                a = s[i] - '0';
                while (s[i + 1] >= '0' && s[i + 1] <= '9') //下一位还是分子部分
                {
                    a *= 10;
                    a += s[i + 1] - '0';
                    i++;
                }
                if (f == 1) //负号给分子
                {
                    a = -a;
                    f = 0;
                }
                i++; //分子读取结束，下一步读取分母或者是读取指数
            }

            if (s[i] == '/') //有‘/’说明有分母
            {
                i++; //移位到分母第一位
                b = 0;
                if (s[i] >= '0' && s[i] <= '9') //读取分母,退出循环时，i对应分母最后一位
                {
                    b = s[i] - '0';
                    while (s[i + 1] >= '0' && s[i + 1] <= '9') //下一位还是分子部分
                    {
                        b *= 10;
                        b += s[i + 1] - '0';
                        i++;
                    }
                }
                else
                    exit(-2);
                i++; //分母读取结束，下一步读取指数
            }

            if (s[i] == '.') //小数形式系数
            {
                int j = 0;                                         //记录小数点位置
                while (s[i - 1 - j] >= '0' && s[i - 1 - j] <= '9') //读取系数的整数部分
                {
                    y.Int_String[j] = s[i - 1 - j] - '0';
                    j++;
                }
                y.Num_Int = j;
                j = 0;
                while (s[i + 1 + j] >= '0' && s[i + 1 + j] <= '9') //读取系数的小数部分
                {
                    y.Dec_String[j] = s[i + 1 + j] - '0';
                    j++;
                }
                y.Num_Dec = j;
                i += j + 1;
                flag = 1; //标记系数为小数
            }
            if (s[i] == '*')
                i++;
            if (s[i] == 'x') //读取指数
            {
                i++;             //移位到'^'或者是下一个+ -
                if (s[i] == '^') //有指数符号，说明指数大于1
                {
                    i++; //移位到指数第一位
                    c = 0;
                    if (s[i] >= '0' && s[i] <= '9') //读取指数
                    {
                        c = s[i] - '0';
                        while (s[i + 1] >= '0' && s[i + 1] <= '9') //下一位还是指数部分
                        {
                            c *= 10;
                            c += s[i + 1] - '0';
                            i++;
                        }
                    }
                    else
                        exit(-2);
                    i++;
                }
                else //这一项x的指数为1,此时i对应的字符为加减或者换行符
                {
                    c = 1;
                }
            }
            if (a == 0 && b == 0) //无系数，分子分母都是1
            {
                a = 1;
                b = 1;
            }
            if (b == 0)
                b = 1; //无分母，默认为1
            if (flag == 0)
                r = Plus(r, Polynomial(x, a, b, c)); //加上多项式
            else
            {
                High_Pre_Num t = Polynomial(x, 1, 1, c); //先指定系数为1
                t = Multi(t, y);                         //乘系数
                r = Plus(r, t);
            }
            a = 0; //置零
            b = 0;
            c = 0;
            flag = 0;
        }
    }
    return r;
}
High_Pre_Num a, b, c;

int main()
{
    /*printf("高精度十进制数输出二进制结果\n");
    a = Input();
    Output(Transform(a, a.Base, 2));
    printf("\n");

    printf("高精度二进制数输出十进制结果\n");
    b = Input();
    b = Transform(b, b.Base, 10);
    Output(b);
    printf("\n");

    printf("高精度十进制数输出N进制结果\n");
    c = Input();
    int N = 0;
    printf("需要转化为几进制？");
    scanf("%d", &N);
    Output(Transform(c, c.Base, N));
    printf("\n");

    printf("高精度十进制数输出N进制结果\n");
    c = Input();
    printf("需要转化为几进制？");
    scanf("%d", &N);
    Output(Transform(c, c.Base, N));
    printf("\n");

    printf("10进制数的加法\n");
    a = Input();
    b = Input();
    c = Plus(a, b);
    Output_Result(c);
    printf("\n");

    printf("10进制减法\n");
    c = Minus(a, b);
    Output_Result(c);
    printf("\n");

    printf("10进制乘法\n");
    c = Multi(a, b);
    Output_Result(c);
    printf("\n");*/

    printf("单变量多项式计算\n");
    printf("输入x的值：\n");
    High_Pre_Num x = Input();
    printf("多项式为：");
    High_Pre_Num result = Input_Poly(x);
    Output_Result(result);
    printf("\n");

    printf("单变量多项式计算\n");
    printf("输入x的值：\n");
    x = Input();
    printf("多项式为：");
    result = Input_Poly(x);
    Output_Result(result);
    printf("\n");

    system("pause");
    return 0;
}