#include <bits/stdc++.h>

int a[105][105];
int b[105][105];
int n,m;

int main(){
    std::cin>>n>>m;
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            std::cin>>a[i][j];
        }
    }
    int p;
    std::cin>>p;
    while(p<0){
        p+=360;
    }
    while(p>=360){
        p-=360;
    }
    p /= 90;
    if(p==0){
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                std::cout<<a[i][j]<<" ";
            }
            std::cout<<"\n";
        }
    }
    if(p==2){
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                b[i][j]=a[n-i-1][m-j-1];
                std::cout<<b[i][j]<<" ";
            }
            std::cout<<"\n";
        }
    }
    if(p==3){
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                b[m-j-1][i]=a[i][j];
            }
        }
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                std::cout<<b[i][j]<<" ";
            }
            std::cout<<"\n";
        }
    }
    if(p==1){
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                b[j][n-i-1]=a[i][j];
            }
        }
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                std::cout<<b[i][j]<<" ";
            }
            std::cout<<"\n";
        }
    }
    return 0;
}