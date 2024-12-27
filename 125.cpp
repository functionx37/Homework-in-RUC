#include <bits/stdc++.h>

int a[10];
int b[15][15];
int n;
int ni(){
    int p=0;
    for(int i=0;i<10;i++){
        for(int j=i+1;j<10;j++){
            if(a[i]>a[j]) p++;
        }
    }
    return p%2;
}
int jie(int i){
    int p=1;
    for(int j=1;j<=i;j++){
        p*=j;
    }
    return p;
}

int main()
{
    std::cin>>n;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            std::cin>>b[i][j];
        }
    }
    int x=0;
    for(int i=0;i<n;i++){
        a[i]=i;
    }
    for(int j=1;j<=jie(n);j++){
        std::next_permutation(a,a+n);
        int y=1;
        for(int i=0;i<n;i++){
            y*=b[i][a[i]];
        }
        x+=std::pow(-1,ni())*y;
    }
    std::cout<<x;
    return 0;
}