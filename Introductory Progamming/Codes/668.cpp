#include <bits/stdc++.h>

struct name{
    char x[25];
}a[25];

bool cmp(name a,name b){
    int m = std::max(strlen(a.x),strlen(b.x));
    for(int i=0;i<m;i++){
        if(a.x[i]>b.x[i]) return 0;
        if(a.x[i]<b.x[i]) return 1;
    }
    return 0;
}

int main()
{
    int n;
    std::cin>>n;
    for(int i=0;i<n;i++){
        std::cin>>a[i].x;
        std::sort(a[i].x,a[i].x+strlen(a[i].x));
    }
    std::sort(a,a+n,cmp);
    int p=1;
    for(int i=1;i<n;i++){
        if(strcmp(a[i].x,a[i-1].x)!=0){
            p++;
        }
    }
    std::cout<<p;
    return 0;
}
