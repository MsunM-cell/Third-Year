#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

const int N = 1010;

int n = 100, m = 650; // n表示物品个数，m表示背包重量
int v[N], w[N];       // v数组保存物品的重量，w数组保存物品的价值
int f[N];             // f[j]表示总重量不超过j的所有选法中的最大价值
int weight;           //最优选取方案的物品总重量

int main()
{
    ifstream in;
    in.open("knapsack2.txt", ios::in);
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
        //这里j需要从背包容量开始遍历，防止第i-1层的f被第i层的f所覆盖
        for (int j = m; j >= v[i]; j--)
            f[j] = max(f[j], f[j - v[i]] + w[i]);

    printf("The total value is %d.\n", f[m]);

    return 0;
}