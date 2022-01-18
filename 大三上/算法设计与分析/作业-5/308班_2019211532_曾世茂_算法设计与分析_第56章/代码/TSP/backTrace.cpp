/*
 * @Descripttion:
 * @version:
 * @Author: shimmerZ
 * @Date: 2022-01-02 00:53:42
 * @LastEditors: shimmerZ
 * @LastEditTime: 2022-01-05 21:43:14
 */

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>
using namespace std;
vector<int> *nodeMap;      //节点名对应表，将题目中不规律节点顺序映射为从0开始
double **dis;              //两点间的距离
int nodeNum;               //节点数量
int *visit;                //是否访问过某节点
double currentLength;      //当前长度
double bestLength;         //当前最短回路长度
vector<int> road;          //当前路径
vector<int> bestRoad;      //当前最短路径
int start;                 //开始节点
long int visitNodeNum = 0; //访问节点数量
void split(const string &s, vector<int> &result, const string &delimiters);
void backTrace(int n);
void read(string fileName);
int main()
{
    LARGE_INTEGER timeEnd, timeBegin;
    QueryPerformanceFrequency(&timeEnd);
    double secondsPerTick = 1.0 / timeEnd.QuadPart;
    read("15.txt");
    for (int i = 0; i < nodeMap->size(); i++)
    {
        if (nodeMap->at(i) == 20)
        {
            start = i;
            break;
        }
    }
    road.push_back(start);
    visit[start] = 1;
    QueryPerformanceCounter(&timeBegin);
    backTrace(1);
    QueryPerformanceCounter(&timeEnd);
    cout << "最短路径为: " << bestLength << endl;
    for (int i = 0; i < bestRoad.size(); i++)
    {
        cout << nodeMap->at(bestRoad.at(i)) << " ";
    }
    LONGLONG duration = timeEnd.QuadPart - timeBegin.QuadPart;
    double timeElapsedTotal = secondsPerTick * duration;
    cout << endl
         << "time cost :" << timeElapsedTotal << "s" << endl;
    cout << "visit nodes :" << visitNodeNum << endl;
    for (int i = 0; i < nodeNum; i++)
    {
        free(dis[i]);
    }
    free(dis);
    free(visit);
    free(nodeMap);
}

void backTrace(int n)
{
    visitNodeNum++;
    if (n >= nodeNum)
    {
        //这里还要写回去的
        int lastNode = road.back();
        if (dis[start][lastNode] == -1)
        {
            return; //没有路回去了
        }
        currentLength += dis[start][lastNode];
        if (currentLength <= bestLength)
        {
            bestLength = currentLength;
            bestRoad = road;
        }
        currentLength -= dis[start][lastNode];
        return;
    }
    else
    {
        for (int i = 0; i < nodeNum; i++)
        {
            int lastNode = road.back();
            if (visit[i] == 1 || dis[lastNode][i] == -1 || currentLength + dis[lastNode][i] >= bestLength)
            {
                continue;
            }
            road.push_back(i);
            visit[i] = 1;
            currentLength += dis[lastNode][i];
            backTrace(n + 1);
            currentLength -= dis[lastNode][i];
            visit[i] = 0;
            road.erase(road.end() - 1);
        }
    }
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
    visit = new int[nodeNum];
    currentLength = 0;
    bestLength = INT_MAX;
    for (int i = 0; i < nodeNum; i++)
    {
        dis[i] = new double[nodeNum];
        visit[i] = 0;
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

// double bound(){
//     double c = currentLength;
//     int* v = new int[nodeNum];
//     for(int i = 0 ; i < nodeNum ; i++){
//         v[i] = visit[i];
//     }

// }