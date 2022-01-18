#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cstring>
#include <Windows.h>
#include <map>
#include <fstream>
#include "non_recursive_merge.cpp"
#include "recursive_merge.cpp"
#include "quickSort.cpp"
int BitGetSum(int *bit, int i, int maxLength);
void BitAdd(int *bit, int i, int maxLenght);
long getDD(int *arr, int length, int maxNum);
void printArray(int *a, int length);
void randomArray(int *arr, int length);
using namespace std;

int debug = 0;
LARGE_INTEGER BegainTime;
LARGE_INTEGER EndTime;
LARGE_INTEGER Frequency;
int N, M;
long DD;
double ADD, duration;
double maxADD, minADD;

int *o;
int *a;
int *b;
int *c;
int *d;

int main()
{
    QueryPerformanceFrequency(&Frequency);
    cout << "请输入数组长度N , 数组最大数值M" << endl;
    scanf("%d%d", &N, &M);
    a = new int[N];
    b = new int[N];
    c = new int[N];
    d = new int[N];

    srand(time(NULL));
    for (int i = 0; i < N; i++)
    {
        a[i] = rand() % (M + 1);
    }


    memcpy(b, a, N * sizeof(int));
    memcpy(c, a, N * sizeof(int));
    memcpy(d, a, N * sizeof(int));

    printArray(a, N);

    DD = getDD(a, N, M);
    printf("DD: %ld\n", DD);
    ADD = (double)DD / N;
    printf("ADD: %lf\n", ADD);

    QueryPerformanceCounter(&BegainTime);
    int rec_depth = mergeSortByRecursion(a, N);
    QueryPerformanceCounter(&EndTime);
    duration = (double)(EndTime.QuadPart - BegainTime.QuadPart) / Frequency.QuadPart * 1000;
    printf("\n\n递归合并排序：\n递归次数: %d\ntime cost: %.4f ms\n", rec_depth, duration);
    printArray(a, N);

    QueryPerformanceCounter(&BegainTime);
    nonReCusiveMerge(b, N, M);
    QueryPerformanceCounter(&EndTime);
    duration = (double)(EndTime.QuadPart - BegainTime.QuadPart) / Frequency.QuadPart * 1000;
    printf("\n\n非递归合并排序:\ntime cost: %.4f ms\n", duration);
    printArray(b, N);

    QueryPerformanceCounter(&BegainTime);
    rec_depth = quickSort(c, N);
    QueryPerformanceCounter(&EndTime);
    duration = (double)(EndTime.QuadPart - BegainTime.QuadPart) / Frequency.QuadPart * 1000;
    printf("\n\n快速排序1 ：\n递归次数：%d\ntime cost: %.4f ms\n", rec_depth, duration);
    printArray(c, N);

    QueryPerformanceCounter(&BegainTime);
    rec_depth = quickSort2(d, N);
    QueryPerformanceCounter(&EndTime);
    duration = (double)(EndTime.QuadPart - BegainTime.QuadPart) / Frequency.QuadPart * 1000;
    printf("\n\n快速排序2 ：\n递归次数： %d\ntime cost: %.4f ms\n", rec_depth, duration);
    printArray(d, N);

    free(a);
    free(b);
    free(c);
    free(d);
    return 0;
}


long getDD(int *arr, int length, int maxNum)
{
    int bitLength = maxNum + 2;
    int *bit = new int[bitLength]; //0号位置不用，方便读写

    memset(bit, 0, bitLength * sizeof(int));
    long DD = 0;
    for (int i = length - 1; i >= 0; i--)
    {
        int j = arr[i] + 1; //0放到1的位置，
        BitAdd(bit, j, bitLength);
        DD += BitGetSum(bit, arr[i], bitLength);
    }
    free(bit);
    return DD;
}

void BitAdd(int *bit, int i, int maxLenght)
{
    while (i < maxLenght)
    {
        bit[i]++;
        i += i & -i;
    }
}

int BitGetSum(int *bit, int i, int maxLength)
{
    int sum = 0;
    while (i > 0)
    {
        sum += bit[i];
        i -= i & -i;
    }
    return sum;
}

void printArray(int *a, int length)
{
    if (debug == 1)
    {
        printf("数列为:\n");
        for (int i = 0; i < length; i++)
        {
            printf("%d ", a[i]);
        }
        printf("\n");
    }
}

void randomArray(int *arr, int length)
{
    map<int, int> hashmap;
    for (int i = 0; i < length;)
    {
        unsigned int u = rand();
        if (hashmap.find(u) == hashmap.end())
        {
            hashmap.insert(pair<int, int>(u, arr[i++]));
        }
    }
    int i = 0;

    for (auto it = hashmap.begin(); it != hashmap.end(); it++)
    {
        arr[i++] = it->second;
    }
}
