/*
 * @Descripttion:
 * @version:
 * @Author: shimmerZ
 * @Date: 2022-01-05 16:27:35
 * @LastEditors: shimmerZ
 * @LastEditTime: 2022-01-05 21:21:09
 */
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include "MinHeapNode.cpp"
#include <windows.h>
using namespace std;

void read(string fileName);
void split(const string &s, vector<int> &result, const string &delimiters);
struct Operater
{
    bool operator()(MinHeapNode *a, MinHeapNode *b)
    {
        return a->lowerstCost > b->lowerstCost;
    }
};

priority_queue<MinHeapNode *, vector<MinHeapNode *>, Operater> H;
vector<int> *nodeMap;  //将题目中的节点编号映射成从0开始
double **dis;          //两点间的距离
double *minOut;        //每个节点的最小出度
int nodeNum;           //节点数量
double bestLength;     //当前最优路径长度
vector<int> bestRoad;  //最优路径
int start;             //起始点
long visitNodeNum = 0; //已访问的节点数量
int main()
{
    LARGE_INTEGER timeEnd, timeBegin;
    QueryPerformanceFrequency(&timeEnd);
    double secondsPerTick = 1.0 / timeEnd.QuadPart;
    read("22.txt");
    for (int i = 0; i < nodeMap->size(); i++)
    {
        if (nodeMap->at(i) == 20)
        {
            start = i;
            break;
        }
    }
    //首先先把根节点放进H里面
    MinHeapNode *root = new MinHeapNode(nodeNum);
    for (int i = 0; i < nodeNum; i++)
    {
        root->visist[i] = 0;
    }
    root->visist[start] = 1;
    root->currentRoad.push_back(start);
    root->currentCost = 0;
    double restCost = 0;
    for (int i = 0; i < nodeNum; i++)
    {
        restCost += minOut[i];
    }
    root->restCost = restCost;
    root->lowerstCost = restCost;
    H.push(root);
    QueryPerformanceCounter(&timeBegin);
    while (H.size() > 0)
    {
        visitNodeNum++;
        MinHeapNode *currentNode = H.top();
        H.pop();
        if (currentNode->currentRoad.size() == nodeNum - 1)
        {//还剩余一个节点以及回去的道路，故路径已经确定
            int unvisit = 0;
            for (; unvisit < nodeNum; unvisit++)
            {
                if (currentNode->visist[unvisit] == 0)
                {
                    break;
                }
            } //找到尚未访问的点
            if (dis[currentNode->getLastVisit()][unvisit] != -1 &&
                dis[unvisit][start] != -1 &&
                currentNode->currentCost + dis[currentNode->getLastVisit()][unvisit] + dis[unvisit][start] < bestLength)
            {
                bestLength = currentNode->currentCost + dis[currentNode->getLastVisit()][unvisit] + dis[unvisit][start];
                currentNode->currentCost = bestLength;
                currentNode->lowerstCost = bestLength;
                currentNode->visist[unvisit] = 1;
                currentNode->currentRoad.push_back(unvisit);
                H.push(currentNode);
            }
            else//该节点无法产生最优路径
            {
                delete currentNode;
            }
        }
        else if (currentNode->currentRoad.size() == nodeNum)
        { //已经找到最优路径
            bestRoad = currentNode->currentRoad;
            delete currentNode;
            break;
        }
        else
        {
            for (int i = 0; i < nodeNum; i++)
            {
                if (currentNode->visist[i] == 1 || dis[currentNode->getLastVisit()][i] == -1)
                {//已访问过或到该节点无可行路径
                    continue;
                }
                else
                {   //得到新的节点
                    MinHeapNode *nextNode = currentNode->getNext(i);
                    if (nextNode->lowerstCost < bestLength)//有望望产生最优路径
                    {
                        H.push(nextNode);
                    }
                    else
                    {
                        delete nextNode;
                    }
                }
            }
            delete currentNode;
        }
    }
    QueryPerformanceCounter(&timeEnd);
    LONGLONG duration = timeEnd.QuadPart - timeBegin.QuadPart;
    double timeElapsedTotal = secondsPerTick * duration;

    cout << "最短路径为: " << bestLength << endl;
    for (int i = 0; i < bestRoad.size(); i++)
    {
        cout << nodeMap->at(bestRoad.at(i)) << " ";
    }
    cout << endl
         << "time cost :" << timeElapsedTotal << "s" << endl;
    cout << "visit nodes :" << visitNodeNum << endl;
    while (H.size() > 0)
    {
        MinHeapNode *n = H.top();
        H.pop();
        delete n;
    }

    return 0;
}

void read(string fileName)
{
    nodeMap = new vector<int>;
    fstream file = fstream(fileName, ios::in);
    string firstLine;
    getline(file, firstLine);
    split(firstLine, *nodeMap, " ");
    nodeNum = nodeMap->size();
    dis = new double *[nodeNum];
    bestLength = 1000000;
    for (int i = 0; i < nodeNum; i++)
    {
        dis[i] = new double[nodeNum];
    }
    string temp;
    for (int i = 0; i < nodeNum && !file.eof(); i++)
    {
        for (int j = 0; j < nodeNum && !file.eof(); j++)
        {

            file >> temp;
            int disIJ = stod(temp);
            if (disIJ == 99999)
            {
                dis[i][j] = -1;
            }
            else
            {
                dis[i][j] = disIJ;
            }
        }
    }
    minOut = new double[nodeNum];
    for (int i = 0; i < nodeNum; i++)
    {
        double min = INT_MAX;
        for (int j = 0; j < nodeNum; j++)
        {
            if (dis[i][j] != -1 && dis[i][j] < min)
            {
                min = dis[i][j];
            }
        }
        minOut[i] = min;
    }
}

void split(const string &s, vector<int> &result, const string &delimiters)
{
    result.clear();
    string::size_type lastPos = s.find_first_not_of(delimiters, 0);
    string::size_type pos = s.find_first_of(delimiters, lastPos);
    while (string::npos != pos || string::npos != lastPos)
    {
        string temp = s.substr(lastPos, pos - lastPos);
        result.push_back(stoi(temp));
        lastPos = s.find_first_not_of(delimiters, pos);
        pos = s.find_first_of(delimiters, lastPos);
    }
}