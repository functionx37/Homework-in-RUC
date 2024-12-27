#include <bits/stdc++.h>

struct cla{
    int time;
    int f;
}a[49]={};

bool cmp(cla a,cla b){
    if(a.f<b.f) return 1;
    if(a.f>b.f) return 0;
    if(a.time<b.time) return 1;
    return 0;
}

int main()
{
    int n,k;
    std::cin>>n>>k;
    for(int i=0;i<n;i++){
        long long num;
        std::cin>>num;
        int p;
        std::cin>>p;
        for(int i=0;i<p;i++){
            int x,y;
            scanf("%d.%d",&x,&y);
            a[(x-1)*7+y-1].f++;
        }
    }
    for(int i=0;i<49;i++){
        a[i].time = i;
    }
    std::sort(a,a+49,cmp);
    for(int i=0;i<k;i++){
        printf("%d.%d %d\n",(a[i].time/7)+1,(a[i].time%7)+1,a[i].f);
    }
    return 0;
}