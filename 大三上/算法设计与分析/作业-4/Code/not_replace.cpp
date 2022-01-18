#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR 0
#define INF 0x3f3f3f3f
#define N 256
#define M 1024 * 100

typedef char **HuffmanCode;
typedef struct
{
    char data;
    int weight;
    int parent, lchild, rchild;
} HuffmanNode;

typedef struct
{
    HuffmanNode arr[N];
    int n, count;
} HuffmanTree;

int root, match[N];
bool flag[N];

void initHuffmanTree(HuffmanTree &T);
void createHuffmanTree(HuffmanTree &T);
void createHuffmanCode(HuffmanTree T, HuffmanCode &HC);
void printHuffmanInfo(HuffmanTree T, HuffmanCode HC);

bool findTwoMin(HuffmanTree T, int &m1, int &m2, int num);

int main()
{
    HuffmanTree T;
    HuffmanCode HC;

    initHuffmanTree(T);
    createHuffmanTree(T);
    createHuffmanCode(T, HC);

    printHuffmanInfo(T, HC);

    return 0;
}

void initHuffmanTree(HuffmanTree &T)
{
    T.n = T.count = 0;
    for (int i = 0; i < N; i++)
    {
        T.arr[i].weight = 0;
        T.arr[i].parent = T.arr[i].lchild = T.arr[i].rchild = -1;
    }

    FILE *fp = fopen("text.txt", "r");
    if (!fp)
        exit(ERROR);

    char c = fgetc(fp);
    while (c != EOF)
    {
        if (!flag[c])
        {
            match[c] = T.n++;
            flag[c] = true;
        }

        T.arr[match[c]].data = c;
        T.arr[match[c]].weight++;

        T.count++;
        c = fgetc(fp);
    };

    fclose(fp);
}

void createHuffmanTree(HuffmanTree &T)
{
    int m1 = 0, m2 = 0;
    int i = T.n;

    while (findTwoMin(T, m1, m2, i))
    {
        T.arr[m1].parent = T.arr[m2].parent = i;
        T.arr[i].lchild = m1, T.arr[i].rchild = m2;
        T.arr[i].parent = -1;
        T.arr[i].weight = T.arr[m1].weight + T.arr[m2].weight;
        i++;
    }

    root = m1;
}

void createHuffmanCode(HuffmanTree T, HuffmanCode &HC)
{
    HC = (char **)malloc(sizeof(char *) * (T.n));

    char tmp[N];
    for (int i = 0; i < T.n; i++)
    {
        int p = T.arr[i].parent;
        int c = i, top = 0;

        while (p != -1)
        {
            if (T.arr[p].lchild == c)
                tmp[top++] = '0';
            else
                tmp[top++] = '1';
            c = p, p = T.arr[p].parent;
        }

        HC[i] = (char *)malloc(sizeof(char) * N);

        int j = 0;
        while (top != 0)
            HC[i][j++] = tmp[--top];
        HC[i][j] = '\0';
    }
}

void printHuffmanInfo(HuffmanTree T, HuffmanCode HC)
{
    printf("%15s%15s%15s\n", "character", "frequency", "huffman-code");
    for (int i = 0; i < T.n; i++)
    {
        char c = T.arr[i].data;
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
            printf("%15c%15d%15s\n", c, T.arr[i].weight, HC[i]);
    }
    for (int i = 0; i < T.n; i++)
    {
        char c = T.arr[i].data;
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')))
        {
            if (c == '\n')
                printf("%15s%15d%15s\n", "\\n", T.arr[i].weight, HC[i]);
            else    
                printf("%15c%15d%15s\n", c, T.arr[i].weight, HC[i]);
        }
    }

    printf("\nFor fixed-length code, the file needs %d bytes to save the text.\n", T.count);
    int bitsNum = 0;
    for (int i = 0; i < T.n; i++)
        bitsNum += T.arr[i].weight * strlen(HC[i]);
    printf("But for huffman-code, the file only needs %d bytes to save the text.\n", (bitsNum / 8) + 1);
}

bool findTwoMin(HuffmanTree T, int &m1, int &m2, int num)
{
    int min1 = INF, min2 = INF;
    for (int i = 0; i < num; i++)
    {
        if (T.arr[i].parent == -1)
        {
            if (T.arr[i].weight < min1)
            {
                min2 = min1;
                m2 = m1;
                min1 = T.arr[i].weight;
                m1 = i;
            }
            else if (T.arr[i].weight < min2)
            {
                min2 = T.arr[i].weight;
                m2 = i;
            }
        }
    }

    if (min1 == INF || min2 == INF)
        return false;

    return true;
}