#include <iostream>
#include <sstream>
#include <fstream>
#include <time.h>

using namespace std;

int sum_nodes = 0;
clock_t start_time = clock();

class Color
{
    friend int mColoring(int, int, int **);

private:
    bool Ok(int k);
    void Backtrack(int t);

    int n,    //图的顶点数
        m,    //可用颜色数
        **a,  //图的邻接矩阵
        *x;   //当前已找到的可m着色方案
    long sum; //当前已找到的可m着色方案数
};

bool Color::Ok(int k) //检查颜色可用性
{
    for (int j = 1; j <= n; j++)
        if ((a[k][j] == 1) && (x[j] == x[k]))
            return false;
    return true;
}

void Color::Backtrack(int t)
{
    sum_nodes++;
    if (t > n)
    {
        sum++;
        for (int i = 1; i <= n; i++)
            printf("%d ", x[i]);
        printf("\n");
        clock_t end_time = clock();
        printf("The run time is %lfs.\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
        printf("Total number of nodes searched is %d.\n", sum_nodes);
        exit(-1);
    }
    else
    {
        for (int i = 1; i <= m; i++)
        {
            x[t] = i;
            if (Ok(t))
                Backtrack(t + 1);
            x[t] = 0;
        }
    }
}

int mColoring(int n, int m, int **a)
{
    // clock_t start_time = clock();
    Color X;
    //初始化X
    X.n = n;
    X.m = m;
    X.a = a;
    X.sum = 0;
    int *p = new int[n + 1];
    for (int i = 0; i <= n; i++)
        p[i] = 0;
    X.x = p;
    X.Backtrack(1);
    delete[] p;
    // clock_t end_time = clock();
    // printf("The run time is %lfs.\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);
    // printf("Total number of nodes searched is %d.\n", sum_nodes);
    return X.sum;
}

void initMatrix(int n, int **&a)
{
    a = new int *[n + 1];
    for (int i = 1; i <= n; i++)
        a[i] = new int[n + 1];

    //读取基站信息
    ifstream in;
    in.open("42.txt", ios::in);
    string buffer;

    int i = 1;
    double dist;
    while (getline(in, buffer))
    {
        stringstream ss(buffer);
        for (int j = 1; j <= n; j++)
        {
            ss >> dist;
            if (dist == 99999)
                a[i][j] = 0;
            else
                a[i][j] = 1;
        }
        i++;
    }
}

int main()
{
    int n = 42, m = 20;
    int **a = NULL;
    initMatrix(n, a);

    int res = mColoring(n, m, a);
    printf("Total number of coloring schemes is %d.\n", res);

    return 0;
}