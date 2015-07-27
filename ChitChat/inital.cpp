#include "inital.h"

extern int Vnum;
extern int Enum;
extern Hub *GraphList;
extern EdgeState *EdgeList;
extern int *Dout;
extern int *Din;
extern double  *Rp;
extern double  *Rc;
extern HubDensity  HubDensityList; //Density for Hub Graph List
extern DensityList *VDensity; //Density for each vertex in a HubGraph

bool vsort(int a1,int a2)
{
    return a1<a2;
}


//边结构序关系
bool esort(Edge Edge1,Edge Edge2)
{
    if(Edge1.u!=Edge2.u)
        return Edge1.u<Edge2.u;
    else
        return Edge1.v<Edge2.v;
}

bool ecom1(Edge Edge1,Edge Edge2)
{
    return Edge1.u<Edge2.u;
}

bool ecom2(Edge Edge1,Edge Edge2)
{
    return Edge1.v<Edge2.v;
}

bool compd(Density *D1,Density *D2)
{
    return D1->density<D2->density;
}

bool comphd(MaxHubDensity MD1,MaxHubDensity MD2)
{
    return MD1.density<MD2.density;
}

void UpdateNeighbor(int w,VSet MaxVset,int nvdx)
{
    //if max vertex in X set
    vector<Density *>::iterator it1,it2,it3;
    Density TmpDensity,*newDensity;
    if(MaxVset.vset==0){
       TmpDensity.density= GraphList[w].TmpYDeg[nvdx]/Rc[nvdx];
       it1=std::lower_bound(VDensity[w].begin(),VDensity[w].end(),&TmpDensity,compd);
       for(it2=it1;it2!=VDensity[w].end()&&(*it2)->density>TmpDensity.density-0.5;it2++)
          if((*it2)->vdx==nvdx&&(*it2)->xoy==1){
              VDensity[w].erase(it2);
              GraphList[w].TmpYDeg[nvdx]--;
              newDensity = new Density();
              newDensity->vdx=nvdx;
              newDensity->xoy=1;
              newDensity->density=GraphList[w].TmpYDeg[nvdx]/Rc[nvdx];
              for(it3=it2;it3!=VDensity[w].end()&&(*it3)->density>newDensity->density;it3++)
                ;
              VDensity[w].insert(it3,newDensity);
          }
    }
    else {
       TmpDensity.density= GraphList[w].TmpXDeg[nvdx]/Rp[nvdx];
       it1=std::lower_bound(VDensity[w].begin(),VDensity[w].end(),&TmpDensity,compd);
       for(it2=it1;it2!=VDensity[w].end()&&(*it2)->density>TmpDensity.density-0.5;it2++)
          if((*it2)->vdx==nvdx&&(*it2)->xoy==0){
              VDensity[w].erase(it2);
              GraphList[w].TmpYDeg[nvdx]--;
              newDensity = new Density();
              newDensity->vdx=nvdx;
              newDensity->xoy=0;
              newDensity->density=GraphList[w].TmpYDeg[nvdx]/Rp[nvdx];
              for(it3=it2;it3!=VDensity[w].end()&&(*it3)->density>newDensity->density;it3++)
                ;
              VDensity[w].insert(it3,newDensity);
          }
    }
}

void UpdateWithMaxVertex(int w,VSet MaxVset)
{
    int vdx = MaxVset.vdx;
    Edge TmpEdge;
    TmpEdge.u=vdx;
    vector<Edge>::iterator it;
    if(MaxVset.vset==0){
        it=std::lower_bound(GraphList[vdx].XYCrossEdge.begin(),GraphList[vdx].XYCrossEdge.end(),TmpEdge,ecom1);
        //for each neighbor
        for(;it->u!=vdx;it++)
            UpdateNeighbor(vdx,MaxVset,it->v);
    }
    else{
        it=std::lower_bound(GraphList[vdx].YXCrossEdge.begin(),GraphList[vdx].YXCrossEdge.end(),TmpEdge,ecom2);
        for(;it->v!=vdx;it++)
            UpdateNeighbor(vdx,MaxVset,it->u);
    }
}

//make the deneset hub-graph
void InitHubDenisty(int vdx)
{
    VSet TmpVset;
    MaxHubDensity TmpMaxHubDensity;
    double density;
    GraphList[vdx].maxdenest=GraphList[vdx].EgSum/GraphList[vdx].WSum;
    while(VDensity[vdx].size()>0)
    {
        TmpVset.vdx = (*(VDensity[vdx].begin()))->vdx;
        TmpVset.vset= (*(VDensity[vdx].begin()))->xoy;
        GraphList[vdx].Order.push_back(TmpVset);
        VDensity[vdx].erase(VDensity[vdx].begin());
        if(TmpVset.vset==0)
        {
            GraphList[vdx].TmpEgSum-=GraphList[vdx].TmpYDeg[TmpVset.vdx];
            GraphList[vdx].TmpWSum-=Rp[vdx];
        }
        else
        {
            GraphList[vdx].TmpEgSum-=GraphList[vdx].TmpXDeg[TmpVset.vdx];
            GraphList[vdx].TmpWSum-=Rc[vdx];
        }
        density=GraphList[vdx].TmpEgSum/GraphList[vdx].TmpWSum;
        UpdateWithMaxVertex(vdx,TmpVset);
        if(density>GraphList[vdx].maxdenest){
            GraphList[vdx].maxdenest=density;
            GraphList[vdx].index=VDensity[vdx].size();
           }
        }
        TmpMaxHubDensity.density=GraphList[vdx].maxdenest;
        TmpMaxHubDensity.vdx=vdx;
        HubDensityList.push_back(TmpMaxHubDensity);
}

