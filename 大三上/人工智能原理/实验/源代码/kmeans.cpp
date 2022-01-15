#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <time.h>
#include <windows.h>
#include "Data.cpp"
#define DBL_MAX 1.7976931348623158e+308
using namespace std;
void kMeansClustering(vector<Data> *points, int k);
DWORD WINAPI UpdateMinDist(LPVOID lpParameter);
//用于多线程传递参数
typedef struct PARAMAR
{
    vector<Data> *datas;
    vector<Data> *centerPoint;
    int left;
    int right;
} parameter;

int main()
{

    fstream fp("out.csv", ios::in);
    string temp;
    vector<Data> datas;
    if (!fp.is_open())
    {
        cout << "文件打开失败" << endl;
        return 0;
    }
    while (!fp.eof())
    {
        getline(fp, temp);
        if (temp == "")
        {
            continue;
        }
        Data data(temp);
        datas.push_back(data);
    }
    fp.close();

    kMeansClustering(&datas, 95);
}

DWORD WINAPI UpdateMinDist(LPVOID lpParameter)
{
    /* 原包含参数：
    vector<Data> *datas; 数据集
    vector<Data> *centerPoint;//中心集
    int left;//该线程的任务划分起始下标
    int right;//该线程任务划分结束下标，左闭右闭类型
     */
    parameter *para = (parameter *)lpParameter;
    if (para->right < para->left)
    {
        return 0;
    }
    for (int i = para->left; i <= para->right; i++)
    {
        Data &data = para->datas->at(i);//遍历每一个该线程的划分节点
        for (auto it = para->centerPoint->begin(); it != para->centerPoint->end(); it++)//遍历每一个中心
        {
            int clusterId = it - para->centerPoint->begin();//得到中心id
            double dist = data.distance(*it);//计算距离
            if (dist < data.minDist)
            {
                data.minDist = dist;
                data.cluster = clusterId;//更新中心
            }
        }
    }
    return 0;
}

void kMeansClustering(vector<Data> *datas, int k)
{
    clock_t start, finish;
    start = clock();
    vector<Data> centerPoint;
    srand(time(0));
    map<int, int> used;
    for (int i = 0; i < k; i++)
    {
        //随机选择几个点作为起始中心点
        int temp = rand() % datas->size();
        auto it = used.find(temp);
        if (it == used.end())//如果该中心点未被选取过
        {

            used.insert({temp, 1});
            centerPoint.push_back(datas->at(temp));
        }
        else//已被选取则重新挑选
        {
            i--;
            continue;
        }
    }
    double changeX;
    double changeY;//更新中心点后每个中心点的经纬度变化最大值
    do
    {
        changeX = 0;
        changeY = 0;
        int piece = datas->size() / 4;//每个线程的任务量
        parameter para1{datas, &centerPoint, 0, piece - 1};
        parameter para2{datas, &centerPoint, piece, 2 * piece - 1};
        parameter para3{datas, &centerPoint, 2 * piece, 3 * piece - 1};
        parameter para4{datas, &centerPoint, 3 * piece, (int)(datas->size() - 1)};
        HANDLE hThread_1 = CreateThread(NULL, 0, UpdateMinDist, &para1, 0, NULL);
        HANDLE hThread_2 = CreateThread(NULL, 0, UpdateMinDist, &para2, 0, NULL);
        HANDLE hThread_3 = CreateThread(NULL, 0, UpdateMinDist, &para3, 0, NULL);
        HANDLE hThread_4 = CreateThread(NULL, 0, UpdateMinDist, &para4, 0, NULL);
        WaitForSingleObject(hThread_1, INFINITE);
        CloseHandle(hThread_1);
        WaitForSingleObject(hThread_2, INFINITE);
        CloseHandle(hThread_2);
        WaitForSingleObject(hThread_3, INFINITE);
        CloseHandle(hThread_3);
        WaitForSingleObject(hThread_4, INFINITE);
        CloseHandle(hThread_4);//等待所有线程结束

        vector<int> nPoints;
        vector<double> sumX, sumY, sumZ;

        for (int j = 0; j < k; ++j)
        {
            nPoints.push_back(0);
            sumX.push_back(0.0);
            sumY.push_back(0.0);
            sumZ.push_back(0.0);
        }

        for (vector<Data>::iterator it = datas->begin(); it != datas->end(); ++it)
        {
            int clusterId = it->cluster;
            nPoints[clusterId] += 1;
            double lat = it->latitude * M_PI / 180;
            double lon = it->longtitude * M_PI / 180;
            double cosLat = cos(lat);
            sumX[clusterId] += cosLat * cos(lon);
            sumY[clusterId] += cosLat * sin(lon);
            sumZ[clusterId] += sin(lat);
            it->minDist = DBL_MAX;
        }
        //改变每个中心点位置
        for (vector<Data>::iterator center = centerPoint.begin(); center != centerPoint.end(); ++center)
        {
            int clusterId = center - centerPoint.begin();
            if (nPoints[clusterId] == 0)
            {
                //说明该类出现了空簇
                continue;
            }
            double X = sumX[clusterId] / nPoints[clusterId];
            double Y = sumY[clusterId] / nPoints[clusterId];
            double Z = sumZ[clusterId] / nPoints[clusterId];
            double lon = atan2(Y, X);
            double hyp = sqrt(X * X + Y * Y);
            double lat = atan2(Z, hyp);
            double newLongtitude = lon * 180 / M_PI;
            double newLatitude = lat * 180 / M_PI;
            changeX = max(abs(newLatitude - center->latitude), changeX);
            changeY = max(abs(newLongtitude - center->longtitude), changeY);
            center->latitude = newLatitude;
            center->longtitude = newLongtitude;
        }
    } while (changeX > 1 || changeY > 1);//只要有一个中心经纬度变化超过1则继续聚类

    /**
     * 输出处理时间
     * **/

    finish = clock();
    double Times = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "time cost: " << Times << endl;

    //输出所有样本的坐标以及分类id
    ofstream outputFile;
    outputFile.open("output.csv");
    for (vector<Data>::iterator it = datas->begin();
         it != datas->end(); ++it)
    {
        outputFile << it->longtitude << " " << it->latitude << " " << it->cluster << endl;
    }
    outputFile.close();
    //输出所有聚类中心的坐标以及id
    ofstream outputFile2;
    outputFile2.open("centerOutput.csv");
    for (vector<Data>::iterator it = centerPoint.begin();
         it != centerPoint.end(); ++it)
    {
        outputFile2 << it->longtitude << " " << it->latitude << " " << it - centerPoint.begin() << endl;
    }
    outputFile2.close();
}
