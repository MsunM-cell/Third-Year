#include <iostream>
#include <time.h>

using namespace std;

int swapTime = 0;
void merge_non(int x[], int y[], int l, int m, int r)
{
    int i = l, j = m + 1, k = l;
    while ((i <= m) && (j <= r))
    {
        swapTime++;

        if (x[i] <= x[j])
            y[k++] = x[i++];
        else
            y[k++] = x[j++];
    }

    while (i <= m)
    {
        y[k++] = x[i++];
        swapTime++;
    }

    while (j <= r)
    {
        y[k++] = x[j++];
        swapTime++;
    }
}

void merge_pass(int x[], int y[], int s, int n)
{
    int i = 0;
    while (i <= n - 2 * s)
    {
        merge_non(x, y, i, i + s - 1, i + 2 * s - 1);
        i = i + 2 * s;
    }
    if (i + s < n)
        merge_non(x, y, i, i + s - 1, n - 1);
    else
        for (int j = i; j <= n - 1; j++){
             y[j] = x[j];
             swapTime++;
        }  
}

int nonReCusiveMerge(int *a, int N, int M)
{
    swapTime = 0;
    int *b = new int[N];
    int s = 1;
    while (s < N)
    {
        merge_pass(a, b, s, N);
        s += s;
        merge_pass(b, a, s, N);
        s += s;
    }
    return swapTime;
}
