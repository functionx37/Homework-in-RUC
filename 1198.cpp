#include <bits/stdc++.h>

int n,m;

struct rep{
    int p[15];
}a[1005];

void bos(int k){
    for(int i=0;i<n;i++){
        bool f = 1;
        for(int j = 0;j<m;j++){
            if(a[i].p[j]<=a[k].p[j]){
                f = 0;
                break;
            }
        }
        if(f){
            printf("%d\n",i+1);
            return;
        }
    }
    printf("0\n");
}

int main()
{
    std::cin>>n>>m;
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++)
            std::cin>>a[i].p[j];
    }
    for(int i=0;i<n;i++){
        bos(i);
    }
    return 0;
}