#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <stdio.h>
#include <cstring>
using namespace std;
const double C1 = 0.53;
const double C2 = 10000;
FILE *fp = fopen("answer.txt", "w");
vector<int> queen;                        //记录第i行皇后所在的列
void initQueen(int N, vector<int> &queen) // 初始化queen
{
    vector<int> array(N);
    for (int i = 0; i < N; i++) //先把第i行的皇后放在第i列，保证行列上的皇后不冲突
        array[i] = i;
    queen.clear(); //重置
    while (N)      //随机交换
    {
        double t = rand() % N;
        queen.push_back(array[t]);
        array[t] = array[--N];
    }
}
long long compute_collisions(const vector<int> &queen, vector<int> &dn, vector<int> &dp) //计算总冲突组数
{
    for (int t = 0; t < 2 * queen.size(); t++)
        dn[t] = dp[t] = 0; //初始化
    for (int i = 0; i < (int)queen.size(); i++)
    {
        int j = queen.at(i);
        dn[i + j]++;
        dp[queen.size() - 1 + i - j]++;
    }
    long long collisions = 0;
    for (int k = 0; k < 2 * queen.size() - 1; k++)
    {
        collisions += dn[k] * (dn[k] - 1) / 2; //同一左斜线上的冲突皇后组数
        collisions += dp[k] * (dp[k] - 1) / 2; //同一右斜线上的冲突皇后组数
    }
    return collisions; //返回总冲突数
}
int compute_attacks(const vector<int> &queen, vector<int> &dn, vector<int> &dp, vector<int> &attack)
{
    attack.clear(); //初始化
    int attackedLines = 0;
    for (int i = 0; i < (int)queen.size(); i++)
    {
        int j = queen.at(i);
        if (dn.at(i + j) > 1 || dp.at(queen.size() - 1 + i - j) > 1) //该皇后所在的左斜线或者右斜线上存在其他皇后
        {
            attack.push_back(i); //记录这个皇后
            attackedLines++;     //计数
        }
    }
    return attackedLines;
}
int randInt(int k, int size) //从0-size-1之间随机出一个不等于k的数
{
    int val = rand() % size;
    while (val == k) //随机出了k，再次随机
        val = rand() % size;
    return val;
}
long long cacl_collisions(const vector<int> &queen, vector<int> &dn, vector<int> &dp, int a, int b) //计算交换后相较于交换前的冲突数变化
{
    int i, j, counts = 0;
    i = queen[a];
    j = queen[b];
    counts -= dn[a + i] - 1;
    counts -= dn[b + j] - 1;
    if (a + i == b + j) //两个皇后在同一左斜线上
        counts++;
    counts -= dp[queen.size() - 1 + a - i] - 1;
    counts -= dp[queen.size() - 1 + b - j] - 1;
    if (a - i == b - j) //两个皇后在同一右斜线上
        counts++;
    counts += dn[a + j];
    counts += dn[b + i];
    if (a + j == b + i) //两个皇后在同一左斜线上
        counts++;
    counts += dp[queen.size() - 1 + a - j];
    counts += dp[queen.size() - 1 + b - i];
    if (a - j == b - i) //两个皇后在同一右斜线上
        counts++;
    return counts;
}
void updateQueen(vector<int> &queen, vector<int> &dn, vector<int> &dp, int a, int b) //刷新位置，每条斜线上的皇后数
{
    int i = queen.at(a);
    int j = queen.at(b);
    dn[a + i]--;
    dn[b + j]--;
    dp[queen.size() - 1 + a - i]--;
    dp[queen.size() - 1 + b - j]--;
    int t = queen[a]; //交换皇后位置
    queen[a] = queen[b];
    queen[b] = t;
    dn[a + j]++;
    dn[b + i]++;
    dp[queen.size() - 1 + a - j]++;
    dp[queen.size() - 1 + b - i]++;
}
void printQueen(vector<int> &queen)
{

    for (int i = 0; i < (int)queen.size(); i++)
        fprintf(fp, "%d ", queen.at(i));
}
void Nqueens(int N)
{
    long long collisions = N; //总冲突皇后数
    int num_of_attack;
    int swapCount = 0;
    vector<int> attack;    //记录发生冲突的皇后
    vector<int> dn(2 * N); //每条左斜线上的皇后数，从左上角到右下角
    vector<int> dp(2 * N); //每条右斜线上的皇后数，从右上角到左下角
    if (N == 2 || N == 3)
    {
        cout << "无解" << endl;
        return;
    }
    while (collisions) //在冲突数降为0以前一直执行
    {
        initQueen(N, queen);                                    //初始化皇后排列
        collisions = compute_collisions(queen, dn, dp);         //计算冲突数
        num_of_attack = compute_attacks(queen, dn, dp, attack); //记录冲突的皇后
        int limit = C1 * collisions;                            //设定冲突上限
        int loopSteps = 0;
        while (loopSteps < C2 * N) //设定步数上限
        {
            for (int k = 0; k < num_of_attack; k++)
            {
                int i = attack.at(k);
                int j = randInt(i, N);
                long long c;
                if ((c = cacl_collisions(queen, dn, dp, i, j)) < 0) //交换后冲突数变少
                {
                    collisions += c;
                    updateQueen(queen, dn, dp, i, j);
                    swapCount++;
                    if (collisions == 0) //排列完毕
                        break;
                    if (collisions < limit)
                    {
                        limit = C1 * collisions;
                        num_of_attack = compute_attacks(queen, dn, dp, attack);
                    }
                }
            }
            if (collisions == 0)
                break;
            loopSteps += num_of_attack;
        }
    }
    cout << "交换次数: " << swapCount << endl;
}
void check(FILE *fc, int N)
{
    int i;
    vector<int> examine_1(N, 0);     //标记该列是否存在皇后
    vector<int> examine_2(2 * N, 0); //标记该左斜线是否存在皇后
    vector<int> examine_3(2 * N, 0); //标记该右斜线是否存在皇后
    for (i = 0; i < N; i++)
    {
        int t;
        fscanf(fc, "%d", &t);
        if (examine_1[t] != 1 && examine_2[i + t] != 1 && examine_3[N - 1 + i - t] != 1)
        {
            examine_1[t]++;
            examine_2[i + t]++;
            examine_3[N - 1 + i - t]++;
        }
        else
        {
            cout << "此解错误" << endl;
            fclose(fc);
            return;
        }
    }
    cout << "此解正确" << endl;
    fclose(fc);
}
int main()
{
    long long N = 10000000;
    clock_t startTime, endTime;
    cout << "请输入皇后的个数：";
    cin >> N;
    srand((unsigned)time(NULL));
    startTime = clock();
    Nqueens(N);
    endTime = clock();
    printQueen(queen);
    double totalTime = 1.0 * (endTime - startTime) / CLOCKS_PER_SEC;
    cout << "共耗时：" << totalTime << "s" << endl;
    fclose(fp);
    FILE *fc = fopen("answer.txt", "r");
    check(fc, N);
    system("pause");
    return 0;
}
