
void print()
{
    int i;
    FILE *myfile;
    myfile=fopen("result.txt","w+");
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
    for (i=1;i<=Vnum;i++)
    {
        for (map<int,Strategy>::iterator it=EdgeList[i].begin();it!=EdgeList[i].end();it++)
           fprintf(myfile,"%d->%d state:%d %d %d\n",i,it->first,it->second.sign,it->second.Push,it->second.Pull);
    }
    fclose(myfile);
}
