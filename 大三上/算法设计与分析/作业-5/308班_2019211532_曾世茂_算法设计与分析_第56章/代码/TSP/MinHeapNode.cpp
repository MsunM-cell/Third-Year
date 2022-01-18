/*
 * @Descripttion:
 * @version:
 * @Author: shimmerZ
 * @Date: 2022-01-05 18:02:42
 * @LastEditors: shimmerZ
 * @LastEditTime: 2022-01-05 18:17:28
 */
#include <vector>
using namespace std;
extern double **dis;
extern double *minOut;
class MinHeapNode
{
private:
public:
    MinHeapNode(int nodeNum);
    ~MinHeapNode();
    double restCost;
    int nodeNum;
    double lowerstCost;
    vector<int> currentRoad;
    double currentCost;
    int *visist;
    int getLastVisit()
    {
        return currentRoad.back();
    }
    MinHeapNode *getNext(int next);
};

MinHeapNode::MinHeapNode(int nodeNum)
{
    this->nodeNum = nodeNum;
    visist = new int[nodeNum];
}

MinHeapNode::~MinHeapNode()
{
    free(visist);
}
MinHeapNode *MinHeapNode::getNext(int next)
{
    MinHeapNode *result = new MinHeapNode(nodeNum);
    result->currentRoad = currentRoad;
    result->currentRoad.push_back(next);
    for (int i = 0; i < nodeNum; i++)
    {
        result->visist[i] = visist[i];
    }
    result->visist[next] = 1;
    int lastvisit = currentRoad.back();
    result->currentCost = currentCost + dis[lastvisit][next];
    result->restCost = restCost - minOut[lastvisit];
    result->lowerstCost = result->currentCost + result->restCost;
    return result;
}