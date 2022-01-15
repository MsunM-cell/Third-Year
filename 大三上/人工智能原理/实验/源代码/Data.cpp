#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <math.h>
#define DBL_MAX 1.7976931348623158e+308
#define EARTH_RADIUS 6378.137
#define M_PI 3.14159265358979323846
using namespace std;

class Data
{
private:
    string msi;
    long long int time;
    int state;
    int volicity;
    int load;

public:
    double latitude;   //纬度
    double longtitude; //经度
    int cluster;
    double minDist;
    double rad(double LatOrLon);
    static const int UNDER_WAY_USING_ENGINE = 0;
    static const int AT_ANCHOR = 1;
    static const int NOT_UNDERCOMMAND = 2;
    static const int RESTRICTED_MANOEUVRABILITY = 3;
    static const int CONSTRAINEDBYDRAUGHT = 4;
    static const int MOORED = 5;
    static const int AGROUND = 6;
    static const int ENGAGED_IN_FISHING = 7;
    static const int UNDER_WAY_SAILING = 8;
    double GetDistance2(double lng1, double lat1, double lng2, double lat2);
    Data(string data);
    ~Data();
    int getState()
    {
        return state;
    }
    string getMsi()
    {
        return msi;
    }
    void setLoad(int load)
    {
        this->load = load;
    }
    int getLoad()
    {
        return load;
    }
    double distance(Data data)
    {
        return GetDistance2(data.longtitude, data.latitude, longtitude,latitude);
    }

    string toString();
    void static split(const string &s, vector<string> &result, const string &delimiters = " ");
};

Data::Data(string s)
{
    cluster = -1;
    minDist = DBL_MAX;
    vector<string> splitData;
    split(s, splitData);
    msi = splitData.at(0);
    char *stopString;
    time = strtoll(splitData.at(1).c_str(), &stopString, 10);
    state = atoi(splitData.at(2).c_str());
    volicity = atoi(splitData.at(3).c_str());
    longtitude = strtod(splitData.at(4).c_str(), &stopString);
    latitude = strtod(splitData.at(5).c_str(), &stopString);
    load = atoi(splitData.at(6).c_str());
}


string Data::toString()
{
    string res;
    res += msi + " " + to_string(time) + " " + to_string(state) + " " + to_string(volicity) + " " + to_string(latitude) + " " + to_string(longtitude) + " " + to_string(load);
    return res;
}


Data::~Data()
{
}

void Data::split(const string &s, vector<string> &result, const string &delimiters)
{
    if (s == "")
    {
        return;
    }
    result.clear();
    string::size_type lastPos = s.find_first_not_of(delimiters, 0);
    string::size_type pos = s.find_first_of(delimiters, lastPos);
    while (string::npos != pos || string::npos != lastPos)
    {
        result.push_back(s.substr(lastPos, pos - lastPos));
        lastPos = s.find_first_not_of(delimiters, pos);
        pos = s.find_first_of(delimiters, lastPos);
    }
}

double Data::GetDistance2(double lng1, double lat1, double lng2, double lat2)
{
    double radLat1 = rad(lat1);
    double radLat2 = rad(lat2);
    double radLng1 = rad(lng1);
    double radLng2 = rad(lng2);
    double s = acos(cos(radLat1) * cos(radLat2) * cos(radLng1 - radLng2) + sin(radLat1) * sin(radLat2));
    s = s * EARTH_RADIUS;
    s = round(s * 1000 * 1000000) / 1000000;
    return s;
}

double Data::rad(double LatOrLon)
{
    return LatOrLon * M_PI / 180.0;
}