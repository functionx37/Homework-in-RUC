#include<bits/stdc++.h>

int a[205];
int n;
int total = 0;
int cun[105][205];


int main(){
    std::cin>>n;
    for(int i=0;i<n;i++){
        std::cin>>a[i];
    }
    for(int i=0;i<n-1;i++){
        int minx,miny=10000;
        for(int j=i;j<n;j++){
            if(a[j]<miny){
                miny = a[j];
                minx = j;
            }
        }
        if(minx!=i){
            total++;
            std::swap(a[i],a[minx]);
            cun[total][0] = i+1;
            cun[total][1] = minx+1;
            for(int i=0;i<n;i++){
                cun[total][i+2] = a[i];
            }
        }
    }
    for(int i=total;i>0;i--){
        std::cout<<cun[i][0]<<"<->"<<cun[i][1]<<":";
        for(int j=0;j<n;j++){
            std::cout<<cun[i][j+2]<<" ";
        }
        std::cout<<std::endl;
    }
    printf("Total steps:%d\nRight order:",total);
    for(int i=0;i<n;i++){
        std::cout<<a[i]<<" ";
    }
    return 0;
}