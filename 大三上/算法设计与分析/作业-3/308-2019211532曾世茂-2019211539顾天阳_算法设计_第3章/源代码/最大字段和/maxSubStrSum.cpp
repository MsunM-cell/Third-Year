#include <vector>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;
int main(int argc, char const *argv[])
{
    vector<int> array;
    fstream file;
    file.open("file3.txt");
    string temp;
    while (!file.eof())
    {
        file >> temp;
        array.push_back(atoi(temp.c_str()));
    }
    int *dp = new int[array.size()];
    int *startIndex = new int[array.size()];
    dp[0] = array.at(0);
    startIndex[0] = 0;
    int maxIndex = 0;
    int max = 0;
    for (int i = 1; i < array.size(); i++)
    {
        if (dp[i - 1] >= 0)
        {
            dp[i] = dp[i - 1] + array.at(i);
            startIndex[i] = startIndex[i - 1];
        }
        else
        {
            dp[i] = array.at(i);
            startIndex[i] = i;
        }
        if (dp[i] > max)
        {
            max = dp[i];
            maxIndex = i;
        }
    }
    cout << "最大字段和为" << dp[maxIndex] << endl;
    cout << "区间范围为： (" << startIndex[maxIndex] << "," << maxIndex << ")" << endl;
    file.close();
    return 0;
}
