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
HubDensity  HubDensityList; //Density for Hub Graph List
DensityList *VDensity; //Density for each vertex in a HubGraph

void MaxDenisty()
{
    int vdx;
    int counter;
    Edge TmpEdge;
    vector<int> X,Y;
    vector<VSet>::iterator it;
    vector<Edge>::iterator itE;
    MaxHubDensity TmpHubDensity;
    TmpHubDensity=*(HubDensityList.begin());
    vdx=TmpHubDensity.vdx;
    for(it=GraphList[vdx].Order.end(),counter=0;counter<GraphList[vdx].index;counter++,it--){
            if(it->vset==0){
                if(EdgeList[it->vdx][vdx].sign==0){
                    EdgeList[it->vdx][vdx].sign=1;
                    RestEdge--;
                }
                EdgeList[it->vdx][vdx].Push=1;
                TmpEdge.u=it->vdx;
                GraphList[vdx].EgSum-=GraphList[vdx].XDeg[itE->u];
                GraphList[vdx].XDeg[itE->u]=0;
                itE=std::lower_bound(GraphList[vdx].XYCrossEdge.begin(),GraphList[vdx].XYCrossEdge.end(),TmpEdge,ecom1);
                for(;itE->u!=TmpEdge.u;itE++){
                    GraphList[vdx].YDeg[itE->v]--;
                    GraphList[vdx].TmpYDeg[itE->v]=GraphList[vdx].YDeg[itE->v];
                }
            else if(it->vset==1){
                if(EdgeList[vdx][it->vdx].sign==0){
                    EdgeList[vdx][it->vdx].sign=1;
                    RestEdge--;
                }
                EdgeList[vdx][it->vdx].Pull=1;
                TmpEdge.v=it->vdx;
                GraphList[vdx].EgSum-=GraphList[vdx].XDeg[itE->u];
                GraphList[vdx].XDeg[itE->u]=0;
                itE=std::lower_bound(GraphList[vdx].YXCrossEdge.begin(),GraphList[vdx].YXCrossEdge.end(),TmpEdge,ecom2);
                for(;itE->u!=TmpEdge.u;itE++){
                    GraphList[vdx].XDeg[itE->u]--;
                    GraphList[vdx].TmpXDeg[itE->u]=GraphList[vdx].XDeg[itE->u];
            }
    }

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
