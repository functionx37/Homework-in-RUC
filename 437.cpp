#include<bits/stdc++.h>

int n;
int map[15][15];
int xx0,yy0,xx1,yy1,xx2,yy2;

int min11 = 1000;
int min12 = 0;
int min21 = 1000;
int min22 = 0;

void s1(int x,int y){
    if(x==xx1&&y==yy1){
        if(min12<min11){
            min11=min12;
        }
        return;
    }
    if(map[x][y+1]!=1&&y+1<=n){
        min12++;
        map[x][y+1]=1;
        s1(x,y+1);
        min12--;
        map[x][y+1]=0;
    }
    if(map[x+1][y]!=1&&x+1<=n){
        map[x+1][y]=1;
        min12++;
        s1(x+1,y);
        min12--;
        map[x+1][y]=0;
    }
    if(map[x][y-1]!=1&&y-1>0){
        map[x][y-1]=1;
        min12++;
        s1(x,y-1);
        min12--;
        map[x][y-1]=0;
    }
    if(map[x-1][y]!=1&&x-1>0){
        map[x-1][y]=1;
        min12++;
        s1(x-1,y);
        min12--;
        map[x-1][y]=0;
    }
}

void s2(int x,int y){
    if(x==xx2&&y==yy2){
        if(min22<min21){
            min21=min22;
        }
        return;
    }
    if(map[x][y+1]!=1&&y+1<=n){
        map[x][y+1] = 1;
        min22++;
        s2(x,y+1);
        min22--;
        map[x][y+1] = 0;
    }
    if(map[x+1][y]!=1&&x+1<=n){
        map[x+1][y] = 1;
        min22++;
        s2(x+1,y);
        min22--;
        map[x+1][y] = 0;
    }
    if(map[x][y-1]!=1&&y-1>0){
        map[x][y-1]=1;
        min22++;
        s2(x,y-1);
        min22--;
        map[x][y-1]=0;
    }
    if(map[x-1][y]!=1&&x-1>0){
        map[x-1][y]=1;
        min22++;
        s2(x-1,y);
        min22--;
        map[x-1][y]=0;
    }
}

int main()
{
    std::cin>>n;
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++){
            std::cin>>map[i][j];
            if(map[i][j]==2){
                xx0 = i;
                yy0 = j;
            }
            if(map[i][j]==3){
                xx1 = i;
                yy1 = j;
            }
            if(map[i][j]==4){
                xx2 = i;
                yy2 = j;
            }
        }
    }
    for(int i=0;i<15;i++){
        map[0][i] = 1;
        map[i][0] = 1;
        map[n+1][i] = 1;
        map[i][n+1] = 1;
    }
    map[xx0][yy0]=1;
    s1(xx0,yy0);
    map[xx0][yy0]=0;
    map[xx1][yy1]=1;
    s2(xx1,yy1);
    map[xx1][yy1]=0;
    std::cout<<min11+min21;
    return 0;
}