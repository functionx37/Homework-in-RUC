#include <bits/stdc++.h>

struct name{
    char x[65];
}a[205],b[205];

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
    }
    for(int i=0;i<n;i++){
        std::cin>>b[i].x;
    }
    std::sort(a,a+n,cmp);
    std::sort(b,b+n,cmp);
    for(int i=0;i<n;i++){
        printf("%s %s\n",a[i].x,b[i].x);
    }
    return 0;
}
