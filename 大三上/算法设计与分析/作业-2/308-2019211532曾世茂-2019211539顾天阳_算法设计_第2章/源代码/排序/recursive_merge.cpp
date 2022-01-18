#include <iostream>
#include <time.h>

using namespace std;

int depth = 0;

void merge_rec(int a[], int b[], int l, int m, int r)
{
    int i = l, j = m + 1, k = l;
    while ((i <= m) && (j <= r))
    {
        if (a[i] <= a[j])
            b[k++] = a[i++];
        else
            b[k++] = a[j++];
    }
    if (i > m)
    {
        for (int q = j; q <= r; q++)
            b[k++] = a[q];
    }
    else
    {
        for (int q = i; q <= m; q++)
            b[k++] = a[q];
    }
}

void copy(int a[], int b[], int l, int r)
{
    for (int i = l; i <= r; i++)
        a[i] = b[i];
}

void merge_sort_by_recursion(int a[], int b[], int l, int r, int recDepth)
{
    depth ++;
    if (l < r)
    {
        int i = (l + r) / 2;
        merge_sort_by_recursion(a, b, l, i, recDepth+1);
        merge_sort_by_recursion(a, b, i + 1, r, recDepth+1);
        merge_rec(a, b, l, i, r);
        copy(a, b, l, r);
    }
}

int mergeSortByRecursion(int *a, int length)
{
    depth = 0;
    int *b = new int[length];
    merge_sort_by_recursion(a, b, 0, length - 1, 1);
    free(b);
    return depth;
}
