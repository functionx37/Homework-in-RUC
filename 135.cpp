#include<bits/stdc++.h>

struct id{
    char p[20];
}a[100005];

bool cmp(id a,id b){
    for(int i=6;i<14;i++){
        if(a.p[i]>b.p[i]) return 1;
        if(a.p[i]<b.p[i]) return 0;
    }
    for(int i=0;i<18;i++){
        if(a.p[i]>b.p[i]) return 1;
        if(a.p[i]<b.p[i]) return 0;
    }
    return 0;
}

int main(){
    int n;
    std::cin>>n;
    for(int i=0;i<n;i++){
        std::cin>>a[i].p;
    }
    std::sort(a,a+n,cmp);
    for(int i=0;i<n;i++){
        std::cout<<a[i].p<<std::endl;
    }
    return 0;
}