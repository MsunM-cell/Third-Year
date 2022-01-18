#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

const int N = 2000;

struct eNodeB
{
    int enodebid;
    double longitude, latitude;
    double k_dist;
};

int idx, k, r_level;
eNodeB enodebs[N];

int partition(int l, int r)
{
    int i = l, j = r + 1;
    eNodeB x = enodebs[l];
    while (true)
    {
        while (enodebs[++i].k_dist < x.k_dist && i < r)
            ;
        while (enodebs[--j].k_dist > x.k_dist)
            ;
        if (i >= j)
            break;
        swap(enodebs[i], enodebs[j]);
    }
    enodebs[l] = enodebs[j];
    enodebs[j] = x;
    return j;
}

void quick_sort(int l, int r)
{
    if (l < r)
    {
        int q = partition(l, r);
        quick_sort(l, q - 1);
        quick_sort(q + 1, r);
    }
}

double select(int l, int r, int k)
{
    r_level++;
    if (r - l < 20)
    {
        quick_sort(l, r);
        idx = l + k - 1;
        return enodebs[l + k - 1].k_dist;
    }

    for (int i = 0; i <= (r - l - 4) / 5; i++)
    {
        int s = l + 5 * i;
        int t = s + 4;
        quick_sort(s, t);
        swap(enodebs[l + i], enodebs[s + 2]);
    }

    double x = select(l, l + (r - l - 4) / 5, (r - l + 6) / 10);
    swap(enodebs[l], enodebs[idx]);
    int i = partition(l, r);
    int j = i - l + 1;
    // if (k == j)
    // {
    //     idx = i;
    //     return enodebs[i].k_dist;
    // }
    if (k <= j)
        return select(l, i, k);
    else
        return select(i + 1, r, k - j);
}

int main()
{
    ifstream in;
    in.open("enodeb.txt", ios::in);
    string buffer;
    while (getline(in, buffer))
    {
        stringstream ss(buffer);
        ss >> idx >> enodebs[idx].enodebid;
        ss >> enodebs[idx].longitude >> enodebs[idx].latitude >> enodebs[idx].k_dist;
    }

    printf("Please input the k: ");
    scanf("%d", &k);
    double ans = select(0, idx, k);
    printf("The recursion level is %d.\n", r_level);
    printf("The target eNodeB is (enodebid %d, longitude %lf, latitude %lf, k_dist %lf).\n", enodebs[idx].enodebid, enodebs[idx].longitude, enodebs[idx].latitude, enodebs[idx].k_dist);

    system("pause");
    return 0;
}
