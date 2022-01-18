#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <math.h>
#include <time.h>

using namespace std;

const int N = 110;
const double R = 6378.137e3;
const double PI = 3.14159;

//基站结构体
struct eNodeB
{
    int enodebid;               //基站id
    double longitude, latitude; //基站经纬度
    int id;                     //逆时针序号
};

int idx;           //基站数量
double t[N][N];    //最优三角剖分的目标值
int s[N][N];       //最优三角剖分的剖分点信息
eNodeB enodebs[N]; //基站数组

double w(int a, int b, int c);
double dist(int a, int b);
double radian(double a);

void min_weight_triangulation(int n, double t[][N], int s[][N]);
void find_breakpoints(int l, int r);

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

    srand(time(NULL));
    min_weight_triangulation(idx, t, s);

    printf("*********Optimal Triangulation*********\n");
    printf("The optimal target W is %lf.\n", t[1][idx]);
    find_breakpoints(1, idx);

    return 0;
}

void min_weight_triangulation(int n, double t[][N], int s[][N])
{
    for (int i = 1; i <= n; i++)
        t[i][i] = 0; //退化的多边形权值为0
    
    //控制子问题规模r=2,3,...n，自下而上，逐步求解
    for (int r = 2; r <= n; r++)
    {
        //子问题起点i循环, 规模为r的多个子问题t[i][j]求解
        for (int i = 1; i <= n - r + 1; i++)
        {
            int j = i + r - 1;
            //选取离起点最近的第一个断点i
            t[i][j] = t[i + 1][j] + w(i - 1, i, j);
            s[i][j] = i;
            double u;
            //如果待比较断点数目不超过5个，则直接循环求解
            if (r - 1 <= 5)
            {
                for (int k = i + 1; k < i + r - 1; k++)
                {
                    u = t[i][k] + t[k + 1][j] + w(i - 1, k, j);
                    if (u < t[i][j])
                    {
                        t[i][j] = u;
                        s[i][j] = k;
                    }
                }
            }
            //如果待比较断点数目超过5个，则利用随机数选取5个断点进行比较
            else
            {
                for (int k = 0; k < 15; k++)
                {
                    int rdm = i + rand() % (r - 1);
                    u = t[i][rdm] + t[rdm + 1][j] + w(i - 1, rdm, j);
                    if (u < t[i][j])
                    {
                        t[i][j] = u;
                        s[i][j] = rdm;
                    }
                }
            }
        }
    }
}

void find_breakpoints(int l, int r)
{
    if (l == r)
        return;

    int bp = s[l][r];
    printf("The breakpoint between V%d and V%d is V%d.\n", l - 1, r, bp);
    find_breakpoints(l, bp);
    find_breakpoints(bp + 1, r);
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