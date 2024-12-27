#include <bits/stdc++.h>

struct jian{
    char q[25];
} p[1005];

char a,b,c,d;

bool cmp(jian x,jian y){
    for(int i=0;i<25;i++){
        if(x.q[i]==y.q[i]) continue;
        if(x.q[i]==a) return 1;
        if(y.q[i]==a) return 0;
        if(x.q[i]==b) return 1;
        if(y.q[i]==b) return 0;
        if(x.q[i]==c) return 1;
        if(y.q[i]==c) return 0;
    }
    return 0;
}

int main()
{
    int n;
    std::cin>>n;
    std::cin>>a>>b>>c>>d;
    for(int i=0;i<n;i++){
        std::cin>>p[i].q;
    }
    std::sort(p,p+n,cmp);
    for(int i=0;i<n;i++){
        std::cout<<p[i].q<<std::endl;
    }
    return 0;
}