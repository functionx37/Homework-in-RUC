#include<bits/stdc++.h>

int n,s,d;
int map[55][55];
int dis[55];
bool min[55];
int source[55];
int way[55];

void dijkstra(){
    if(min[d]) return;
    int minx=-1, miny=INT_MAX;
    for(int i=0;i<n;i++){
        if(!min[i]&&dis[i]<miny){
            minx = i;
            miny = dis[i];
        }
    }
    if(minx==-1) return;
    min[minx] = 1;
    for(int i=0;i<n;i++){
        if(map[minx][i]!=-1&&dis[minx]+map[minx][i]<dis[i]){
            dis[i] = dis[minx]+map[minx][i];
            source[i] = minx;
        }
    }
    dijkstra();
}

void search(int p){
    if(p==d){

    }
}

int main()
{
    std::cin>>n>>s>>d;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            std::cin>>map[i][j];
        }
    }
    for(int i=0;i<55;i++){
        dis[i] = INT_MAX;
    }
    dis[s] = 0;
    dijkstra();
    if(min[d]==0){
        printf("-1");
        return 0;
    }
    int go = d;
    int i = 0; 
    while(1){
        way[i] = source[go];
        go = way[i];
        if(go==s) break;
        i++;
    }
    for(;i>=0;i--){
        printf("%d->",way[i]);
    }
    printf("%d",d);
    return 0;
}