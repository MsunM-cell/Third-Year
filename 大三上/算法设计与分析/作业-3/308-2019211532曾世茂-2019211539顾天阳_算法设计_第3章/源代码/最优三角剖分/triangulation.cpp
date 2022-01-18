#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <math.h>

using namespace std;

const int N = 110;
const double R = 6378.137e3;
const double PI = 3.14159;

//基站结构体
struct eNodeB
{
    int enodebid;
    double longitude, latitude;
    int id;
};

int idx;           //基站个数
double t[N][N][2]; //权函数，后面2分别对应最优值和次优值
int s[N][N][2];    //三角剖分中的断点信息，后面2分别对应最优值和次优值
eNodeB enodebs[N]; //基站数组

double w(int a, int b, int c);//求三角形的权值和
double dist(int a, int b);//求两个基站间的距离
double radian(double a);//将经纬度转化成弧度

void min_weight_triangulation(int n, double t[][N][2], int s[][N][2]);//最优三角剖分dp函数
void find_breakpoints(int l, int r, int u);//递归寻找断点

int main()
{
    ifstream in;
    in.open("enodebs1.txt", ios::in);
    string buffer;
    int a, b;
    double c, d;
    while (getline(in, buffer))
    {
        stringstream ss(buffer);
        ss >> a >> c >> d >> b;
        enodebs[b].enodebid = a;
        enodebs[b].longitude = c;
        enodebs[b].latitude = d;
        enodebs[b].id = b;
        idx = b;
    }

    min_weight_triangulation(idx, t, s);

    // optimal triangulation
    printf("*********Optimal Triangulation*********\n");
    printf("The optimal target W is %lf.\n", t[1][idx][0]);
    find_breakpoints(1, idx, 0);

    // sub-optimal triangulation
    printf("\n\n*********Sub-optimal Triangulation*********\n");
    printf("The sub-optimal target W is %lf.\n", t[1][idx][1]);
    find_breakpoints(1, idx, 1);

    return 0;
}

void min_weight_triangulation(int n, double t[][N][2], int s[][N][2])
{
    for (int i = 1; i <= n; i++)
        t[i][i][0] = t[i][i][1] = 0;
    for (int r = 2; r <= n; r++)
    {
        for (int i = 1; i <= n - r + 1; i++)
        {
            int j = i + r - 1;
            t[i][j][0] = t[i][j][1] = t[i + 1][j][0] + w(i - 1, i, j);
            s[i][j][0] = s[i][j][1] = i;
            for (int k = i + 1; k < i + r - 1; k++)
            {
                double u = t[i][k][0] + t[k + 1][j][0] + w(i - 1, k, j);
                if (u < t[i][j][0])
                {
                    t[i][j][1] = t[i][j][0];
                    s[i][j][1] = s[i][j][0];
                    t[i][j][0] = u;
                    s[i][j][0] = k;
                }
            }
        }
    }
}

void find_breakpoints(int l, int r, int u)
{
    if (l == r)
        return;

    int bp = s[l][r][u];
    printf("The breakpoint between V%d and V%d is V%d.\n", l - 1, r, bp);
    find_breakpoints(l, bp, u);
    find_breakpoints(bp + 1, r, u);
}

double w(int a, int b, int c)
{
    return dist(a, b) + dist(a, c) + dist(b, c);
}

double dist(int a, int b)
{
    double rad_lon1 = radian(enodebs[a].longitude);
    double rad_lon2 = radian(enodebs[b].longitude);
    double rad_lat1 = radian(enodebs[a].latitude);
    double rad_lat2 = radian(enodebs[b].latitude);
    double s = acos(cos(rad_lat1) * cos(rad_lat2) * cos(rad_lon1 - rad_lon2) + sin(rad_lat1) * sin(rad_lat2));
    s = s * R;
    return s;
}

double radian(double a)
{
    return a * PI / 180;
}