#include <cstdio>
#include <cstring>
#include <iostream>

#include "main.h"
#include "inital.h"
#include <map>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

int   Vnum;
int   Enum;
int   RestEdge;
int  *Dout;
int  *Din;
int  *Rp;
int  *Rc;

Hub *GraphList;
EdgeState *EdgeList;
DensityList  HubDensity; //Density for Hub Graph List
DensityList *VDensity; //Density for each vertex in a HubGraph

void MaxDenisty()
{

}

void UpdateWithMax()
{

}

int main()
{
    inital();
    while(RestEdge)
    {
        MaxDenisty();
        UpdateWithMax();
    }
    //Print();
}
