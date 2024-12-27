#include <bits/stdc++.h>

struct stu{
    int id;
    int ch;
    int ma;
    int en;
    int th;
    int sum;
    int se;
} a[105];

int cmp(stu x,stu y){
    if(x.sum>y.sum) return 1;
    if(x.sum<y.sum) return -1;
    if(x.ch>y.ch) return 1;
    if(x.ch<y.ch) return -1;
    if(x.ma>y.ma) return 1;
    if(x.ma<y.ma) return -1;
    if(x.en>y.en) return 1;
    if(x.en<y.en) return -1;
    return 0;
}

int main(){
    int n;
    std::cin>>n;
    for(int i=0;i<n;i++){
        std::cin>>a[i].id>>a[i].ch>>a[i].ma>>a[i].en>>a[i].th;
        a[i].sum = a[i].ch+a[i].ma+a[i].en+a[i].th;
    }
    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            if(cmp(a[i],a[j])<0){
                std::swap(a[i],a[j]);
            }
        }
    }
    int p;
    std::cin>>a[0].se>>p;
    for(int i=1;i<n;i++){
        a[i].se = a[i-1].se + cmp(a[i-1],a[i]);
    }
    for(int i=0;i<n;i++){
        if(a[i].id==p){
            std::cout<<a[i].se;
            break;
        }
    }
    return 0;
}