bool comp(Density* v1,Density* v2)
{
    return v1->density<v2->density;
}


//Inital each hub-graph's each vertex density
void InitalVDenisty()
{
    int i;
    Density *pDensityStruct;
    for(i=1;i<=Vnum;i++){
        if(GraphList[i].valid){
        for(vector<int>::iterator it=GraphList[i].X.begin();it!=GraphList[i].X.end();it++){
            pDensityStruct=new Density();
            pDensityStruct->vdx=*it;
            pDensityStruct->xoy=0;
            pDensityStruct->density=GraphList[i].XDeg[*it]/Rp[*it];
            VDensity[i].push_back(pDensityStruct);
         }
        for(vector<int>::iterator it=GraphList[i].Y.begin();it!=GraphList[i].Y.end();it++){
            pDensityStruct = new Density();
            pDensityStruct->vdx=*it;
            pDensityStruct->xoy=1;
            pDensityStruct->density=GraphList[i].YDeg[*it]/Rc[*it];
            VDensity[i].push_back(pDensityStruct);
         }
        }
    }
    for(i=1;i<=Vnum;i++)
        if(GraphList[i].valid)
            std::sort(VDensity[i].begin(),VDensity[i].end(),comp);
}


/*read from user.txt and link.txt,create and malloc memory to
the hub-graph structure and other structure*/
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
    VDensity = new DensityList[(Vnum+1)];

    //malloc memory to structure according to number of user
    Dout =(int *)malloc((Vnum+1)*sizeof(int));
    Din  =(int *)malloc((Vnum+1)*sizeof(int));
    Rp =(double *)malloc((Vnum+1)*sizeof(double));
    Rc =(double *)malloc((Vnum+1)*sizeof(double));
    memset(Dout,0,(Vnum+1)*sizeof(int));
    memset(Din,0,(Vnum+1)*sizeof(int));
    memset(Rp,0,(Vnum+1)*sizeof(double));
    memset(Rc,0,(Vnum+1)*sizeof(double));

    //read the edge from link.txt
    link = fopen("link.txt","r+");
    for(int i=1;i<=Vnum;i++)
        GraphList[i].valid=false;
    struct Strategy temp;
    temp.sign='0';
    temp.Pull='0';
    temp.Push='0';
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

void OptHub()
{
    int i;
    //sort the X and Y edge set
    for(i=1;i<=Vnum;i++){
        std::sort(GraphList[i].X.begin(),GraphList[i].X.end(),vsort);
        std::sort(GraphList[i].Y.begin(),GraphList[i].Y.end(),vsort);
    }

    //create the crossedge
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
                        GraphList[i].XYCrossEdge.push_back(Edge(xv,*it2));
                        GraphList[i].YXCrossEdge.push_back(Edge(*it2,xv));
                        it1++;
                        it2++;
                    }
                }
       }
    for(i=1;i<=Vnum;i++){
        std::sort(GraphList[i].XYCrossEdge.begin(),GraphList[i].XYCrossEdge.end(),esort);
        std::sort(GraphList[i].YXCrossEdge.begin(),GraphList[i].YXCrossEdge.end(),esort);
    }
    //compute the degree in the hub-graph
    for(i=1;i<=Vnum;i++)
    {
     for(vector<int>::iterator it=GraphList[i].X.begin();it!=GraphList[i].X.end();it++)
          GraphList[i].XDeg[*it]=1;

     for(vector<int>::iterator it=GraphList[i].Y.begin();it!=GraphList[i].Y.end();it++)
          GraphList[i].YDeg[*it]=1;

     for(vector<Edge>::iterator it=GraphList[i].XYCrossEdge.begin();it!=GraphList[i].XYCrossEdge.end();it++){
            GraphList[i].XDeg[it->u]++;
            GraphList[i].YDeg[it->v]++;
        }
    }
    for(i=1;i<=Vnum;i++){
        for(vector<int>::iterator it=GraphList[i].X.begin();it!=GraphList[i].X.end();it++)
              GraphList[i].TmpXDeg[*it]=GraphList[i].XDeg[*it];

        for(vector<int>::iterator it=GraphList[i].Y.begin();it!=GraphList[i].Y.end();it++)
              GraphList[i].TmpYDeg[*it]=GraphList[i].YDeg[*it];
    }
    //inital Rp and Rc according to in degree and out degree
    for(i=1;i<=Vnum;i++)
        if(GraphList[i].valid)
    {
        Rp[i]=(double)log(double(Din[i]+2));
        Rc[i]=(double)log(double(Dout[i]+2));
    }
    //compute the sum of weight
    for(i=1;i<=Vnum;i++){
        GraphList[i].EgSum=(GraphList[i].X.size()+GraphList[i].Y.size()+GraphList[i].XYCrossEdge.size());
        GraphList[i].TmpEgSum=GraphList[i].EgSum;
    }
}

void inital()
{
    int i;
    ReadInput();
    OptHub();
    InitalVDenisty();
    for(i=1;i<=Vnum;i++)
        InitHubDenisty(i);
    std::sort(HubDensityList.begin(),HubDensityList.end(),comphd);
}
