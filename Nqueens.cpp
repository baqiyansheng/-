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
vector<int> queen;                        //��¼��i�лʺ����ڵ���
void initQueen(int N, vector<int> &queen) // ��ʼ��queen
{
    vector<int> array(N);
    for (int i = 0; i < N; i++) //�Ȱѵ�i�еĻʺ���ڵ�i�У���֤�����ϵĻʺ󲻳�ͻ
        array[i] = i;
    queen.clear(); //����
    while (N)      //�������
    {
        double t = rand() % N;
        queen.push_back(array[t]);
        array[t] = array[--N];
    }
}
long long compute_collisions(const vector<int> &queen, vector<int> &dn, vector<int> &dp) //�����ܳ�ͻ����
{
    for (int t = 0; t < 2 * queen.size(); t++)
        dn[t] = dp[t] = 0; //��ʼ��
    for (int i = 0; i < (int)queen.size(); i++)
    {
        int j = queen.at(i);
        dn[i + j]++;
        dp[queen.size() - 1 + i - j]++;
    }
    long long collisions = 0;
    for (int k = 0; k < 2 * queen.size() - 1; k++)
    {
        collisions += dn[k] * (dn[k] - 1) / 2; //ͬһ��б���ϵĳ�ͻ�ʺ�����
        collisions += dp[k] * (dp[k] - 1) / 2; //ͬһ��б���ϵĳ�ͻ�ʺ�����
    }
    return collisions; //�����ܳ�ͻ��
}
int compute_attacks(const vector<int> &queen, vector<int> &dn, vector<int> &dp, vector<int> &attack)
{
    attack.clear(); //��ʼ��
    int attackedLines = 0;
    for (int i = 0; i < (int)queen.size(); i++)
    {
        int j = queen.at(i);
        if (dn.at(i + j) > 1 || dp.at(queen.size() - 1 + i - j) > 1) //�ûʺ����ڵ���б�߻�����б���ϴ��������ʺ�
        {
            attack.push_back(i); //��¼����ʺ�
            attackedLines++;     //����
        }
    }
    return attackedLines;
}
int randInt(int k, int size) //��0-size-1֮�������һ��������k����
{
    int val = rand() % size;
    while (val == k) //�������k���ٴ����
        val = rand() % size;
    return val;
}
long long cacl_collisions(const vector<int> &queen, vector<int> &dn, vector<int> &dp, int a, int b) //���㽻��������ڽ���ǰ�ĳ�ͻ���仯
{
    int i, j, counts = 0;
    i = queen[a];
    j = queen[b];
    counts -= dn[a + i] - 1;
    counts -= dn[b + j] - 1;
    if (a + i == b + j) //�����ʺ���ͬһ��б����
        counts++;
    counts -= dp[queen.size() - 1 + a - i] - 1;
    counts -= dp[queen.size() - 1 + b - j] - 1;
    if (a - i == b - j) //�����ʺ���ͬһ��б����
        counts++;
    counts += dn[a + j];
    counts += dn[b + i];
    if (a + j == b + i) //�����ʺ���ͬһ��б����
        counts++;
    counts += dp[queen.size() - 1 + a - j];
    counts += dp[queen.size() - 1 + b - i];
    if (a - j == b - i) //�����ʺ���ͬһ��б����
        counts++;
    return counts;
}
void updateQueen(vector<int> &queen, vector<int> &dn, vector<int> &dp, int a, int b) //ˢ��λ�ã�ÿ��б���ϵĻʺ���
{
    int i = queen.at(a);
    int j = queen.at(b);
    dn[a + i]--;
    dn[b + j]--;
    dp[queen.size() - 1 + a - i]--;
    dp[queen.size() - 1 + b - j]--;
    int t = queen[a]; //�����ʺ�λ��
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
    long long collisions = N; //�ܳ�ͻ�ʺ���
    int num_of_attack;
    int swapCount = 0;
    vector<int> attack;    //��¼������ͻ�Ļʺ�
    vector<int> dn(2 * N); //ÿ����б���ϵĻʺ����������Ͻǵ����½�
    vector<int> dp(2 * N); //ÿ����б���ϵĻʺ����������Ͻǵ����½�
    if (N == 2 || N == 3)
    {
        cout << "�޽�" << endl;
        return;
    }
    while (collisions) //�ڳ�ͻ����Ϊ0��ǰһֱִ��
    {
        initQueen(N, queen);                                    //��ʼ���ʺ�����
        collisions = compute_collisions(queen, dn, dp);         //�����ͻ��
        num_of_attack = compute_attacks(queen, dn, dp, attack); //��¼��ͻ�Ļʺ�
        int limit = C1 * collisions;                            //�趨��ͻ����
        int loopSteps = 0;
        while (loopSteps < C2 * N) //�趨��������
        {
            for (int k = 0; k < num_of_attack; k++)
            {
                int i = attack.at(k);
                int j = randInt(i, N);
                long long c;
                if ((c = cacl_collisions(queen, dn, dp, i, j)) < 0) //�������ͻ������
                {
                    collisions += c;
                    updateQueen(queen, dn, dp, i, j);
                    swapCount++;
                    if (collisions == 0) //�������
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
    cout << "��������: " << swapCount << endl;
}
void check(FILE *fc, int N)
{
    int i;
    vector<int> examine_1(N, 0);     //��Ǹ����Ƿ���ڻʺ�
    vector<int> examine_2(2 * N, 0); //��Ǹ���б���Ƿ���ڻʺ�
    vector<int> examine_3(2 * N, 0); //��Ǹ���б���Ƿ���ڻʺ�
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
            cout << "�˽����" << endl;
            fclose(fc);
            return;
        }
    }
    cout << "�˽���ȷ" << endl;
    fclose(fc);
}
int main()
{
    long long N = 10000000;
    clock_t startTime, endTime;
    cout << "������ʺ�ĸ�����";
    cin >> N;
    srand((unsigned)time(NULL));
    startTime = clock();
    Nqueens(N);
    endTime = clock();
    printQueen(queen);
    double totalTime = 1.0 * (endTime - startTime) / CLOCKS_PER_SEC;
    cout << "����ʱ��" << totalTime << "s" << endl;
    fclose(fp);
    FILE *fc = fopen("answer.txt", "r");
    check(fc, N);
    system("pause");
    return 0;
}
