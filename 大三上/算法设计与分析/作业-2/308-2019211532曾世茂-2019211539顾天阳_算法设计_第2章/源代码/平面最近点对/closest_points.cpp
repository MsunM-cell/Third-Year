#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <math.h>
#include <time.h>

using namespace std;

const int N = 2000;
const double R = 6378.137e3;
const double PI = 3.14159;

class PointX
{
public:
    void set_point(double m, double n, int i, int j)
    {
        x = m;
        y = n;
        ID = i;
        enodebid = j;
    }

    double get_x()
    {
        return x;
    }

    double get_y()
    {
        return y;
    }

    int get_ID()
    {
        return ID;
    }

    int get_enodebid()
    {
        return enodebid;
    }

    int operator<=(PointX a) const
    {
        return (x <= a.x);
    }

private:
    int ID, enodebid;
    double x, y;
};

class PointY
{
public:
    void set_point(double m, double n, int i, int j)
    {
        x = m;
        y = n;
        p = i;
        enodebid = j;
    }

    double get_x()
    {
        return x;
    }

    double get_y()
    {
        return y;
    }

    int get_p()
    {
        return p;
    }

    int get_enodebid()
    {
        return enodebid;
    }

    int operator<=(PointY a) const
    {
        return (y <= a.y);
    }

private:
    int p, enodebid;
    double x, y;
};

double d1;

template <class Type>
void merge(Type X[], Type tmp[], int l, int m, int r)
{
    int i = l, j = m + 1, k = l;
    while ((i <= m) && (j <= r))
    {
        if (X[i] <= X[j])
            tmp[k++] = X[i++];
        else
            tmp[k++] = X[j++];
    }

    if (i > m)
    {
        for (int q = j; q <= r; q++)
            tmp[k++] = X[q];
    }
    else
    {
        for (int q = i; q <= m; q++)
            tmp[k++] = X[q];
    }
}

template <class Type>
void copy(Type X[], Type tmp[], int l, int r)
{
    for (int i = l; i <= r; i++)
        X[i] = tmp[i];
}

template <class Type>
void merge_sort(Type X[], Type tmp[], int l, int r)
{
    if (l < r)
    {
        int i = (l + r) / 2;
        merge_sort(X, tmp, l, i);
        merge_sort(X, tmp, i + 1, r);
        merge(X, tmp, l, i, r);
        copy(X, tmp, l, r);
    }
}

double radian(double lon_or_lat)
{
    return lon_or_lat * PI / 180;
}

template <class Type>
double distance(Type u, Type v)
{
    double rad_lon1 = radian(u.get_x());
    double rad_lon2 = radian(v.get_x());
    double rad_lat1 = radian(u.get_y());
    double rad_lat2 = radian(v.get_y());
    double s = acos(cos(rad_lat1) * cos(rad_lat2) * cos (rad_lon1 - rad_lon2) + sin(rad_lat1) * sin(rad_lat2));
    s = s * R;
    if (fabs(s - 0) < 1e-6)
        return 0x3f3f3f3f;
    if (fabs(s - d1) < 1e-6)
        return 0x3f3f3f3f;
    return s;
}

void closest(PointX X[], PointY Y[], PointY Z[], int l, int r, PointX &a, PointX &b, double &d)
{
    if (r - l == 1)
    {
        a = X[l];
        b = X[r];
        d = distance(X[l], X[r]);
        return;
    }

    if (r - l == 2)
    {
        double d1 = distance(X[l], X[l + 1]);
        double d2 = distance(X[l + 1], X[r]);
        double d3 = distance(X[l], X[r]);
        if (d1 <= d2 && d1 <= d3)
        {
            a = X[l];
            b = X[l + 1];
            d = d1;
            return;
        }
        if (d2 <= d3)
        {
            a = X[l + 1];
            b = X[r];
            d = d2;
        }
        else
        {
            a = X[l];
            b = X[r];
            d = d3;
        }
        return;
    }

    // divide and rule
    int m = (l + r) / 2;
    int f = l, g = m + 1;
    for (int i = l; i <= r; i++)
    {
        if (Y[i].get_p() > m)
            Z[g++] = Y[i];
        else
            Z[f++] = Y[i];
    }

    closest(X, Z, Y, l, m, a, b, d);

    double dr;
    PointX ar, br;
    closest(X, Z, Y, m + 1, r, ar, br, dr);

    if (dr < d)
    {
        a = ar;
        b = br;
        d = dr;
    }

    // reconstruct Y
    merge(Z, Y, l, m, r);

    int k = l;
    for (int i = l; i <= r; i++)
    {
        if (fabs(X[m].get_x() - Y[i].get_x()) < d)
            Z[k++] = Y[i];
    }

    for (int i = l; i < k; i++)
    {
        for (int j = i + 1; j < k && Z[j].get_y() - Z[j].get_y() < d; j++)
        {
            double dp = distance(Z[i], Z[j]);
            if (dp < d)
            {
                d = dp;
                a = X[Z[i].get_p()];
                b = X[Z[j].get_p()];
            }
        }
    }
}

bool Cpair2(PointX X[], int n, PointX &a, PointX &b, double &d)
{
    if (n < 2)
        return false;

    PointX *tmp = new PointX[n];
    merge_sort(X, tmp, 0, n - 1);
    delete[] tmp;

    PointY *Y = new PointY[n];
    for (int i = 0; i < n; i++)
        Y[i].set_point(X[i].get_x(), X[i].get_y(), i, X[i].get_enodebid());

    PointY *tmp1 = new PointY[n];
    merge_sort(Y, tmp1, 0, n - 1);
    delete[] tmp1;

    PointY *Z = new PointY[n];
    closest(X, Y, Z, 0, n - 1, a, b, d);
    delete[] Y;
    delete[] Z;

    return true;
}

int main()
{
    PointX *X = new PointX[N];

    ifstream in;
    in.open("enodeb.txt", ios::in);
    string buffer;
    int idx, enodebid;
    double longitude, latitude;
    while (getline(in, buffer))
    {
        stringstream ss(buffer);
        ss >> idx >> enodebid >> longitude >> latitude;
        X[idx].set_point(longitude, latitude, idx, enodebid);
    }

    PointX a, b;
    double d;
    Cpair2(X, idx + 1, a, b, d);

    printf("The closest enodebs pair are eNodeB %d and eNodeB %d.\n", a.get_enodebid(), b.get_enodebid());
    printf("Their distance is %lf.\n\n", d);

    d1 = d;
    Cpair2(X, idx + 1, a, b, d);

    printf("The second closest enodebs pair are eNodeB %d and eNodeB %d.\n", a.get_enodebid(), b.get_enodebid());
    printf("Their distance is %lf.\n", d);

    system("pause");
    return 0;
}