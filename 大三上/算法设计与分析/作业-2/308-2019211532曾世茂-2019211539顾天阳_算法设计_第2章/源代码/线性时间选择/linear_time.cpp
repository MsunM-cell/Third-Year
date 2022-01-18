#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <time.h>

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

int randomized_partition(int l, int r)
{
    int i = l + rand() % (r - l + 1);
    swap(enodebs[i], enodebs[l]);
    return partition(l, r);
}

double randomized_select(int l, int r, int k)
{
    r_level++;
    if (l == r)
    {
        idx = l;
        return enodebs[l].k_dist;
    }

    int i = randomized_partition(l, r);
    int j = i - l + 1;
    // if (k == j)
    // {
    //     idx = i;
    //     return enodebs[i].k_dist;
    // }
    if (k <= j)
        return randomized_select(l, i, k);
    else
        return randomized_select(i + 1, r, k - j);
}

void randomized_quick_sort(int l, int r)
{
    if (l < r)
    {
        int m = randomized_partition(l, r);
        randomized_quick_sort(l, m - 1);
        randomized_quick_sort(m + 1, r);
    }
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

    srand(time(NULL));

    printf("Please input the k: ");
    scanf("%d", &k);
    double ans = randomized_select(0, idx, k);
    printf("The recursion level is %d.\n", r_level);
    printf("The target eNodeB is (enodebid %d, longitude %lf, latitude %lf, k_dist %lf).\n", enodebs[idx].enodebid, enodebs[idx].longitude, enodebs[idx].latitude, enodebs[idx].k_dist);

    system("pause");
    return 0;
}