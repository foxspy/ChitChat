#include <stdio.h>
#include <string.h>
#include <iostream>

#include <map>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

struct Edge
{
    int u;
    int v;
    Edge(int x,int y)
    {
        u=x;
        v=y;
    }
};

typedef struct
{
    bool valid;
    vector<int> X;
    vector<int> Y;
    vector<Edge> CrossEdge;
    vector<int>  Order;
    map<int,int> XDeg;
    map<int,int> XPDeg;
    map<int,int> YDeg;
    map<int,int> YPDeg;
    int    index;
    double maxdenest;
}Hub;

struct Strategy
{
    char sign;
    char Push;
    char Pull;
};

struct VDensity
{
    int vdx;
    double density;
    VDensity(int index,double d)
    {
        vdx=index;
        density=d;
    }
};

typedef map<int,Strategy> EdgeState;

int   Vnum;
int   Enum;
int   RestEdge;
int  *Dout;
int  *Din;
int  *Rp;
int  *Rc;
int   Cur_Flag;

Hub *GraphList;
EdgeState *EdgeList;
priority_queue<Hub *> HubQ;
typedef priority_queue<VDensity> HubDensity;

HubDensity *HubDensityList;
//点结构序关系

bool operator <( Hub *a, Hub *b )
{
    return a->maxdenest<b->maxdenest;
}

bool vsort(const int &a1,const int &a2)
{
    return a1<a2;
}

//边结构序关系
bool esort(const Edge &Edge1,const Edge &Edge2)
{
    if(Edge1.u!=Edge2.u)
        return Edge1.u<Edge2.u;
    else
        return Edge1.v<Edge2.v;
}

//初始化输入，包括全局节点度数分布，Hub结构链

void ReadInput()
{
    FILE *user,*link;
    int  u,v;

    user = fopen("user.txt","r+");
    while(!feof(user))
        fscanf(user,"%d",&Vnum);
    fclose(user);

    GraphList = new Hub[(Vnum+1)];
    EdgeList = new EdgeState[(Vnum+1)];
    VDensityList = new HubDensity[(Vnum+1)];

    Dout =(int *)malloc((Vnum+1)*sizeof(int));
    Din  =(int *)malloc((Vnum+1)*sizeof(int));
    memset(Dout,0,(Vnum+1)*sizeof(int));
    memset(Din,0,(Vnum+1)*sizeof(int));

    link = fopen("link.txt","r+");
    for(int i=1;i<=Vnum;i++)
        GraphList[i].valid=false;
    struct Strategy temp{'0','0','0'};
    while(!feof(link)){
        fscanf(link,"%d%d",&u,&v);
        GraphList[v].valid=true;
        GraphList[u].valid=true;
        GraphList[v].X.push_back(u);
        GraphList[u].Y.push_back(v);
        EdgeList[u][v]=temp;
        Dout[u]++;
        Din[v]++;
        Enum++;
    }
    fclose(link);

}

void InitHubDenisty(int vdx)
{
    int i;
    for(vector<int>::iterator it=GraphList[i].X.begin();it!=GraphList[i].X.end();it++)
        HubDensityList[vdx].push(VDensity(*it,GraphList[vdx].XDeg[*it]/Rp[vdx]));
    while()
}

void InitalDenisty()
{
    int i;
    for(i=1;i<=Vnum;i++)
       if(GraphList[i].valid)
         InitHubDenisty(i);
    for(i=1;i<=Vnum;i++)
       if(GraphList[i].valid)
        HubQ.push(&GraphList[i]);
}

void MaxDenisty()
{

}

void UpdateWithMax()
{

}

void OptHub()
{
    for(i=1;i<=Vnum;i++){
        std::sort(GraphList[i].X.begin(),GraphList[i].X.end(),vsort);
        std::sort(GraphList[i].Y.begin(),GraphList[i].Y.end(),vsort);
    }

    for(i=1;i<=Vnum;i++)
       for(vector<int>::iterator it=GraphList[i].X.begin();it!=GraphList[i].X.end();it++){
            int xv=*it;
            vector<int>::iterator it1=GraphList[xv].Y.begin();
            vector<int>::iterator it2=GraphList[i].Y.begin();
            for(;it1!=GraphList[xv].Y.end()&&it2!=GraphList[i].Y.end();){
                    if(*it1<*it2)
                        it1++;
                    else if(*it1>*it2)
                        it2++;
                    else {
                        GraphList[i].CrossEdge.push_back(Edge(xv,*it2));
                        it1++;
                        it2++;
                    }
                }
       }

    for(i=1;i<=Vnum;i++)
    {
     for(vector<int>::iterator it=GraphList[i].X.begin();it!=GraphList[i].X.end();it++)
          GraphList[i].XDeg[*it]=1;

     for(vector<int>::iterator it=GraphList[i].Y.begin();it!=GraphList[i].Y.end();it++)
          GraphList[i].YDeg[*it]=1;

     for(vector<Edge>::iterator it=GraphList[i].CrossEdge.begin();it!=GraphList[i].CrossEdge.end();it++){
            GraphList[i].XDeg[it->u]++;
            GraphList[i].YDeg[it->v]++;
        }
    }
    for(i=1;i<=Vnum;i++){
        for(vector<int>::iterator it=GraphList[i].X.begin();it!=GraphList[i].X.end();it++)
              GraphList[i].XPDeg[*it]=GraphList[i].XDeg[*it];

        for(vector<int>::iterator it=GraphList[i].Y.begin();it!=GraphList[i].Y.end();it++)
              GraphList[i].YPDeg[*it]=GraphList[i].YDeg[*it];
    }
}
void inital()
{
    Cur_Flag=0;
    ReadInput();
    OptHub();
    InitDenisty();
}

void PrintHub()
{
    int i;
    FILE *myfile;
    myfile=fopen("Hub.txt","w+");
    for(i=1;i<=Vnum;i++)
    {
        fprintf(myfile,"w:%d\n",i);
        fprintf(myfile,"X:");
        for(vector<int>::iterator it=GraphList[i].X.begin();it!=GraphList[i].X.end();it++)
            fprintf(myfile,"%d ",*it);
        fprintf(myfile,"\n");
        fprintf(myfile,"Y:");
        for(vector<int>::iterator it=GraphList[i].Y.begin();it!=GraphList[i].Y.end();it++)
            fprintf(myfile,"%d ",*it);
        fprintf(myfile,"\n");
        fprintf(myfile,"Edge:");
        for(vector<Edge>::iterator it=GraphList[i].CrossEdge.begin();it!=GraphList[i].CrossEdge.end();it++)
            fprintf(myfile,"(%d,%d) ",it->u,it->v);
        fprintf(myfile,"\n");
        fprintf(myfile,"\n\n");
    }
     fclose(myfile);
}

void PrintEdge()
{
    int i;
    FILE *myfile;
    myfile=fopen("Edge.txt","w+");
    for (i=1;i<=Vnum;i++)
    {
        for (map<int,Strategy>::iterator it=EdgeList[i].begin();it!=EdgeList[i].end();it++)
           fprintf(myfile,"%d->%d state:%d %d %d\n",i,it->first,it->second.sign,it->second.Push,it->second.Pull);
    }
    fclose(myfile);
}

void Print()
{
    PrintHub();
    PrintEdge();
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
