#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

const int N = 1010;

int n = 50, m = 340;       // n表示物品个数，m表示背包重量
int v[N], w[N]; // v数组保存物品的重量，w数组保存物品的价值
int f[N][N];    // f[i][j]表示从前i个物品中选取总重量不超过j的所有选法中的最大价值
int weight;     //最优选取方案的物品总重量
bool flag[N];   //存放最优选取方案中每个物品是否被放入

int main()
{
    ifstream in;
    in.open("knapsack1.txt", ios::in);
    string buffer;
    int index = 0;
    while (getline(in, buffer))
    {
        stringstream ss(buffer);
        if (index == 0)
            for (int i = 1; i <= n; i++)
                ss >> v[i];
        else if (index == 1)
            for (int i = 1; i <= n; i++)
                ss >> w[i];
        else
            break;
        index++;
    }

    //从第i个物品，总重量不超过j开始遍历
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
        {
            //首先假设不选取第i个物品
            f[i][j] = f[i - 1][j];
            //若第i个物品的重量不超过当前剩余重量，则考虑选取第i个物品的情况（取较大者）
            if (j >= v[i])
                f[i][j] = max(f[i][j], f[i - 1][j - v[i]] + w[i]);
        }

    int c = m;
    //判断最优选取方案中各个物品是否被选取
    for (int i = n; i > 0; i--)
    {
        //若选取第i个物品的最大价值等于不选取第i个物品的最大价值，则表示第i个物品未选取
        if (f[i][c] == f[i - 1][c])
            flag[i] = false;
        //否则选取第i个物品，标志为true
        else
        {
            flag[i] = true;
            c -= v[i];
            weight += v[i];
        }
    }

    for (int i = 1; i <= n; i++)
        if (flag[i])
            printf("Goods %d is put.\n", i);
        else
            printf("Goods %d is not put.\n", i);

    printf("\nThe total weight is %d.\n", weight);
    printf("The total value is %d.\n", f[n][m]);

    return 0;
}