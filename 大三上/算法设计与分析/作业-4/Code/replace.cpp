#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR 0
#define INF 0x3f3f3f3f
#define N 256
#define M 1024 * 100

typedef char **HuffmanCode; //哈夫曼编码类型
typedef struct
{
    char data;                  //编码树结点的字符
    int weight;                 //当前字符的出现频率
    int parent, lchild, rchild; //当前结点的父亲、左儿子和右儿子
} HuffmanNode;                  //哈夫曼树结点类型

typedef struct
{
    HuffmanNode arr[N]; //编码树结点数组
    int n, count;       // n表示字符的种类数，count表示字符的总数目
} HuffmanTree;          //哈夫曼树类型

int root, match[N]; // root表示根结点，match用来存储字符-下标映射关系
bool flag[N];       //标记当前字符是否为第一次出现

void initHuffmanTree(HuffmanTree &T);                   //初始化哈夫曼树
void createHuffmanTree(HuffmanTree &T);                 //构造哈夫曼树
void createHuffmanCode(HuffmanTree T, HuffmanCode &HC); //构造哈夫曼编码
void printHuffmanInfo(HuffmanTree T, HuffmanCode HC);   //打印相关信息

bool findTwoMin(HuffmanTree T, int &m1, int &m2, int num); //寻找当前最小频率的两个字符结点

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
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')))
            c = '#';

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

//构造哈夫曼树
void createHuffmanTree(HuffmanTree &T)
{
    int m1 = 0, m2 = 0; //最小频率的两个结点
    int i = T.n;

    //循环寻找最小频率的两个结点
    while (findTwoMin(T, m1, m2, i))
    {
        //合并两个结点
        T.arr[m1].parent = T.arr[m2].parent = i;
        T.arr[i].lchild = m1, T.arr[i].rchild = m2;
        T.arr[i].parent = -1;
        T.arr[i].weight = T.arr[m1].weight + T.arr[m2].weight;
        i++;
    }

    root = m1; //哈夫曼树的根结点
}

//构造哈夫曼编码
void createHuffmanCode(HuffmanTree T, HuffmanCode &HC)
{
    HC = (char **)malloc(sizeof(char *) * (T.n));

    char tmp[N];
    for (int i = 0; i < T.n; i++)
    {
        //自底向上构建字符的逆向哈夫曼编码
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
        //倒置处理，获取正向的哈夫曼编码
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