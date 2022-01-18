#include <string>
#include <iostream>
#include <algorithm>
using namespace std;
int main(int argc, char const *argv[])
{
    string x;
    string y;
    cin >> x;
    cin >> y;
    int **dp = new int *[x.length() + 1];
    int **action = new int *[x.length() + 1];
    for (int i = 0; i <= x.length(); i++)
    {
        dp[i] = new int[y.length() + 1];
        action[i] = new int[y.length() + 1];
    }

    for (int i = 0; i <= x.length(); i++)
    {
        for (int j = 0; j <= y.length(); j++)
        {

            if (i == 0 || j == 0)
            {
                dp[i][j] = 0;
                action[i][j] = 1;
            }
            else
            {
                if (x[i - 1] == y[j - 1])
                {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                    action[i][j] = 1;
                }
                else if (dp[i - 1][j] >= dp[i][j - 1])
                {
                    dp[i][j] = dp[i - 1][j];
                    action[i][j] = 2;
                }
                else
                {
                    dp[i][j] = dp[i][j - 1];
                    action[i][j] = 3;
                }
            }
        }
    }

    int i = x.length() - 1;
    int j = y.length() - 1;
    string res = "";
    while (i >= 0 && j >= 0)
    {
        if (action[i + 1][j + 1] == 1)
        {
            res += x[i];
            i--;
            j--;
        }
        else if (action[i + 1][j + 1] == 2)
        {
            i--;
        }
        else if (action[i + 1][j + 1] == 3)
        {
            j--;
        }
    }
    reverse(res.begin(), res.end());
    cout << "最长公共子序列为：" << endl;
    cout << res << endl;
    cout << "最长公共子序列的长度为：" << endl;
    cout << dp[x.length()][y.length()];
    return 0;
}
