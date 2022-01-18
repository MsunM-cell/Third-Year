#include <algorithm>

using namespace std;

int recursiveTimes = 0;
int partition(int *q, int l, int r)
{
    int i = l, j = r + 1;
    int x = q[l];
    while (true)
    {
        while (q[++i] < x && i < r)
            ;
        while (q[--j] > x)
            ;
        if (i >= j)
            break;
        swap(q[i], q[j]);
    }
    q[l] = q[j];
    q[j] = x;
    return j;
}

void quickSort(int *a, int l, int r, int recDepth)
{
    recursiveTimes++;
    int notDesc = 1;
    for (int i = l; i < r; i++)
    {
        if (a[i + 1] < a[i])
        {
            notDesc = 0;
            break;
        }
    }
    if (notDesc == 1)
    {
        return;
    }

    int notInc = 1;
    for (int i = l; i < r; i++)
    {
        if (a[i + 1] > a[i])
        {
            notInc = 0;
            break;
        }
    }
    if (notInc == 1)
    {
        int N = (r - l + 1);
        for (int i = 0; i < N / 2; i++)
        {
            swap(a[i + l], a[N - i - 1 + l]);
        }
        return ;
    }

    if (l < r)
    {
        int m = partition(a, l, r);
        quickSort(a, l, m - 1, recDepth + 1);
        quickSort(a, m + 1, r, recDepth + 1);
    }
}

int quickSort(int *a, int length)
{
    recursiveTimes = 0;
    quickSort(a, 0, length - 1, 1);
    return recursiveTimes;
}

//快速排序2

int randomized_partition(int *q, int l, int r)
{
   int i = l + rand() % (r - l + 1);
    swap(q[i], q[l]);
    return partition(q, l, r);
}

void randomizedQuickSort(int *a, int l, int r, int recDepth)
{

    recursiveTimes++;
    int notDesc = 1;
    for (int i = l; i < r; i++)
    {
        if (a[i + 1] < a[i])
        {
            notDesc = 0;
            break;
        }
    }
    if (notDesc == 1)
    {
        return;
    }

    int notInc = 1;
    for (int i = l; i < r; i++)
    {
        if (a[i + 1] > a[i])
        {
            notInc = 0;
            break;
        }
    }
    if (notInc == 1)
    {
        int N = (r - l + 1);
        for (int i = 0; i < N / 2; i++)
        {
            swap(a[i + l], a[N - i - 1 + l]);
        }
        return;
    }

    if (l < r)
    {
         int m = randomized_partition(a, l, r);
        randomizedQuickSort(a, l, m - 1, recDepth + 1);
        randomizedQuickSort(a, m + 1, r, recDepth + 1);
    }
}

int quickSort2(int *a, int lenght)
{
    recursiveTimes = 0;
    randomizedQuickSort(a, 0, lenght - 1, 1);
    return recursiveTimes;
}
