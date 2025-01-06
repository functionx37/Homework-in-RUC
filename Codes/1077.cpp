#include<bits/stdc++.h>

struct people{
    int id;
    int rednum = 0;
    int money = 0;
} a[105];

bool cmp(people x,people y){
    if(x.money>y.money) return 1;
    if(x.money<y.money) return 0;
    if(x.rednum>y.rednum) return 1;
    if(x.rednum<y.rednum) return 0;
    if(x.id<y.id) return 1;
    return 0;
}

int main()
{
    int n;
    std::cin>>n;
    for(int i=1;i<=n;i++){
        int m;
        std::cin>>m;
        a[i].id = i;
        for(int j=0;j<m;j++){
            int p,q;
            std::cin>>p>>q;
            a[p].money += q;
            a[i].money -= q;
            a[p].rednum ++;
        }
    }
    std::sort(a+1,a+n+1,cmp);
    for(int i=1;i<=n;i++){
        printf("%d %d %d\n",a[i].id,a[i].rednum,a[i].money);
    }
    return 0;
}