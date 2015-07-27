#pragma once
#include <map>
#include <vector>

using namespace std;

struct Edge
{
    int u;
    int v;
    Edge(){}
    Edge(int x,int y)
    {
        u=x;
        v=y;
    }
};

struct VSet
{
    int  vdx;
    char vset;
};

typedef struct
{
    bool valid;
    vector<int> X;
    vector<int> Y;
    vector<Edge> XYCrossEdge;
    vector<Edge> YXCrossEdge;
    vector<VSet> Order;
    int  index;
    double maxdenest;

    map<int,int> XDeg;
    map<int,int> TmpXDeg;
    map<int,int> YDeg;
    map<int,int> TmpYDeg;
    int  EgSum;
    int  TmpEgSum;
    double  WSum;
    double  TmpWSum;
}Hub;

struct Strategy
{
    char sign;
    char Push;
    char Pull;
};

struct Density
{
    int  vdx;
    char xoy;
    double density;
};

struct MaxHubDensity
{
    int vdx;
    double density;
};

typedef map<int,Strategy> EdgeState;
typedef vector<Density*> DensityList;
typedef vector<MaxHubDensity> HubDensity;
typedef vector<Edge> DesetEdge;
